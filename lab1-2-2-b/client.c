#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#define BUFSIZE 1024
#define PORT 23333

void str_cli(FILE *fp, int sockfd){
  
    char getsbuf[BUFSIZE]; // 用以存储从终端里得到的数据
    char sendbuf[BUFSIZE]; // 发送缓冲区
    char *recvbuf; // 用numbuf得到的长度来确定具体长度
    char numbuf[5]; //读取收到的字符的前4位确定后面的长度，最后一位存\0
    char wasteBuf[65535]; //垃圾区
    while(fgets(getsbuf, sizeof(getsbuf), fp) != NULL)  //循环从stdin读入 向stdin输出
    {
        getsbuf[strlen(getsbuf) - 1] = 0; //换行不算有效数据，设置为0
        sprintf(sendbuf, "%4ld", strlen(getsbuf)); //将getsbuf中有效的长度放入sendbuf前四位
        strcpy(sendbuf + 4, getsbuf);
        write(sockfd, sendbuf, sizeof(sendbuf));
        read(sockfd, numbuf, 4); //读取长度
        int length = atoi(numbuf);
        recvbuf = (char*)malloc(sizeof(char) * (length + 1)); //根据实际长度确定recvbuf大小
        read(sockfd, recvbuf, length); //这里不能用sizeof(recvbuf)，因为它是指针
        read(sockfd, wasteBuf, sizeof(wasteBuf)); //把套接字缓冲区剩余的放入垃圾区
        printf("%s\n", recvbuf);
        printf("---------------\n");
    }
}

int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error!\n");
        return 0;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT); //端口号
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); //设置服务器ip
    if (connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
    {
        printf("connect to server error!\n");
        return 0;
    }
    str_cli(stdin, sockfd);
    return 0;
}
