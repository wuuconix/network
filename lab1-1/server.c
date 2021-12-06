#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <errno.h>
#include <time.h>
#include <unistd.h> 
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#define PORT 23333
#define BUFSIZE 1024

int main()
{
    time_t clock;
    char buf[BUFSIZE]; // 时间信息装在里面
    int listenfd, connfd; //分别代表服务器的监听套接字与 连接套接字
    struct sockaddr_in serveraddr;

    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) //socket函数返回-1则表示有错误
    {
        printf("Socket not created \n");
        return 0;
    }

    bzero(&serveraddr, sizeof(serveraddr));
    bzero(buf, sizeof(buf));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); //Host to network long 将32位整数从主机字节序转到网络字节序
    serveraddr.sin_port = htons(PORT); // 指定端口 需要用网络字节序
    bind(listenfd, (struct sockaddr*)&serveraddr , sizeof(serveraddr));
    listen(listenfd , 5);
 
    while(1)
    {
        connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
        printf("Some Client connect me !\n"); // 有新的连接会输出这一句
        
        clock = time(NULL);
        snprintf(buf, sizeof(buf), "%.24s\n", ctime(&clock)); // 将格式化的时间字符串放入buf中
        write(connfd, buf, strlen(buf));
    
        close(connfd);
        sleep(1);
    }
    
    return 0;
}