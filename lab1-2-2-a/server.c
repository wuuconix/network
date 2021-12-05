#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#define BUFSIZE 1024

void str_back(int sockfd, int n_recv)
{
    char recvbuf[BUFSIZE];
    char sendbuf[BUFSIZE];
    while(1) 
    { 
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));

        int n = read(sockfd, recvbuf, sizeof(recvbuf));
        if(n == 0)
            break;
        sendbuf[0] = 'E', sendbuf[1] = 'C', sendbuf[2] = 'H', sendbuf[3] = 'O', sendbuf[4] = ':'; //实验题目要求加ECHO:
        char c = recvbuf[0];
        int index = 0;
        while ((int)c != 0 && index < n_recv) //将recvbuf的值搬到sendbuf中
        {
            sendbuf[index + 5] = recvbuf[index];
            index++;
            c = recvbuf[index];
        }
        if ((int)sendbuf[index + 5 - 1] != 10) //若最后没有换行（输入的字符长度大于n_recv的情况手动添加换行
            sendbuf[index + 5] = 10;
        
        write(sockfd, sendbuf, sizeof(sendbuf));
    }
}
int main() 
{
    int n_recv = 5; //每次recv的长度
    printf("please input n to decide how long to receive: ");
    scanf("%d", &n_recv);
    fgetc(stdin);

    int listenfd, connfd;
    struct sockaddr_in servaddr;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create a listen socket error!\n");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(23334);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    while(1) //循环服务器  处理好一个连接后 断开 接受另外一个连接。
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);   //创建子进程 进行读入和返回
        printf("a client connect me!\n");
        str_back(connfd, n_recv);
        close(connfd);   //父进程中关闭已连接套接字
        printf("a client disconnect me!\n");
    }
    return 0;
}