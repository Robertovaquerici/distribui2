//
// Created by rober on 6/4/21.
//

#ifndef DISTRIBUTED2_KEYS_H
#define DISTRIBUTED2_KEYS_H

#endif //DISTRIBUTED2_KEYS_H

#include "keys.c"



/* Makes all necessary steps to connect to server so that the other functions can work */
int setConnectionToServer(char * ipaddr);

/* Once the service is no longer necessary, this function ends the connection from client to server */
int endConnectionToServer();


/* Initializes the tuple element service by destroying any already existing tuple.
 * Returns 0 on success and -1 on error
 * */
int init();

/*
 *  Inserts tuple into the server's list.
 *  Returns 0 if successful, -1 if error
 * */
int set_value(char * key, char * value1, int value2, float value3);

/*
 *  Obtain the values associated to the key.
 *  Returns 0 and sets values to given parameters if successful, returns -1 if error
 * */
int get_value(char * key, char * value1, int * value2, float * value3);

/*
 *  Modify the values associated to the key.
 *  Returns 0 and modifies values if successful, -1 if error
 */
int modify_value(char * key, char * value1, int value2, float value3);

/*
 *  Deletes tuple from list based on its key
 *  Returns 0 and deletes values if successful, -1 if error and 1 in case tuple does not exist
 * */
int delete_key(char * key);

/*
 *  Determine if an element with key exists.
 *  Returns 0 , -1 if error
 * */
int exist(char * key);

/*
 *  Check the number of items in the database (list)
 *  Returns the number of tuples in the list or -1 in cases of error
 * */
int num_items(int socket);