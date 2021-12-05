#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>

void str_back(int sockfd)
{
    char *recvbuf;
    char numbuf[5]; //读取收到的字符的前4位确定后面的长度，最后一位存\0
    char *sendbuf;
    char wasteBuf[65535]; //垃圾区
    while(1) 
    { 
        bzero(numbuf, sizeof(numbuf));
        int n = read(sockfd, numbuf, 4);
        numbuf[4] = 0;
        if(n == 0)
            break;
        int length = (numbuf[0] - 48) * 1000 + (numbuf[1] - 48) * 100 + (numbuf[2] - 48) * 10 + (numbuf[3] - 48); //信息中有效字符长度（包括换行

        recvbuf = (char*)malloc(sizeof(char) * (length + 1));
        read(sockfd, recvbuf, length + 1); //没有手动加\0 可能需要更改
        read(sockfd, wasteBuf, sizeof(wasteBuf));
        sendbuf = (char*)malloc(sizeof(char) * (length + 10));
        strcpy(sendbuf + 9, recvbuf); //把真实的有效信息cpy进入sendbuf
        sendbuf[0] = (length + 5) / 1000 + 48; //多了五位有效信息 ECHO:
        sendbuf[1] = ((length + 5) / 100) % 10 + 48;
        sendbuf[2] = ((length + 5) / 10) % 10 + 48;
        sendbuf[3] = (length + 5) % 10 + 48;
        sendbuf[4] = 'E', sendbuf[5] = 'C', sendbuf[6] = 'H', sendbuf[7] = 'O', sendbuf[8] = ':';
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
    servaddr.sin_port = htons(23334);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    while(1) //循环服务器  处理好一个连接后 断开 接受另外一个连接。
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);
        printf("a client connect me!\n");
        str_back(connfd);
        close(connfd);   //处理好一个请求后关闭它
        printf("a client disconnect me!\n");
    }
    return 0;
}