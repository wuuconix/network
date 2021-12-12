#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<time.h>
#include<unistd.h>

#define PORT 23333

void package_receive(int sockfd, struct sockaddr_in clientaddr, int addrlen, int bufsize, int wait_time)
{
    char *recvbuf = (char*)malloc(sizeof(char) * bufsize); //根据终端输入的缓冲区长度动态申请大小
    int num = 0; //用来统计服务端已经接收了多少个报文了
    while(1)
    {
        usleep(wait_time); //睡眠wait_time个毫秒数，wait_time数值越高，服务器端接收速率越慢
        if ((recvfrom(sockfd, recvbuf, bufsize, 0, (struct sockaddr *) &clientaddr, &addrlen)) == -1)
        {
            printf("recv package error!\n");
            return;
        }
        printf("has receive package:%d\n", num++);
    }
}

int main(void)
{
    int bufsize, wait_time; //分别代表缓冲区大小 与每次recvfrom前等待的毫秒数
    printf("please input bufsize and wait time(microseconds): ");
    scanf("%d %d", &bufsize, &wait_time);

	struct sockaddr_in serveraddr, clientaddr;
	int sockfd;
    int addrlen = sizeof(clientaddr);
	
	if ((sockfd=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) //创建UDP套接字
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

    package_receive(sockfd, clientaddr, addrlen, bufsize, wait_time);
	return 0;
}