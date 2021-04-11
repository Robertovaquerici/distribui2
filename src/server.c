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

#define MAX_THREADS 10
#define THREAD_THRESH 2
#define LISTEN_PORT 23690
#define KEY_SIZE 255

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
    // Receive OP code
    char c;
    if(recv(client_socket, &c, sizeof(char), NULL) == -1) {
        printf("[ERROR][op_code] recv failed with code %s\n", strerror(errno));
        return 3;
    }
    char* key[KEY_SIZE], val1[KEY_SIZE];
    int32_t val2;
    float val3;
    switch (c) {
        case '1':

            break;
        case '2':

            break;
        case '3':
            // Receive key
            if(recv(client_socket, &key, sizeof(KEY_SIZE), NULL) == -1) {
                printf("[ERROR][get_value][key] recv failed with code %s\n", strerror(errno));
                return 3;
            }
            // Receive val1
            if(recv(client_socket, &val1, sizeof(KEY_SIZE), NULL) == -1) {
                printf("[ERROR][get_value][val1] recv failed with code %s\n", strerror(errno));
                return 3;
            }
            // Receive val2
            if(recv(client_socket, &val2, sizeof(KEY_SIZE), NULL) == -1) {
                printf("[ERROR][get_value][val2] recv failed with code %s\n", strerror(errno));
                return 3;
            }
            // Receive val3
            if(recv(client_socket, &val3, sizeof(KEY_SIZE), NULL) == -1) {
                printf("[ERROR][get_value][val3] recv failed with code %s\n", strerror(errno));
                return 3;
            }
            val2 = ntohl(val2);
            printf("%s, %s, %d, %f\n", key, val1, val2, val3);
            uint32_t fb_code = 0;
            int bytes_sent = send(client_socket, fb_code, strlen(fb_code), 0);
            if(bytes_sent == -1) {
                printf("[ERROR][get_value][fb_code] send failed with code %s\n", strerror(errno));
                return -1;
            }
            break;
        case '4':

            break;
        case '5':

            break;
        case '6':

            break;
        case '7':

            break;
    }



    char* s = "Server, can you read this?\n";
    int bytes_sent = send(client_socket, s, strlen(s), 0);
    if(bytes_sent == -1) {
        printf("[ERROR] send failed with code %s\n", strerror(errno));
        return 3;
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
        int accept_socket = accept(sockfd, (struct sockaddr*) &client_addr, sizeof(socklen_t));

        pthread_t t = 0;
        int thread_err = pthread_create(&t, NULL, &worker_thread, (void*) &accept_socket);
        if(thread_err != 0) {
            printf("[ERROR] pthread_create failed with code %d\n", thread_err);
            return 3;
        }
    }



    return 0;
}