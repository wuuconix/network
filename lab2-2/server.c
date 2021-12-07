#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<unistd.h>

#define BUFSIZE 10240
#define PORT 23333

void package_receive(int sockfd, struct sockaddr_in clientaddr, int addrlen)
{
    char recvbuf[BUFSIZE];
    int recv_len;
    int num = 0;
    while(1)
    {
        usleep(100);
        if ((recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen)) == -1) //recvfrom的最后一个参数不能写为sizeof(clientaddr)
        {
            printf("recv package error!\n");
            return;
        }
        printf("has receive package:%d\n", num++);
    }
}

int main(void)
{
	struct sockaddr_in serveraddr, clientaddr;
	int sockfd;
    int addrlen = sizeof(clientaddr);
	
	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
        printf("create socket error!\n");
        return 0;
	}
	
    bzero(&serveraddr, 0);
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if( bind(sockfd , (struct sockaddr*)&serveraddr, sizeof(serveraddr) ) == -1)
	{
        printf("socket bind error!\n");
        return 0;
	}

    package_receive(sockfd, clientaddr, addrlen);
	return 0;
}