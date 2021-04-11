//
// Created by rober on 3/4/21.
//

/*// I/O and error dependencies
#include <stdio.h>
#include <errno.h>

// TCP/IP dependencies
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory.h>*/

// Include our custom-made library to connect to the server
#include "keys.h"

//#define SERVER_PORT 23690

int main(int argc, char* argv[]) {


   // argv[0]=clientBinary argv[1]=IPv4 address
    if(argc != 2) {
        printf("[ERROR] Incorrect argument count\n");
        printf("Usage: ./clientBinary IPv4_address\n");
        return 1;
    }

    if(!setConnectionToServer(argv[1])){
        printf("Connection setting error!");
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

