#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFSIZE 1024
#define PORT 23333

void str_cli(int sockfd, struct sockaddr_in serveraddr, int addrlen)
{
	char sendbuf[BUFSIZE];
	char recvbuf[BUFSIZE];
	while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)  //循环从stdin读入 向stdin输出
	{
		if (sendbuf[0] == 'q' && sendbuf[1] == '\n')
			break;
        if (sendto(sockfd, sendbuf, sizeof(sendbuf), 0, (struct sockaddr *) &serveraddr, addrlen) == -1)
		{
			printf("socket sendto error!\n");
			return;
		}
		if (recvfrom(sockfd, recvbuf, sizeof(recvbuf), 0, (struct sockaddr *) &serveraddr, &addrlen) == -1)
		{
			printf("socket recvfrom error!\n");
			return;
		}
        printf("%s", recvbuf);
		printf("---------------\n");
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
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //设置服务器ip
	str_cli(sockfd, serveraddr, addrlen);
	return 0;
}