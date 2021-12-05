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

void str_echo(int sockfd) {
    ssize_t n;
    char buf[255] = {0};
    while(1) {
        n = read(sockfd,buf,sizeof(buf));
        if(n == 0)
            break;
        write(sockfd, buf, n);
        // bzero(buf,sizeof(buf));
    }
}
int main() {
    int listenfd, connfd;
    socklen_t clilen;
    pid_t child_pid;
    struct sockaddr_in servaddr;
    struct sockaddr_in cliaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(9877);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    //设置最大连接数目
    listen(listenfd, 5);

    while(1) {
        clilen = sizeof(clilen);
        connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&clilen);   //创建子进程 进行读入和返回
        printf("connected : connfd = %d\n", connfd);
        if((child_pid = fork()) == 0) {
            close(listenfd); //在子进程中关闭父进程的监听套接字
            str_echo(connfd);
            exit(0);
        }
        close(connfd);   //父进程中关闭已连接套接字
    }

    close(listenfd);
    return 0;
}