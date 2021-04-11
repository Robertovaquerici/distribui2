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

#define MAX_CHAR_LENGTH 255

#include "keys.h"


/*
     *  op_code = 1     init()
     *  op_code = 2     set_value()
     *  op_code = 3     get_value()
     *  op_code = 4     modify_value()
     *  op_code = 5     delete_key()
     *  op_code = 6     exist()
     *  op_code = 7     num_items()
     *
     *  feedback_code: 0 == success, any other == error
     *
     * */


struct sockaddr_in server_addr; // socket to manage the connection
int sockfd; // socket file descriptor


/* Makes all necessary steps to connect to server so that the other functions can work */
int setConnectionToServer(char * ipaddr, uint16_t port){


    // Varias variables aquí tienen que ser puestas como globales

    // Check and save the IPv4 parameter and port 23690
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if(inet_pton(AF_INET, ipaddr, &server_addr.sin_addr.s_addr) == 0) { // inet_aton ????
        printf("[ERROR] The first argument should contain a valid IPv4 address\n");
        return 1;
    }

    // Initialize socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    char * op = "1";
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][init] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // success
    return 0;
}

/*
 *  Inserts tuple into the server's list.
 *  Returns 0 if successful, -1 if error
 * */
int set_value(char * key, char * value1, int value2, float value3){

    // Notify server this is a set_value operation
    char op = '2';
    if(( send(sockfd, op, strlen(op), 0)) == -1) {
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the key
    if ( send(sockfd, key, strlen(*key), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    /*// send the first value
    if ( send(sockfd, value1, strlen(*value1), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the second value
    if ( send(sockfd, htonl(value2), sizeof(uint32_t), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the third value
    if ( send(sockfd, htonl(value3), sizeof(float), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][set_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code != 0){
        return -1; // error
    }*/


    // succcess
    return 0;

}

/*
 *  Obtain the values associated to the key.
 *  Returns 0 and sets values to given parameters if successful, returns -1 if error
 * */
int get_value(char * key, char * value1, int * value2, float * value3){
    /*
    // Notify server this is a get_value operation
    char op = '3';
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][get_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
     */

    // send the key
    if ( send(sockfd, key, MAX_CHAR_LENGTH, 0) == -1 ){
        printf("[ERROR][get_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // if (recv())


    /*// receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code != 0){return -1;}

    // receive value 1
    if (recv(sockfd, value1, MAX_CHAR_LENGTH, 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive value 2
    if(recv(sockfd, htonl(value2), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive value 3
    if(recv(sockfd, htonl(value3), sizeof(float), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }*/


    // success
    return 0;
}

/*
 *  Modify the values associated to the key.
 *  Returns 0 and modifies values if successful, -1 if error
 */
int modify_value(char * key, char * value1, int value2, float value3){

    // Notify server this is a modify_value operation
    char op = '4';
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the key
    if ( send(sockfd, key, strlen(*key), 0) ){
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    /*
    // send the first value
    if ( send(sockfd, value1, strlen(*value1), 0) ){
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the second value
    if ( send(sockfd, htonl(value2), sizeof(uint32_t), 0) ){
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the third value
    if ( send(sockfd, htonl(value3), sizeof(float), 0) ){
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][modify_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code != 0){
        return -1; // error
    }*/

    // success
    return 0;
}

/*
 *  Deletes tuple from list based on its key
 *  Returns 0 and deletes values if successful, -1 if error and 1 in case tuple does not exist
 * */
int delete_key(char * key){
    // Notify server this is a delete_key operation
    char op = '5';
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][delete_key] send failed with code %s\n", strerror(errno));
        return -1;
    }

  /*
    // send the key
    if ( send(sockfd, key, strlen(*key), 0) ){
        printf("[ERROR][delete_key] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][delete_key] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code == 2){
        // error
        printf("[ERROR][delete_key] error on server side\n");
        return -1;
    }else if(feedback_code == 1){
        // key does not exist in server
        return 1;
    }*/


    // success
    return 0;
}

/*
 *  Determine if an element with key exists.
 *  Returns 0 , -1 if error
 * */
int exist(char * key){
    // Notify server this is an exist operation
    char op = '6';
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][exist] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // send the key
    if ( send(sockfd, key, strlen(key), 0) ){
        printf("[ERROR][exist] send failed with code %s\n", strerror(errno));
        return -1;
    }
/*
    // receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][exist] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code == 2){
        printf("[ERROR][exist] error on server side\n");
        return -1; // error
    }else if(feedback_code == 1){
        return 1; // tuple exists
    }*/


    // tuple does not exist
    return 0;


}

/*
 *  Check the number of items in the database (list)
 *  Returns the number of tuples in the list or -1 in cases of error
 * */

int num_items(){
   /* char op = '7';
    int bytes_sent = send(sockfd, op, strlen(op), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][num_items] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // receive feedback to check if operation was carried out successfully on server
    int * feedback_code;
    if(recv(sockfd, htonl(feedback_code), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][num_items] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    if(feedback_code == 0){
        printf("[ERROR][num_items] error on server side\n");
        return -1;
    }

    // receive feedback to check if operation was carried out successfully on server
    int * items;
    if(recv(sockfd, htonl(items), sizeof(uint32_t), 0) == -1){
        printf("[ERROR][num_items] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    return items;*/
   return 0;
}