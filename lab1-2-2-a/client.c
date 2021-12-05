#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#define BUFSIZE 1024

void str_cli(FILE *fp, int sockfd, int n_recv){

    char sendbuf[BUFSIZE]; // 发送缓冲区
    char recvbuf[BUFSIZE]; // 接受缓冲区
    char printbuf[BUFSIZ]; // 输出缓冲区
    while(fgets(sendbuf, sizeof(sendbuf), fp) != NULL)  //循环从stdin读入 向stdin输出
    {
        write(sockfd, sendbuf, sizeof(sendbuf));
        read(sockfd, recvbuf, sizeof(recvbuf)); //接收字符不能在read第三个参数直接改，否则缓冲区实际长度和第三个参数不匹配会发生奇怪现象
        char c = recvbuf[0];
        int index = 0;
        while ((int)c != 0 && index < n_recv) //将recvbuf的值搬到printbuf中
        {
            printbuf[index] = recvbuf[index];
            index++;
            c = recvbuf[index];
        }
        if ((int)printbuf[index - 1] != 10) //若最后没有换行
        {
            printbuf[index] = 10;
            printbuf[index + 1] = 0;
        }
        else
            printbuf[index] = 0;
        printf("%s", printbuf);
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, sizeof(recvbuf));
        bzero(printbuf, sizeof(printbuf));
    }

}

int main()
{
    int n_recv = 5; //每次recv的长度
    printf("please input n to decide how long to receive: ");
    scanf("%d", &n_recv);
    fgetc(stdin);

    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("create socket error!\n");
        return 0;
    }
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(23333); //端口号
    inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr); //设置服务器ip
    if (connect(sockfd, (struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
    {
        printf("connect to server error!\n");
        return 0;
    }
    str_cli(stdin, sockfd, n_recv);
    return 0;
}
