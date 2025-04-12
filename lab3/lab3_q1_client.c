/*
** A datagram "client" demo
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SERVERPORT 4952 // the port users will be connecting to
#define MAXBUFLEN 200
int main()
{
int sockfd;
struct sockaddr_in their_addr; // connector's address information
//struct hostent *he;
socklen_t addr_len;
int numbytes;
char buf[MAXBUFLEN];
char arg[30];

if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
perror("socket");
exit(1);
}

their_addr.sin_family = AF_INET; // host byte order
their_addr.sin_port = htons(SERVERPORT); // short, network byte order
their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//memset(their_addr.sin_zero, '\0', sizeof their_addr.sin_zero);

while(1){
	printf("Client:");
	gets(arg);
	
	if ((numbytes = sendto(sockfd, arg, strlen(arg), 0,
	(struct sockaddr *)&their_addr, sizeof their_addr)) == -1) {
	perror("sendto");
	exit(1);}
	
	if ((numbytes = recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
	(struct sockaddr *)&their_addr, &addr_len)) == -1) {
	perror("recvfrom");
	exit(1);}
	
	printf("Server:\"%s\"\n",buf);
	
	int i = strncmp("bye", buf, 3);
	if(i == 0)
	break;

}
close(sockfd);
return 0;
}









	