//
// Created by rbn on 11/4/21.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIST_LENGTH 10
#define KEY_SIZE 255
#define VAL_1_SIZE 255
#define TUPLE_SIZE KEY_SIZE + 1 + VAL_1_SIZE + 1 + VAL_1_SIZE + 1 + VAL_1_SIZE + 1

#include "database.h"

/* This is just to check that it works separately - no real use when compiled with other files
struct tuple {
    char key[255];
    char value1[255];
    int value2;
    float value3;
};
*/


char * db;

int isValue(char * key); // is used before it's declared, so it has to be here for proper compilation

// Auxiliar method - returns 1 if same, 0 otherwise
int compareStrings(int len, char * str1, char * str2){
    //printf("Comparing Strings %s and %s\n", str1, str2);
    for(int ii = 0; ii < len; ii++){
        //printf("str1[%d] == %c, str2[%d] == %c\n", ii, str1[ii], ii, str2[ii]);
        if(str1[ii] != str2[ii]){
            return 0;
        }
    }
    return 1;
}

/* Starts the database service */
int startSession(char * filename){
    printf("Starting session...\n");

    db = filename;

    printf("db == %s\n", db);
    printf("session started!\n");
    return 0;
}

// destroy all previous tuples
int initiateNewDB(){

    FILE * dbfile = fopen(db, "w");

    if(dbfile != 0){
        fclose(dbfile);
        return 0; // fopen success
    }else{
        // fopen failure
        printf("[initiateNewDB] open failuer");
        if(dbfile != NULL){
            fclose(dbfile);
        }
        return -1;
    }

}

/*   Inserts a given tuple into the file   */
int insertTuple(char * key, char * val1, int val2, float val3){
    //printf("Inserting tuple:\n\tkey: %s\tvalue 1: %s\tvalue 2: %d\tvalue 3: %f\n", key, val1, val2, val3);
    int is = isValue(key);

    if(is == 1){
        printf("[insertTuple] Tuple already exists\n");
        return -1;
    }else if(is == -1){
        printf("[insertTuple] Inserting tuple in new file!\n");
    }

    FILE * dbfile = fopen(db, "a+");

    // copy tuple into struct to insert into file

    char * string = malloc(TUPLE_SIZE);
    sprintf(string, "%s\t%s\t%d\t%f\n", key, val1, val2, val3); // \n
    //printf("%s", string);


    if (fwrite(string, strlen(string), 1, dbfile) <= 0){
        printf("[insertTuple] Writing error\n");
        free(string);
        fclose(dbfile);
        return -1;
    }

    free(string);
    fclose(dbfile);

    return 0;

}

