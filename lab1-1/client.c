#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#define PORT 23333
#define BUFSIZE 1024

int main()
{
    int sockfd; //套接字 
    char buf[BUFSIZE];
    struct sockaddr_in serveraddr;
 
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //socket函数返回-1则表示有错误
    {
        printf("Socket not created \n");
        return 0;
    }
 
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(PORT);
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //将点分十进制ip字符串转化为网络序的十进制整数ip
 
    if(connect(sockfd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
    {
        printf("connect to server error!\n");
        return 0;
    }
 
    int n = read(sockfd, buf, sizeof(buf) - 1); //read函数的返回值是读到的字符长度，如果为0则是EOF，-1则是报错
    buf[n] = 0;
    printf("%s", buf);
    return 0;
}