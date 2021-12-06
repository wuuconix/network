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

void str_cli(FILE *fp, int sockfd, int n_recv){

    char sendbuf[BUFSIZE]; // 发送缓冲区
    char *recvbuf = (char*)malloc(sizeof(char) * (n_recv + 1));
    char wastebuf[65535];
    while(fgets(sendbuf, sizeof(sendbuf), fp) != NULL)  //循环从stdin读入 向stdin输出
    {
        sendbuf[strlen(sendbuf) - 1] = 0; //不发送换行, 因为换行不算有效数据
        write(sockfd, sendbuf, sizeof(sendbuf));
        read(sockfd, recvbuf, n_recv); //只读有效的，\0不读，recvbuf初始都是0
        read(sockfd, wastebuf, sizeof(wastebuf)); //将sockfd缓冲区剩余的读入wastebuf丢弃
        printf("%s\n", recvbuf);
        printf("--------------\n");
        bzero(sendbuf, sizeof(sendbuf));
        bzero(recvbuf, n_recv + 1);
    }

}

int main()
{
    int n_recv; //每次recv的长度
    printf("please input n to decide how long to receive: ");
    scanf("%d", &n_recv);
    fgetc(stdin);

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
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1"); //设置服务器ip
    if (connect(sockfd, (struct sockaddr*)&serveraddr,sizeof(serveraddr)) < 0)
    {
        printf("connect to server error!\n");
        return 0;
    }
    str_cli(stdin, sockfd, n_recv);
    return 0;
}
