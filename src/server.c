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

void* worker_thread(void* param) {
    pthread_t this_t = pthread_self();
    pthread_detach(this_t);
    printf("[INFO]  Thread %lu spawned\n", this_t);
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
    int accept_socket, thread_err;
    struct sockaddr_in client_addr;

    pthread_t threads[MAX_THREADS] = {0};


    while(1) {
        accept_socket = accept(sockfd, (struct sockaddr*) &client_addr, sizeof(socklen_t));

        pthread_t t = 0;
        thread_err = pthread_create(&t, NULL, &worker_thread, NULL);
        if(thread_err != 0) {
            printf("[ERROR] pthread_create failed with code %d\n", thread_err);
            return 3;
        }
    }



    return 0;
}