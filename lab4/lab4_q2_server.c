#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client1_fd, client2_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 2) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for clients to connect...\n");

    if ((client1_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept client 1 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 1 connected.\n");

    if ((client2_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept client 2 failed");
        exit(EXIT_FAILURE);
    }
    printf("Client 2 connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        int valread = read(client1_fd, buffer, BUFFER_SIZE);
        if (valread > 0) {
            printf("Client 1: %s\n", buffer);

            send(client2_fd, buffer, strlen(buffer), 0);
            printf("Message sent to Client 2\n");
        }

        memset(buffer, 0, BUFFER_SIZE);

        valread = read(client2_fd, buffer, BUFFER_SIZE);
        if (valread > 0) {
            printf("Client 2: %s\n", buffer);

            send(client1_fd, buffer, strlen(buffer), 0);
            printf("Message sent to Client 1\n");
        }
    }

    close(client1_fd);
    close(client2_fd);
    close(server_fd);
    return 0;
}