//
// Created by rbn on 11/4/21.
//
#include <stdio.h>
#include <stdlib.h>

#define LIST_LENGTH 10
#define KEY_SIZE 255
#define VAL_1_SIZE 255

char ** keys;
char ** val1;
int * val2;
float * val3;
int list_index;


/* Loads the database file into memory */
int startSession(){
    printf("Starting session...");
    keys = malloc(KEY_SIZE * LIST_LENGTH);
    val1 = malloc((VAL_1_SIZE * LIST_LENGTH));
    val2 = malloc (LIST_LENGTH * sizeof(int));
    val3 = malloc((LIST_LENGTH * sizeof(float)));
    list_index = 0;

    if(keys == NULL || val1 == NULL || val2 == NULL || val3 == NULL){
        printf("Error starting session: malloc null return\n");
        return -1;
    }
    printf("session started!\n");
    return 0;
}

/*   Inserts a given tuple into the file   */
int insertTuple(char * key, char * val1, int val2, float val3){

}

/* This will delete a tuple from the database file */
int deleteTuple(char * key){

}

/* Modifies the value of the tuple with said key */
int mod_tuple(char * key, char * val1, int val2, float val3){

}

/* Obtain values associated with the given key */
int get_tuple(char * key, char * val1, int val2, float val3){

}

/* Returns 1 if tuple with key exists in file */
int isValue(char * key){

}

/* Saves and closes data from file */
int endSession(){
    free(keys);
    free(val1);
    free(val2);
    free(val3);
    printf("Session finished!\n");
}
