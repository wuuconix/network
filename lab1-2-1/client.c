#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>

// 客户端从标准输入输入字符串和从标准输出输出字符串
void str_cli(FILE *fp, int sockfd){

    char sendbuf[255] = {0}; // 发送buf
    char recvbuf[255] = {0}; // 接受的buf
    // 循环输入和输出
    while(fgets(sendbuf, sizeof(sendbuf), fp) != NULL)
    {
        write(sockfd,sendbuf, sizeof(sendbuf));
        read(sockfd,recvbuf, sizeof(recvbuf));
        printf("%s", recvbuf);
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
    }

}
// 客户端的主程序
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    //指定通信协议理性
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);//端口号
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);//设置服务器ip
    connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr));//进行tcp连接
    str_cli(stdin, sockfd);
    exit(0);

}
