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

void str_echo(int sockfd)
{
    char *recvbuf;
    char numbuf[5]; //读取收到的字符的前4位确定后面的长度，最后一位存\0
    char *sendbuf;
    char wasteBuf[65535]; //垃圾区
    while(1) 
    { 
        int n = read(sockfd, numbuf, 4);
        if(n == 0)
            break;
        int length = atoi(numbuf); //将字符串转化为数字
        recvbuf =  (char*)malloc(sizeof(char) * (length + 1));
        read(sockfd, recvbuf, length);
        read(sockfd, wasteBuf, sizeof(wasteBuf));
        sendbuf = (char*)malloc(sizeof(char) * (length + 10));
        sprintf(sendbuf, "%4d", length + 5); //将整形转化为字符放入sendbuf
        strcpy(sendbuf + 4, "ECHO:");
        strcpy(sendbuf + 9, recvbuf); //把真实的有效信息cpy进入sendbuf
        write(sockfd, sendbuf, length + 10);
    }
}
int main() 
{
    int listenfd, connfd;
    struct sockaddr_in servaddr;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create a listen socket error!\n");
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    while(1) //循环服务器  处理好一个连接后 断开 接受另外一个连接。
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        printf("a client connect me!\n");
        str_echo(connfd);
        close(connfd);   //处理好一个请求后关闭它
        printf("a client disconnect me!\n");
    }
    return 0;
}