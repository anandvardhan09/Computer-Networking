#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    char message[BUFFER_SIZE];
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    socklen_t len = sizeof(servaddr);

    while (1) {
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';

        sendto(sockfd, message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));

        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}
