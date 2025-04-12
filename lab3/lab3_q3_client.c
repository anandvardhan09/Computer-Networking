#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVERPORT 4952
#define MAXBUFLEN 100

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int numbytes;
    int arr[MAXBUFLEN];
    int n;
    socklen_t addr_len;

    // Create a UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    // Setup the server address struct
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVERPORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    memset(&(server_addr.sin_zero), '\0', 8);

    addr_len = sizeof(server_addr);

    // Get the size of the array from the user
    printf("Enter the size of the array: ");
    scanf("%d", &n);

    // Get the array elements from the user
    printf("Enter the elements of the array:\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &arr[i]);
    }

    // Send the size of the array to the server
    if ((numbytes = sendto(sockfd, &n, sizeof(int), 0, (struct sockaddr *)&server_addr, addr_len)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(1);
    }

    // Send the array to the server
    if ((numbytes = sendto(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&server_addr, addr_len)) == -1) {
        perror("sendto");
        close(sockfd);
        exit(1);
    }

    // Receive the sorted array from the server
    if ((numbytes = recvfrom(sockfd, arr, n * sizeof(int), 0, (struct sockaddr *)&server_addr, &addr_len)) == -1) {
        perror("recvfrom");
        close(sockfd);
        exit(1);
    }

    // Print the sorted array
    printf("Sorted array received from the server:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    close(sockfd);
    return 0;
}