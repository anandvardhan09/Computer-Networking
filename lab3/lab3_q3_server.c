#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVERPORT 4952
#define MAXBUFLEN 100

void sort_array(int arr[], int n) {
    int temp;
    for(int i = 0; i < n - 1; i++) {
        for(int j = i + 1; j < n; j++) {
            if(arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int numbytes;
    int arr[MAXBUFLEN];
    int n;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Setup the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), '\0', 8);

    // Bind the socket to the server address
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        close(sockfd);
        exit(1);
    }

    printf("Server is ready\n");

    addr_len = sizeof(client_addr);

    // Receive the size of the array from the client
    if ((numbytes = recvfrom(sockfd, &n, sizeof(int), 0, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(1);
    }

    // Receive the array from the client
    if ((numbytes = recvfrom(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&client_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(1);
    }

    // Sort the array
    sort_array(arr, n);
    printf("Sorted array\n");

    // Send the sorted array back to the client
    if ((numbytes = sendto(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&client_addr, addr_len)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(1);
    }

    close(sockfd);
    return 0;
}