/* This will delete a tuple from the database file */
int deleteTuple(char * key){
    printf("Deleting tuple with key ==  %s\n", key);


    // Open database file
    FILE * dbfile = fopen(db, "r+");
    if(dbfile == 0){
        printf("[deleteTuple] opening file error\n");
        return -1;
    }

    // Create auxiliar file
    char * db_aux = "auxFile.txt";
    FILE * auxfile = fopen(db_aux, "w+");
    if(auxfile == 0){
        printf("[deleteTuple] opening aux file error\n");
        return -1;
    }

    char k[KEY_SIZE];
    char v1[VAL_1_SIZE];
    int v2;
    float v3;
    char * string = malloc(TUPLE_SIZE);

    // Prepare for fijle reading
    char * line = NULL;
    size_t lineSize = 0;
    ssize_t nread = getline(&line, &lineSize, dbfile);

    while (nread != -1){

        //printf("\n[deleteTuple][while][LINE] == %s\n", line);
        sscanf(line, "%s\t%s\t%d\t%f\n", k, v1, &v2, &v3);
        printf("[deleteTuple][while]: %s  with length == %ld\n", k, strlen(k));

        if(line == NULL){
            //printf("[deleteTuple][while]: line is null\n");
            fclose(dbfile);
            return -1;
        }


        if(compareStrings(strlen(key), key, k) == 0){
            // this is not the tuple, so we  copy into aux file
            strcpy(string, line);
            printf("[deleteTuple][while][if compareStrings] after strcpy\n");
            if (fwrite(string, strlen(string), 1, auxfile) <= 0){
                printf("[deleteTuple][while][if compareStrings] Auxiliar file writing error\n");
                free(string);
                free(line);
                fclose(dbfile);
                fclose(auxfile);
                return -1;
            }

        }else{
            printf("[deleteTuple][while] else\n");
        }

        nread = getline(&line, &lineSize, dbfile);
        //nread = fread(line, sizeof(line), 1, dbfile);
    }
    free(line);
    free(string);

    // Go back to start of files
    rewind(dbfile);
    rewind(auxfile);

    // Now copy data from aux file into database file
    char * line2 = NULL;
    size_t lineSize2 = 0;
    nread = 0;
    nread = getline(&line2, &lineSize2, auxfile);

    while (nread != -1){

        //printf("\n[deleteTuple][while][LINE] == %s\n", line);
        sscanf(line, "%s\t%s\t%d\t%f\n", k, v1, &v2, &v3);
        printf("[deleteTuple][while 2]: %s  with length == %ld\n", k, strlen(k));

        if(line2 == NULL){
            //printf("[deleteTuple][while]: line is null\n");
            fclose(dbfile);
            return -1;
        }

        // copy from aux file into database file
        strcpy(string, line2);
        printf("[deleteTuple][while 2]: before fwrite if\n");

        if (fwrite(string, strlen(string), 1, auxfile) <= 0){
            printf("[deleteTuple] Auxiliar file writing error\n");
            free(string);
            free(line2);
            fclose(dbfile);
            fclose(auxfile);
            return -1;
        }

        nread = getline(&line, &lineSize2, auxfile);
    }





    free(string);
    free(line);
    fclose(dbfile);
    fclose(auxfile);
    remove(db_aux); //
    return 0;
}

/* Modifies the value of the tuple with said key */
int mod_tuple(char * key, char * val1, int val2, float val3){
    printf("Modifying tuple with key ==  %s to insert values:\n\tvalue 1: %s\tvalue 2: %d\tvalue 3: %f\n", key, val1, val2, val3);

    /*
    FILE * dbfile = fopen(db, "r");

    if(dbfile == 0){
      printf("[mod_tuple] opening file error\n");
      return -1;
    }

    char * string = malloc(TUPLE_SIZE);
    sprintf(string, "%s\t%s\t%d\t%f\n", key, val1, val2, val3);



    fclose(dbfile);
    return 0;
    */

}

/* Obtain values associated with the given key */
int get_tuple(char * key, char * val1, int * val2, float * val3){
    //printf("Retrieving tuple with key ==  %s with length == %ld\n", key, strlen(key));

    // Open database file
    FILE * dbfile = fopen(db, "r");
    if(dbfile == 0){
        printf("[get_tuple] opening file error\n");
        fclose(dbfile);
        return -1;
    }

    char k[KEY_SIZE];
    char v1[VAL_1_SIZE];
    int v2;
    float v3;

    // Prepare for fijle reading
    char * line = NULL;
    size_t lineSize = 0;
    ssize_t nread = getline(&line, &lineSize, dbfile);
    //size_t nread = fread(line, sizeof(line), 1, dbfile);



    while (nread != -1){ // fgets(line, TUPLE_SIZE, dbfile) != NULL

        //printf("\n[get_tuple][while][LINE] == %s\n", line);
        sscanf(line, "%s\t%s\t%d\t%f\n", k, v1, &v2, &v3);
        //printf("[get_tuple][while]: %s  with length == %ld\n", k, strlen(k));

        if(/*k*/line == NULL){
            printf("[get_tuple][while]: line is null\n");
            fclose(dbfile);
            return -1;
        }


        if(compareStrings(strlen(key), key, k)/*strcmp(k, key) == 0*/){
            // key tuple exists
            //printf("[get_tuple][while]: antes de strcpy\n");
            strcpy(val1, v1);
            *val2 = v2;
            *val3 = v3;
            //printf("[get_tuple][inside strcmp if]: %s\t%s\t%d\t%f", k, v1, v2, v3);
            fclose(dbfile);
            return 1;
        }

        nread = getline(&line, &lineSize, dbfile);
    }

    free(line);
    fclose(dbfile);
    return 0;

}



