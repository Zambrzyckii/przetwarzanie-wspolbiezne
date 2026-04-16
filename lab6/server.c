#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_CHATTERS 5

int server_fd;
int current_chatters = 0;
int chatters[MAX_CHATTERS];
pthread_mutex_t chatters_lock = PTHREAD_MUTEX_INITIALIZER;

void get_current_time(char* buffer, size_t size) {
    time_t t;
    struct tm* tm;
    time(&t);
    tm = localtime(&t);
    strftime(buffer, size, "%H:%M:%S", tm);
}

void handle_close(int sig) {
    char time_str[20];
    get_current_time(time_str, sizeof(time_str));
    printf("\n[%s] Closing server\n", time_str);

    pthread_mutex_lock(&chatters_lock);
    for (int i = 0; i < current_chatters; i++) {
        send(chatters[i], "Closing Server", strlen("Closing Server"), 0);
        close(chatters[i]);
    }
    pthread_mutex_unlock(&chatters_lock);

    close(server_fd);
    exit(0);
}

void handle_message(char* message, int sender_fd) {
    pthread_mutex_lock(&chatters_lock);

    for (int i = 0; i < current_chatters; i++) {
        if (chatters[i] != sender_fd) {
            send(chatters[i], message, strlen(message), 0);
        }
    }

    pthread_mutex_unlock(&chatters_lock);
}

void* handle_client(void* arg) {
    int client_fd = *(int*) arg;
    free(arg);

    pthread_mutex_lock(&chatters_lock);
    chatters[current_chatters++] = client_fd;
    pthread_mutex_unlock(&chatters_lock);

    char buffer[1024];
    char message[10240];
    char time_str[20];
    int n;

    get_current_time(time_str, sizeof(time_str));
    printf("[%s] New client: [%d]\n",time_str, client_fd);

    while ((n = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        get_current_time(time_str,sizeof(time_str));
        printf("[%s] Client [%d] message: %s",time_str, client_fd, buffer);
        snprintf(message, sizeof(message), "[%s] Client [%d]: %s",time_str, client_fd, buffer);
        handle_message(message, client_fd);
    }

    get_current_time(time_str, sizeof(time_str));
    printf("[%s] Client [%d] disconnected\n",time_str, client_fd);

    pthread_mutex_lock(&chatters_lock);
    for (int i = 0; i < current_chatters; i++) {
        if (chatters[i] == client_fd) {
            for (int j = i; j < current_chatters - 1; j++) {
                chatters[j] = chatters[j + 1];
            }
            current_chatters--;
            break;
        }
    }
    pthread_mutex_unlock(&chatters_lock);

    close(client_fd);
    return NULL;
}

int main() {
    signal(SIGINT, handle_close);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(12345);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_fd, (struct sockaddr *) &addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Serwer nasłuchuje...\n");

    for (;;) {
        int* client_fd_pointer = malloc(sizeof(int));
        *client_fd_pointer = accept(server_fd, NULL, NULL);

        if (*client_fd_pointer < 0) {
            perror("accept error");
            free(client_fd_pointer);
            continue;
        }

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, (void*)client_fd_pointer) != 0) {
            perror("pthread_create error");
            free(client_fd_pointer);
        }
        pthread_detach(client_thread);
    }

    close(server_fd);
    return 0;
}