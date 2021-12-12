#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFSIZE 1024
#define PORT 23333

void str_cli(int sockfd, struct sockaddr_in serveraddr, int addrlen)
{
	char sendbuf[BUFSIZE]; //接收缓冲区
	char recvbuf[BUFSIZE]; //发送缓冲区
	while(fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)  //循环从stdin读取数据
	{
		if (sendbuf[0] == 'q' && sendbuf[1] == '\n') //如果输入是q则退出
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
        printf("%s", recvbuf); //输出接收到的数据
		printf("---------------\n");
        bzero(sendbuf, sizeof(sendbuf)); //置零，防止对后面产生影响
        bzero(recvbuf, sizeof(recvbuf));
    }
}
int main(void)
{
	struct sockaddr_in serveraddr;
	int sockfd; //udp套接字句柄
	int addrlen=sizeof(serveraddr); //sendto和recvfrom都需要这个长度

	if ( (sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
        printf("create socket error!\n");
        return 0;
	}

    bzero(&serveraddr, 0);
	serveraddr.sin_family = AF_INET; //设置服务端地址
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	str_cli(sockfd, serveraddr, addrlen);
	return 0;
}