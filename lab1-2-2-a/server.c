#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#define PORT 23333

void str_echo(int sockfd, int n_recv)
{
    char *sendbuf = (char*)malloc(sizeof(char) * (n_recv + 1)); //多的一个用来存\0
    char *recvbuf = (char*)malloc(sizeof(char) * (n_recv + 6)); //相比sendbuf多了ECHO: 5个字符
    char wastebuf[65535];
    while(1) 
    { 
        int n = read(sockfd, recvbuf, n_recv);
        if(n == 0)
            break;
        printf("%s\n", recvbuf);
        read(sockfd, wastebuf, sizeof(wastebuf)); //将sockfd缓冲区剩余的读入wastebuf丢弃
        strcpy(sendbuf, "ECHO:"); //实验题目要求加ECHO:
        strcpy(sendbuf + 5, recvbuf);
        write(sockfd, sendbuf, n_recv + 6);
        bzero(sendbuf, n_recv + 1);
        bzero(recvbuf, n_recv + 6);
    }
}
int main() 
{
    int n_recv; //每次recv的长度
    printf("please input n to decide how long to receive: ");
    scanf("%d", &n_recv);
    fgetc(stdin);

    int listenfd, connfd;
    struct sockaddr_in serveraddr;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create a listen socket error!\n");
    }

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr));
    listen(listenfd, 5);

    while(1) //循环服务器  处理好一个连接后 断开 接受另外一个连接。
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        printf("a client connect me!\n");
        str_echo(connfd, n_recv);
        close(connfd);   //父进程中关闭已连接套接字
        printf("a client disconnect me!\n");
    }
    return 0;
}