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
    //close(sockfd); // ???? /////////
    shutdown(sockfd, SHUT_RDWR);
    //return sockfd;
    return 0;
}

/* Initializes the tuple element service by destroying any already existing tuple.
 * Returns 0 on success and -1 on error
 * */
int init(int socket){

    char op = '1';
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][init] send failed with code %s\n", strerror(errno));
        return -1;
    }

    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][init][fb_code] recv failed with code %s\n", strerror(errno));
        return(-1);
    }
    if(fb_code = 'k')
        return 0;

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
    if(( send(sockfd, &op, sizeof(char), 0)) == -1) {
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the key
    if ( send(sockfd, key, MAX_CHAR_LENGTH, 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    //// ////
    // send the first value
    if ( send(sockfd, value1, MAX_CHAR_LENGTH, 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the second value
    uint32_t val2 = htonl(value2);
    if ( send(sockfd, &val2, sizeof(uint32_t), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

   // send the third value
   uint32_t val3 = htonl(value3); /// ????¿¿¿¿
    if ( send(sockfd, &val3, sizeof(uint32_t), 0) ){
        printf("[ERROR][set_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][set_value][fb_code] recv failed with code %s\n", strerror(errno));
        return(-2);
    }
    if(fb_code = 'k')
        return 0;
    //printf("Received feedback code: %c\n", fb_code);


    // succcess
    return 0;

}

/*
 *  Obtain the values associated to the key.
 *  Returns 0 and sets values to given parameters if successful, returns -1 if error
 * */
int get_value(char * key, char * value1, int * value2, float * value3){

    // Notify server this is a get_value operation
    char op = '3';
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][get_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // send the key
    if ( send(sockfd, key, MAX_CHAR_LENGTH, 0) == -1 ){
        printf("[ERROR][get_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // Receive value 1
    if(recv(sockfd, value1, MAX_CHAR_LENGTH, 0) == -1) {
        printf("[ERROR][get_value][key] recv failed with code %s\n", strerror(errno));
        return -1;
    }
    // printf("Value is %s\n", value1);


    // receive value 2
    //int val2;
    uint32_t val2;
    if(recv(sockfd, &val2, sizeof(uint32_t), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    int v = ntohl(val2); ////// val2
    value2 = &v;
    //value2 = &val2;

    // receive value 3
    //int val3 = htonl(value3);
    uint32_t val3;
    if(recv(sockfd, &val3, sizeof(float), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    float v3 = ntohl(val3);
    value3 = &v3;



    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][get_value][fb_code] recv failed with code %s\n", strerror(errno));
        return(-2);
    }
    if(fb_code = 'k')
        return 0;
    //printf("Received feedback code: %c\n", fb_code);



    // success
    return 0;
}

/*
 *  Modify the values associated to the key.
 *  Returns 0 and modifies values if successful, -1 if error
 */
int modify_value(char * key, char * value1, int value2, float value3) {

    // Notify server this is a modify_value operation
    char op = '4';
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if (bytes_sent == -1) {
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }
    // send the key
    if (send(sockfd, key, MAX_CHAR_LENGTH, 0)) {
        printf("[ERROR][modify_value] send failed with code %s\n", strerror(errno));
        return -1;
    }

    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if (recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][modify_value][fb_code] recv failed with code %s\n", strerror(errno));
        return (-1);
    }
    if (fb_code = 'e'){
        return -1;
    }
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
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][delete_key] send failed with code %s\n", strerror(errno));
        return -1;
    }

    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][delete_key][fb_code] recv failed with code %s\n", strerror(errno));
        return(-1);
    }

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
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][exist] send failed with code %s\n", strerror(errno));
        return -1;
    }

    // send the key
    if ( send(sockfd, key, strlen(key), 0) ){
        printf("[ERROR][exist] send failed with code %s\n", strerror(errno));
        return -1;
    }


    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][delete_key][fb_code] recv failed with code %s\n", strerror(errno));
        return(-1);
    }

    // tuple does not exist
    return 0;


}

/*
 *  Check the number of items in the database (list)
 *  Returns the number of tuples in the list or -1 in cases of error
 * */

int num_items(){
    int items;

    char op = '7';
    int bytes_sent = send(sockfd, &op, sizeof(char), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][num_items] send failed with code %s\n", strerror(errno));
        return -1;
    }


    // receive value 2
    uint32_t val;
    if(recv(sockfd, &val, sizeof(uint32_t), 0) == -1){
        printf("[ERROR][get_value] receive failed with code %s\n", strerror(errno));
        return -1;
    }
    items = ntohl(val); ////// val2

    char fb_code = 0;
    // Receive feedback code ('k': OK)
    if(recv(sockfd, &fb_code, sizeof(fb_code), 0) == -1) {
        printf("[ERROR][delete_key][fb_code] recv failed with code %s\n", strerror(errno));
        return(-1);
    }

    return items;
    //return 0;
}