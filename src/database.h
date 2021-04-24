//
// Created by rbn on 10/4/21.
//

#ifndef DISTRIBUTED2_DATABASE_H
#define DISTRIBUTED2_DATABASE_H

/* Loads the database file into memory */
int startSession();

/*   Inserts a given tuple into the file   */
int insertTuple(char * key, char * val1, int val2, float val3);

/* This will delete a tuple from the database file */
int deleteTuple(char * key);

/* Modifies the value of the tuple with said key */
int mod_tuple(char * key, char * val1, int val2, float val3);

/* Obtain values associated with the given key */
int get_tuple(char * key, char * val1, int * val2, float * val3);

/* Returns 1 if tuple with key exists in file */
int isValue(char * key);

/* Display all tuples on screen
void print_list();*/

/* Saves and closes data from file
int endSession(char * filename);*/


#endif //DISTRIBUTED2_DATABASE_H