/* Returns 1 if tuple with key exists in file */
int isValue(char * key){
    // printf("Checking if tuple with key ==  %s exists\n", key);

    FILE * dbfile = fopen(db, "r");


    if(dbfile == 0){
        printf("[isValue] opening file error\n");
        fclose(dbfile);
        return -1;
    }

    char k[KEY_SIZE];
    char v1[VAL_1_SIZE];
    int v2;
    float v3;

    char line[TUPLE_SIZE];

    while (fgets(line, TUPLE_SIZE, dbfile) != NULL){
        sscanf(line, "%s\t%s\t%d\t%f", k, v1, &v2, &v3);
        if(k == NULL){
            printf("k is null\n");
            fclose(dbfile);
            return -1;
        }
        if(strcmp(k, key) == 0){
            // key tuple exists
            fclose(dbfile);
            return 1;
        }
    }
    /*
    while ((fscanf(dbfile, "%s\t%s\t%d\t%f\n", k, v1, &v2, &v3) != EOF)){
      printf("INSIDE WHILE LOOP [isValue] k = %s, key = %s\n", k, key);
      if(k == NULL){
        printf("k is null\n");
      }
      if(strcmp(k, key) == 0){
        // key tuple exists
        printf("INSIDE STRCMP IF - %s\n", k);
        fclose(dbfile);
        return 1;
      }
    }
    */
    fclose(dbfile);
    return 0;
}

// Returns the amount of tuples in database
int numTuples(){
    FILE * dbfile = fopen(db, "r");

    if(dbfile == 0){
        printf("[numTuples] opening file error\n");
        fclose(dbfile);
        return -1;
    }

    // Prepare for fijle reading
    char * line = NULL;
    size_t lineSize = 0;
    ssize_t nread = getline(&line, &lineSize, dbfile);
    int size = 0;

    while (nread != -1){ // fgets(line, TUPLE_SIZE, dbfile) != NULL
        size++;
        nread = getline(&line, &lineSize, dbfile);

    }
    fclose(dbfile);

    return size;

}

/* Display all tuples on screen
void print_list(){
}*/

/* Saves and closes data from file
int endSession(char * filename){

    printf("Session finished!\n");
}*/



/*

    // Again, this is just a commented main for testing that the database implementation works

 int main(int argc, char* argv[]) {

  char * archivo = "miArchivo.txt";
  startSession(archivo);
  initiateNewDB();

  int ll = 3;
  struct tuple list[ll];



  for(int i = 0; i < ll; i++){
    char * k = malloc(KEY_SIZE);
    sprintf(k, "clave%d", i);
    strcpy(list[i].key, k);
    free(k);

    char *v_1 = malloc(KEY_SIZE);
    sprintf(v_1, "valor%d", i);
    strcpy(list[i].value1, k);
    free(k);

    list[i].value2 = i;
    list[i].value3 = (float) i;
  }

  for(int i = 0; i < ll; i++){
    printf("\n////////////////////////////////////////////////////////\n\n");
    int t = insertTuple(list[i].key, list[i].value1, list[i].value2, list[i].value3);

    char *va1 = malloc(VAL_1_SIZE);
    int va2;
    float va3;
    //printf("\nEn el MAIN antes de get_tuple:\t %s %s, %d, %f\n", list[i].key, list[i].value1, list[i].value2, list[i].value3);
    int tu = get_tuple(list[i].key, va1, &va2, &va3);
    //printf("\nEn el MAIN despues de get_tuple: tu = %d, \t %s %s, %d, %f\n", tu, list[i].key, va1, va2, va3);
    free(va1);
  }

  for(int i = 0; i < ll; i++){
    int ex = isValue(list[i].key);
    if(ex == 1){
      printf("Tupla con key = %s existe\n", list[i].key);
    }
  }

  char * cl = "Clave0";
  int del = deleteTuple(cl);

  int ex = isValue(cl);
  if(ex == 1){
    printf("Tupla con key = asdf existe\n");
  }else{
    printf("Tupla con key = asdf NO existe\n");
  }


  return 0;
}


 */