//
// Created by rober on 3/4/21.
//

// I/O and error dependencies

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

// TCP/IP dependencies
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>

// Include our custom-made library to connect to the server
#include "keys.h"

#define SERVER_PORT 23690
#define CHAR_LENGTH 255




void scase2();
void scase3();
void scase4();
void scase5();
void scase6();
void scase7();





int main(int argc, char* argv[]) {


   // argv[0]=clientBinary argv[1]=IPv4 address
    if(argc != 2) {
        printf("[ERROR] Incorrect argument count\n");
        printf("Usage: ./clientBinary IPv4_address\n");
        return 1;
    }

    if(setConnectionToServer(argv[1], SERVER_PORT) != 0){
        printf("[ERROR] connection set not working\n");
    }

    

    int option;
    do{

        printf("Choose an option below and press Enter key\n");
        // 1: init      2: set      3: get          4: modify
        // 5: delete    6: exists   7: num items
        printf("[1] Initialize database     [2] Insert key-value pair\n");
        printf("[3] Get value from key      [4] Modify a value from key\n");
        printf("[5] Delete key-value pair   [6] Check whether a key exists\n");
        printf("[7] Return number of pairs in the database\n");
        scanf("%d", &option);

    }
    while(option <= 0 || 8 <= option);

    switch (option) {
        case 1:
            init();
            break;
        case 2: // set_value
            scase2();
            break;
        case 3: // get_value
            {


            char *key = malloc(CHAR_LENGTH);
            char *val1 = malloc(CHAR_LENGTH);
            int val2;
            float val3;
            printf("Insert key to retreive values:\n");
            scanf("%s", key);

            int err = get_value(key, val1, &val2, &val3);
            if (err) {
                printf("get_value didn't work");
            }

            free(key);
            free(val1);
            }
            break;
        case 4: // modify_value
            scase4();
            break;
        case 5: // delete_key
            scase5();
            break;
        case 6: // exists
            scase6();
            break;
        case 7: // num items
            // NUM ITEMS
            printf("Num items: %d", num_items());
            break;

        default:
            printf("Wrong code. Try again!\n");


    }

        





    /*
    // Check and save the IPv4 parameter and port 23690
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr.s_addr) == 0) {
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
    // Send nud- data
    char* s = "Server, can you read this?\n";
    int bytes_sent = send(sockfd, s, strlen(s), 0);
    if(bytes_sent == -1) {
        printf("[ERROR] send failed with code %s\n", strerror(errno));
        return 3;
    }*/



}


void scase2(){
    // SET VALUE
    char *key = malloc(CHAR_LENGTH);
    char *val1 = malloc(CHAR_LENGTH);
    int val2;
    float val3;
    // get data from user
    printf("Insert key:\n");
    scanf("%s", key);
    printf("Insert val1:\n");
    scanf("%s", val1);
    printf("Insert val2:\n");
    scanf("%d", &val2);
    printf("Insert val3:\n");
    scanf("%f", &val3);

    int err = set_value(key, val1, val2, val3);
    if (err) {
        printf("set_value didn't work");
    }

    free(key);
    free(val1);
}

/*void scase3(){

}*/


void scase4(){
    // MODIFY VALUE
    char *key = malloc(CHAR_LENGTH);
    char *val1 = malloc(CHAR_LENGTH);
    int val2;
    float val3;
    // get data from user
    printf("Insert key:\n");
    scanf("%s", key);
    printf("Insert val1:\n");
    scanf("%s", val1);
    printf("Insert val2:\n");
    scanf("%d", &val2);
    printf("Insert val3:\n");
    scanf("%f", &val3);

    if(modify_value(key, val1, val2, val3)){
        printf("modify_value didn't work");
    }

    free(key);
    free(val1);

}

void scase5(){
    // DELETE TUPLE
    char *key = malloc(CHAR_LENGTH);

    printf("Insert key to delete tuple:\n");
    scanf("%s", key);

    int err = delete_key(key);
    if (err) {
        printf("delete_key didn't work");
    }

    free(key);
}

void scase6(){
    // EXISTS
    char *key = malloc(CHAR_LENGTH);

    printf("Insert key to delete tuple:\n");
    scanf("%s", key);

    int err = delete_key(key);
    if (err == -1) {
        printf("exists didn't work");
    }else if(err == 1){
        printf("key does not exist\n");
    }
    printf("Key does exist\n");
    free(key);
}








