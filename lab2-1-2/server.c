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
    char sendbuf[BUFSIZE + 5];
    char recvbuf[BUFSIZE];
    strcpy(recvbuf, Recvbuf);
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
        recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen);
    }
    return;
}

int main()
{
	struct sockaddr_in serveraddr, clientaddr;
	int sockfd;
    int addrlen = sizeof(clientaddr);
	pid_t child_pid;

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

    int recv_len;
    char recvbuf[BUFSIZE];
    while(1)
	{
        if ((recv_len = recvfrom(sockfd, recvbuf, BUFSIZE, 0, (struct sockaddr *) &clientaddr, &addrlen)) == -1) //recvfrom的最后一个参数不能写为sizeof(clientaddr)
        {
            printf("socket recvfrom error!\n");
            return 0;
        }
        if ((child_pid = fork()) == 0)
        {
            str_echo(sockfd, clientaddr, addrlen, recvbuf, recv_len);
            exit(0); //子进程退出
        }
        bzero(recvbuf, sizeof(recvbuf));
    }
	return 0;
}