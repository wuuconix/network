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
 
int main()
{
    int CreateSocket = 0, n = 0;
    char dataReceived[1025];
    struct sockaddr_in ipOfServer;
 
    memset(dataReceived, '0' ,sizeof(dataReceived));

    if((CreateSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0) //socket函数返回-1则表示有错误
    {
        printf("Socket not created \n");
        return 1;
    }

 
    ipOfServer.sin_family = AF_INET;
    ipOfServer.sin_port = htons(2017);
    ipOfServer.sin_addr.s_addr = inet_addr("127.0.0.1"); //将点分十进制ip字符串转化为网络序的十进制整数ip
 
    if(connect(CreateSocket, (struct sockaddr *)&ipOfServer, sizeof(ipOfServer))<0)
    {
        printf("Connection failed due to port and ip problems\n");
        return 1;
    }
 
    while((n = read(CreateSocket, dataReceived, sizeof(dataReceived)-1)) > 0) //read函数的返回值是读到的字符长度，如果为0则是EOF，-1则是报错
    {
        dataReceived[n] = 0;
        if(fputs(dataReceived, stdout) == EOF)
        {
            printf("\nStandard output error");
        }
        printf("\n");
    }
 
    if( n < 0)
    {
        printf("Standard input error \n");
    }
 
    return 0;
}