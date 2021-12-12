#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUFSIZE 1024
#define PORT 23333

void str_echo(int sockfd, struct sockaddr_in clientaddr, int addrlen)
{
    char recvbuf[BUFSIZE]; //接收缓冲区
    char sendbuf[BUFSIZE + 5];  //发送缓冲区
    int recv_len; //用以存储recvfrom的有效字符个数
    if ((recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen)) == -1) ///接收数据
    {
        printf("socket recvfrom error!\n");
        return;
    }
    
    printf("%s" , recvbuf);
    strcpy(sendbuf, "echo:");
    strcpy(sendbuf + 5, recvbuf);

    if (sendto(sockfd, sendbuf, recv_len + 5, 0, (struct sockaddr*) &clientaddr, sizeof(clientaddr)) == -1) //发送数据
    {
        printf("socket sendto error!\n");
        return;
    }
}

int main(void)
{
	struct sockaddr_in serveraddr, clientaddr;
	int sockfd; //udp套接字句柄
    int addrlen = sizeof(clientaddr); //sendto和recvfrom都需要这个长度
	
	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) //创建数据包套接字
	{
        printf("create socket error!\n");
        return 0;
	}
	
    bzero(&serveraddr, 0);
	serveraddr.sin_family = AF_INET; //设置serveraddr
	serveraddr.sin_port = htons(PORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	if( bind(sockfd , (struct sockaddr*)&serveraddr, sizeof(serveraddr) ) == -1) //将udp套接字与serveraddr绑定
	{
        printf("socket bind error!\n");
        return 0;
	}

    while(1)
	{
        str_echo(sockfd, clientaddr, addrlen);
    }
	return 0;
}