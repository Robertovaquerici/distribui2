//
// Created by rbn on 10/4/21.
//

// I/O and error dependencies
#include <stdio.h>
#include <errno.h>

// TCP/IP dependencies
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

#define SERVER_PORT 23690

struct sockaddr_in server_addr; // socket to manage the connection


/* Makes all necessary steps to connect to server so that the other functions can work */
int setConnectionToServer(char * ipaddr){


    // Varias variables aquí tienen que ser puestas como globales

    // Check and save the IPv4 parameter and port 23690
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, ipaddr, &server_addr.sin_addr.s_addr) == 0) { // inet_aton ????
        printf("[ERROR] The first argument should contain a valid IPv4 address\n");
        return 1;
    }

    // Initialize socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        printf("[ERROR] socket creation failed with code %s\n", strerror(errno));
        return 2;
    }
    // Connection
    if(connect(sockfd, (const struct sockaddr *)& server_addr, sizeof(server_addr)) < 0) {
        printf("[ERROR] connect failed with code %s\n", strerror(errno));
        return 2;
    }
    /*
    // Send nud- data
    char* s = "Server, can you read this?\n";
    int bytes_sent = send(sockfd, s, strlen(s), 0);
    if(bytes_sent == -1) {
        printf("[ERROR] send failed with code %s\n", strerror(errno));
        return 3;
    }
     */

    // connection successful
    return 0;
}


/* Once the service is no longer necessary, this function ends the connection from client to server */
int endConnectionToServer(){

}

/* Initializes the tuple element service by destroying any already existing tuple.
 * Returns 0 on success and -1 on error
 * */
int init(int socket){

}

/*
 *  Inserts tuple into the server's list.
 *  Returns 0 if successful, -1 if error
 * */
int set_value(char * key, char * value1, int value2, float value3){

}

/*
 *  Obtain the values associated to the key.
 *  Returns 0 and sets values to given parameters if successful, returns -1 if error
 * */
int get_value(char * key, char * value1, int * value2, float * value3){

}

/*
 *  Modify the values associated to the key.
 *  Returns 0 and modifies values if successful, -1 if error
 */
int modify_value(char * key, char * value1, int value2, float value3){

}

/*
 *  Deletes tuple from list based on its key
 *  Returns 0 and deletes values if successful, -1 if error and 1 in case tuple does not exist
 * */
int delete_key(char * key){

}

/*
 *  Determine if an element with key exists.
 *  Returns 0 , -1 if error
 * */
int exist(char * key){

}

/*
 *  Check the number of items in the database (list)
 *  Returns the number of tuples in the list or -1 in cases of error
 * */
int num_items(int socket){

}