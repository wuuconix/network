#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFSIZE 1024
#define PORT 23333

void package_send(int sockfd, struct sockaddr_in serveraddr, int addrlen)
{
	char sendbuf[1];

	sendbuf[0] = 67;
	for (int i = 0; i < 1000; i++)
	{
		if (sendto(sockfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *) &serveraddr, addrlen) == -1)
		{
			printf("send package error!\n");
			return;
		}
		printf("has send package%d\n", i);
	}


}
int main(void)
{
	struct sockaddr_in serveraddr;
	int sockfd;
	int addrlen=sizeof(serveraddr);

	if ( (sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
        printf("create socket error!\n");
        return 0;
	}

    bzero(&serveraddr, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr);
	package_send(sockfd, serveraddr, addrlen);
	return 0;
}