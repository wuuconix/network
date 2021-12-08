#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 23333
#define BUFSIZE 1024

void str_cli(FILE *fp, int sockfd){

    char sendbuf[BUFSIZE]; // 发送缓冲区
    char recvbuf[BUFSIZE]; // 接受缓冲区
    while(fgets(sendbuf, sizeof(sendbuf), fp) != NULL)  //循环从stdin读入 向stdin输出
    {
        write(sockfd, sendbuf, sizeof(sendbuf));
        read(sockfd, recvbuf, sizeof(recvbuf));
        printf("%s", recvbuf);
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
    }

}

int main()
{
    int sockfd;
    struct sockaddr_in serveraddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error!\n");
        return 0;
    }
    bzero(&serveraddr,sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT); //端口号
    inet_pton(AF_INET, "127.0.0.1", &serveraddr.sin_addr); //设置服务器ip
    if (connect(sockfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        printf("connect to server error!\n");
        return 0;
    }
    str_cli(stdin, sockfd);
    return 0;
}
