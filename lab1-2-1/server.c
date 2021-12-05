#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <strings.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <errno.h>

// 服务器从网络读入和返回

void str_back(int sockfd) //这里的参数即 每次的连接套接字
{
    ssize_t n;
    char buf[1025] = {0};
    while(1) {  //循环从套接字中读，保证读完整，每次读好后马上写
        n = read(sockfd, buf, sizeof(buf)); //raed函数的返回值是ssize_t
        if(n == 0)
            break;
        write(sockfd, buf, n);
        bzero(buf, sizeof(buf));
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
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    while(1) //循环服务器  处理好一个连接后 断开 接受另外一个连接。
    {
        connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);   //创建子进程 进行读入和返回
        printf("a client connect me!\n");
        str_back(connfd);
        close(connfd);   //父进程中关闭已连接套接字
    }

    return 0;
}