int recvvl(SOCKET s, char *recvbuf, unsigned int recvbuflen)
{
    int iResult;         //存储单次recv操作的返回值
    unsigned int reclen; //用亍存储报文头部存储的长度信息
    //获叏接收报文长度信息
    iResult = recvn(s, (char *)&reclen, sizeof(unsigned int));
    if ( iResult !=sizeof ( unsigned int ) )
    {
        if (iResult == -1)
        {
            printf("接收収生错误: %d\n", WSAGetLastError());
            return -1;
        }
        else
        {
            printf("连接关闭\n");
            return 0;
        }
    }
    //转换网络字节顺序到主机字节顺序
    reclen = ntohl( reclen );
    
    if ( reclen > recvbuflen ) {
    //如果recvbuf没有足够的空间存储发长消息，则接收该消息并丢弃，返回错误
        while ( reclen > 0)
        {
            iResult = recvn(s, recvbuf, recvbuflen);
            if (iResult != recvbuflen)
            {
                //如果发长消息在接收时没有返回足够的数据就返回（连接关闭）戒-1（収生错误）
                if (iResult == -1)
                {
                    printf("接收収生错误: %d\n", WSAGetLastError());
                    return -1;
                }
                else
                {
                    printf("连接关闭\n");
                    return 0;
                }
            }
            reclen -= recvbuflen;
            //处理最后一殌数据长度
            if (reclen < recvbuflen)
                recvbuflen = reclen;
        }
        printf("可发长度的消息超出预分配的接收缓存\r\n");
        return -1;
    }
//接收可发长消息
    iResult = recvn(s, recvbuf, reclen);
    if (iResult != reclen)
    {
        /*(如果消息在接收时没有返回足够的数据就返回（连接关闭）戒-1（収
    生错误）*/
        if (iResult == -1)
        {
            printf("接收収生错误: %d\n", WSAGetLastError());
            return -1;
        }
        else
        {
            printf("连接关闭\n");
            return 0;
        }
    }
    return iResult;
}
