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

int main() {
    int listenfd, connfd;
    pid_t child_pid;
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
    listen(listenfd, 5);   //设置最大连接数目

    while(1)
    {
        connfd = accept(listenfd, NULL,NULL);
        printf("a client connect me!\n");
        if((child_pid = fork()) == 0)  //fork 等于零才会进入if里面，而子进程才是 0，父进程则会返回自己真实的pid。故只有子进程才会进入其中
        {
            close(listenfd); //在子进程中关闭父进程的监听套接字 子进程的作用只是去接收一个链接而已，只需要处理连接套接字
            str_echo(connfd);
            printf("a client disconnect me!\n");
            exit(0);
        }
        close(connfd);   //父进程中关闭已连接套接字
    }

    close(listenfd);
    return 0;
}