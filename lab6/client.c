#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void* receive_message(void* arg) {
    int sockfd = *(int*)arg;
    char buffer[1024];
    int n;
    while ((n = recv(sockfd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[n] = '\0';
        printf("Serwer: %s\n", buffer);
    }
    printf("\n Server disconnected\n");
    exit(0);
}
int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0) {
        perror("Client connection error");
        return 1;
    }
    printf("Connected\n");
    pthread_t thread;
    pthread_create(&thread, NULL, receive_message, (void*)&sockfd);
    char message[1024];
    while (fgets(message,sizeof(message),stdin) != NULL) {
        send(sockfd, message, strlen(message), 0);
    }
    close(sockfd);
    return 0;
}