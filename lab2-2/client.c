#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 23333

void package_send(int sockfd, struct sockaddr_in serveraddr, int addrlen, int package_num)
{
	char sendbuf[1]; //发送缓冲区设置为1，每次只发送一个字符

	sendbuf[0] = 65; //'a'的ascii码
	for (int i = 0; i < package_num; i++) //根据用户设置的发送个数循环发送报文
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
	int package_num; //代表要发送的包的数量
    printf("please input the the num of package to send: ");
    scanf("%d", &package_num);

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
	package_send(sockfd, serveraddr, addrlen, package_num);
	return 0;
}