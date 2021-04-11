//
// Created by rober on 3/4/21.
//

#include <stdio.h>
#include <errno.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#include <pthread.h>
#include <memory.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_THREADS 10
#define THREAD_THRESH 2
#define LISTEN_PORT 23690
#define KEY_SIZE 255

#include "database.h"

typedef struct {
    // 1: init      2: set      3: get          4: modify
    // 5: delete    6: exists   7: num items
    int32_t op_code;
    char key[255];
    char val1[255];
    int32_t val2;
    float val3;
} Message;

void* worker_thread(void* param) {
    pthread_t this_t = pthread_self();
    pthread_detach(this_t);
    printf("[INFO]  Thread %lu spawned\n", this_t);

    int client_socket = *(int*) param;
    printf("Socket for client %d\n", client_socket);

    // Initialize these values before the loop
    char* key = malloc(KEY_SIZE);
    char* val1 = malloc(KEY_SIZE);
    int32_t val2, number_of_tuples;
    float val3;
    bool error = false;

    // Receive OP code
    char c;
    if(recv(client_socket, &c, sizeof(char), NULL) == -1) {
        printf("[ERROR][op_code] recv failed with code %s\n", strerror(errno));
        pthread_exit(1);
    }
    printf("OP code is %c\n", c);

    switch (c) {
        case '1':

            break;
        case '2':
            // Receive key
            if(recv(client_socket, key, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][set_value][key] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            // Receive val1
            if(recv(client_socket, val1, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][set_value][val1] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            // Receive val2
            if(recv(client_socket, &val2, sizeof(val2), 0) == -1) {
                printf("[ERROR][set_value][val2] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            val2 = ntohl(val2);
            // Receive val3
            if(recv(client_socket, &val3, sizeof(val3), 0) == -1) {
                printf("[ERROR][set_value][val3] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            break;
        case '3':
            // Receive key
            if(recv(client_socket, key, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][get_value][key] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }

            // get_tuple()
            val1 = "asd";
            // Send value
            if ( send(client_socket, val1, sizeof(char) * KEY_SIZE, 0) == -1 ){
                printf("[ERROR][get_value][value] send failed with code %s\n", strerror(errno));
                return -1;
            }

            break;
        case '4':
            // Receive key
            if(recv(client_socket, key, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][set_value][key] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            // Receive val1
            if(recv(client_socket, val1, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][set_value][val1] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            // Receive val2
            if(recv(client_socket, &val2, sizeof(val2), 0) == -1) {
                printf("[ERROR][set_value][val2] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            val2 = ntohl(val2);
            // Receive val3
            if(recv(client_socket, &val3, sizeof(val3), 0) == -1) {
                printf("[ERROR][set_value][val3] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            break;
        case '5':
            // Receive key
            if(recv(client_socket, key, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][set_value][key] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            break;
        case '6':
            // Receive key
            if(recv(client_socket, key, sizeof(char) * KEY_SIZE, 0) == -1) {
                printf("[ERROR][exists] recv failed with code %s\n", strerror(errno));
                pthread_exit(2);
            }
            if(!isValue(key)) error = true;
            break;
        case '7':
            // Send value
            number_of_tuples = 27;
            if ( send(client_socket, &number_of_tuples, sizeof(char) * KEY_SIZE, 0) == -1 ){
                printf("[ERROR][num_items] send failed with code %s\n", strerror(errno));
                return -1;
            }
            break;
    }
    char fb_code = 'k';
    if(error) fb_code = 'e';
    int bytes_sent = send(client_socket, &fb_code, sizeof(fb_code), 0);
    if(bytes_sent == -1) {
        printf("[ERROR][fb_code] send failed with code %s\n", strerror(errno));
        pthread_exit(2);
    }
    pthread_exit(0);
}

int main() {
    // Initialization
    // Socket
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        printf("[ERROR] socket creation failed with code %s\n", strerror(errno));
        return 1;
    }
    // Whatever available IP address and port 23690
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(LISTEN_PORT);
    // Bind
    int bind_err = bind(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr));
    if(bind_err < 0) {
        printf("[ERROR] bind call failed with code %s\n", strerror(errno));
        printf("[INFO]  Perhaps another instance of this program is running in the background?\n");
        return 2;
    }
    // Listen
    int listen_err = listen(sockfd, 5);
    if(listen_err < 0) {
        printf("[ERROR] listen call failed with code %s\n", strerror(errno));
        return 3;
    }
    // Connection
    while(1) {
        struct sockaddr_in client_addr;
        size_t size = sizeof(client_addr);
        int accept_socket = accept(sockfd, (struct sockaddr*) &client_addr, &size);
        if(accept_socket == -1) {
            printf("[ERROR] accept failed with code %s\n", strerror(errno));
            return 3;
        }
        printf("Socket for client: %d\n", accept_socket);
        pthread_t t = 0;
        int thread_err = pthread_create(&t, NULL, &worker_thread, (void*) &accept_socket);
        if(thread_err != 0) {
            printf("[ERROR] pthread_create failed with code %d\n", thread_err);
            return 3;
        }
    }



    return 0;
}