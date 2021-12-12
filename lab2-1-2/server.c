#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<unistd.h>

#define BUFSIZE 1024
#define PORT 23333

    
void str_echo(int sockfd, struct sockaddr_in clientaddr, int addrlen, char Recvbuf[BUFSIZE], int recv_len)
{
    char sendbuf[BUFSIZE + 5]; //发送缓冲区
    char recvbuf[BUFSIZE]; //接收缓冲区
    strcpy(recvbuf, Recvbuf); //一开始主进程首先会有一个接收，需要先进行回应
    while(recv_len > 0) //当recv_len 小于等于 0 时表示通信结束或者有错误
    {
        printf("%s" , recvbuf);
        strcpy(sendbuf, "echo:");
        strcpy(sendbuf + 5, recvbuf);

        if (sendto(sockfd, sendbuf, recv_len + 5, 0, (struct sockaddr*) &clientaddr, sizeof(clientaddr)) == -1)
        {
            printf("socket sendto error!\n");
            return;
        }
        bzero(sendbuf, sizeof(sendbuf));
        bzero(sendbuf, sizeof(recvbuf));
        recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen); //再次对同一个客户端进行接收
    }
    return;
}

int main()
{
	struct sockaddr_in serveraddr, clientaddr;
	int sockfd;
    int addrlen = sizeof(clientaddr);
	pid_t child_pid; //保存pid

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

    int recv_len; //保存接收到的长度
    char recvbuf[BUFSIZE];
    while(1)
	{
        if ((recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen)) == -1) //主进程负责接收各个客户端
        {
            printf("socket recvfrom error!\n");
            return 0;
        }
        if ((child_pid = fork()) == 0) //fork函数 主进程会返回进程的pid，而子进程会返回零，故子进程会进入if里面
        {
            str_echo(sockfd, clientaddr, addrlen, recvbuf, recv_len);
            exit(0); //子进程退出
        }
        bzero(recvbuf, sizeof(recvbuf));
    }
	return 0;
}