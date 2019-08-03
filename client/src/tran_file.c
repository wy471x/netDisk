#include "func.h"
//------文件传输-----
int send_file(int sockfd,char* filename)
{
    train_t data;
    int fd = open(filename,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    struct stat buf;
    fstat(fd,&buf);
    data.dataLen = sizeof(buf.st_size);
    memcpy(data.buf,&buf.st_size,data.dataLen);
    send(sockfd,&data,4+data.dataLen,0);
    struct timeval start,end;
    gettimeofday(&start,NULL);
    char *pMap = (char*)mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
    send(sockfd,pMap,buf.st_size,0);
    gettimeofday(&end,NULL);
    printf("use time = %ld\n",(end.tv_sec - start.tv_sec)*1000000+end.tv_usec - start.tv_usec);
    printf("100%%,Success upload\n");
    close(fd);
    return 0;
}
int recv_file(int sockfd,void* pStart,int recvLen){
    char* p =(char*)pStart;
    int total = 0;
    int ret;
    while(total < recvLen){
        ret = recv(sockfd,p + total,recvLen - total,0);
        total = total + ret;
    }
    return 0;
}
