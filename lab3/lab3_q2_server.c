#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len;
    int buffer[2];  // To hold the two integers
    int result;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server running...\n");

    len = sizeof(cliaddr);

    while (1) {
        // Receive the two integers from the client
        recvfrom(sockfd, buffer, 2 * sizeof(int), MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        
        // Add the two integers
        result = buffer[0] + buffer[1];
        
        printf("Received numbers: %d, %d\n", buffer[0], buffer[1]);
        printf("Sending back the sum: %d\n", result);

        // Send the result back to the client
        sendto(sockfd, &result, sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&cliaddr, len);
    }

    close(sockfd);
    return 0;
}
