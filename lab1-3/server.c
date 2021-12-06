#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

#define PORT 23333
#define BUFSIZE 1024

void str_add(int sockfd) //这里的参数即 每次的连接套接字
{
    char recvbuf[BUFSIZE];
    char sendbuf[BUFSIZE];
    while(1)
    { 
        int n = read(sockfd, recvbuf, sizeof(recvbuf));
        if(n == 0) //对方断开了链接
            break;
        int left = 0, right = 0; //左加数、右加数
        int index = 0;
        while(1) //通过空格 区分出左加数
        {
            char c = recvbuf[index];
            if(c == '+')
                break;
            left = left * 10 + (c - 48);
            index++;
        }
        index++; //跳过空格
        while(1) //通过换行 区分出左加数
        {
            char c = recvbuf[index];
            if(c == '\n')
                break;
            right = right * 10 + (c - 48);
            index++;
        }
        int sum = left + right;
        sprintf(sendbuf, "%d\n", sum); //把sum输入到sendbuf中
        write(sockfd, sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
        bzero(sendbuf, sizeof(sendbuf));
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
        str_add(connfd);
        printf("a client disconnect me!\n");
        close(connfd);   //父进程中关闭已连接套接字
    }
    return 0;
}