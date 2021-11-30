#include <stdio.h> // standard input and output library
#include <stdlib.h> // this includes functions regarding memory allocation
#include <string.h> // contains string functions
#include <errno.h> //It defines macros for reporting and retrieving error conditions through error codes
#include <time.h> //contains various functions for manipulating date and time
#include <unistd.h> //contains various constants
#include <sys/types.h> //contains a number of basic derived types that should be used whenever appropriate
#include <arpa/inet.h> // defines in_addr structure
#include <sys/socket.h> // for socket creation
#include <netinet/in.h> //contains constants and structures needed for internet domain addresses
 
int main()
{
    time_t clock;
    char dataSending[1025]; // 时间信息装在里面
    int clintListn = 0, clintConnt = 0; //分别代表服务器的监听套接字与 连接套接字 client Listen   clietn Connecting
    struct sockaddr_in ipOfServer;
    clintListn = socket(AF_INET, SOCK_STREAM, 0); // socket句柄
    memset(&ipOfServer, '0', sizeof(ipOfServer));
    memset(dataSending, '0', sizeof(dataSending));
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY); //Host to network long 将32位整数从主机字节序转到网络字节序
    ipOfServer.sin_port = htons(2017); // 指定端口 需要用网络字节序
    bind(clintListn, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));
    listen(clintListn , 20);
 
    while(1)
    {
        printf("Some Client connect me !\n"); // 有新的连接会输出这一句
        clintConnt = accept(clintListn, (struct sockaddr*)NULL, NULL);
        
        clock = time(NULL);
        snprintf(dataSending, sizeof(dataSending), "%.24s\r\n", ctime(&clock)); // 将格式化的时间字符串放入dataSending中
        write(clintConnt, dataSending, strlen(dataSending));
    
        close(clintConnt);
        sleep(1);
    }
    
    return 0;
}