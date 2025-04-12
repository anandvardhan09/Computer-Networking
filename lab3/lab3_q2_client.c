#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    int buffer[2];  // To hold the two integers to send
    int result;

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

    // Input the two integers
    printf("Enter first number: ");
    scanf("%d", &buffer[0]);

    printf("Enter second number: ");
    scanf("%d", &buffer[1]);

    // Send the two integers to the server
    sendto(sockfd, buffer, 2 * sizeof(int), MSG_CONFIRM, (const struct sockaddr *)&servaddr, len);

    // Receive the result (sum) from the server
    recvfrom(sockfd, &result, sizeof(int), MSG_WAITALL, (struct sockaddr *)&servaddr, &len);

    // Print the result
    printf("The sum of %d and %d is: %d\n", buffer[0], buffer[1], result);

    close(sockfd);
    return 0;
}
