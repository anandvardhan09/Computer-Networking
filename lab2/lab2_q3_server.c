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
    struct sockaddr_in servaddr, client1addr, client2addr;
    socklen_t len = sizeof(struct sockaddr_in);
    int client1_received = 0;
    int client2_received = 0;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&client1addr, 0, sizeof(client1addr));
    memset(&client2addr, 0, sizeof(client2addr));

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

    while (1) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&client1addr, &len);
        buffer[n] = '\0';
        
        if (!client1_received) {
            client1addr = client1addr;
            client1_received = 1;
            printf("Client 1 connected: %s\n", buffer);
        } else if (!client2_received && (client1addr.sin_addr.s_addr != client1addr.sin_addr.s_addr || client1addr.sin_port != client1addr.sin_port)) {
            client2addr = client1addr;
            client2_received = 1;
            printf("Client 2 connected: %s\n", buffer);
        }

        if (client1_received && client2_received) {
            // Forward messages between clients
            if (client1addr.sin_addr.s_addr == client1addr.sin_addr.s_addr && client1addr.sin_port == client1addr.sin_port) {
                sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&client2addr, len);
                printf("Message from Client 1 to Client 2: %s\n", buffer);
            } else {
                sendto(sockfd, buffer, strlen(buffer), MSG_CONFIRM, (const struct sockaddr *)&client1addr, len);
                printf("Message from Client 2 to Client 1: %s\n", buffer);
            }
        }
    }

    close(sockfd);
    return 0;
}
