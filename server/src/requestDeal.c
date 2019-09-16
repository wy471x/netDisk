#include "func.h"
//-------------------登录与注册------------------
void login_init(int fd){
    char curname[20] = {0};
    int curdirnum ;
    int ret;
    recv(fd,&ret,sizeof(ret),0);
label:
    // printf("%d\n",ret);
    if(ret == 1){
        deal_Login(fd,curname,&curdirnum);
    }else{
        deal_Register(fd);
        recv(fd,&ret,4,0);
        goto label;
    }
    send(fd,&curdirnum,4,0);
    char command[20] = {0};
    recv(fd,command,sizeof(command),0);
    if(strcmp(command,"gets")==0||strcmp(command,"puts") == 0){
    }else {
        //其他命令的处理
    }

}
//------文件传输-----
int send_file(int sockfd,char* filename)
{
    printf("%s\n",filename);
    printf("front send_file\n");
    train_t data;
    int fd;
    fd = open(filename,O_RDONLY);
    ERROR_CHECK(fd,-1,"open");
    struct stat buf;
    fstat(fd,&buf);
    //传输文件大小
    printf("%ld\n",buf.st_size);
    data.dataLen = sizeof(buf.st_size);
    memcpy(data.buf,&buf.st_size,data.dataLen);
    send(sockfd,&data,4+data.dataLen,0);
    int ret;
    if(buf.st_size >= 1073741824){
        struct timeval start,end;
        gettimeofday(&start,NULL);
        char *pMap = (char*)mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
        printf("%s",pMap);
        send(sockfd,pMap,buf.st_size,0);
        gettimeofday(&end,NULL);
        printf("use time = %ld\n",(end.tv_sec - start.tv_sec)*1000000+end.tv_usec - start.tv_usec);
    }else{
        while((data.dataLen = read(fd,data.buf,sizeof(data.buf))))
        {
            ret = send(sockfd,&data,4 + data.dataLen,0);
            if(-1 == ret){
                goto end;
            }
        }
        send(sockfd,&data,4 + data.dataLen,0);
    }
end:
    close(fd);
    printf("back send_file\n");
    return 0;
}
//-------文件接收------
int recv_file(int sockfd,void* pStart,int recvLen){
    printf("recv_file\n");
    char* p =(char*)pStart;
    int total = 0;
    int ret;
    while(total < recvLen)
    {
        ret = recv(sockfd,p + total,recvLen - total,0);
        total = total + ret;
    }
    return 0;
}
//------请求处理函数-----
void requestDeal(int fd){
    printf("newFd = %d\n",fd);
    char request[20] = {0};
    int dataLen;
    recv(fd,&dataLen,4,0);
    recv(fd,request,dataLen,0);
    //登录与注册
    printf("1 %s len = %ld\n",request,strlen(request));
    if(strcmp(request,"LR") == 0){
        login_init(fd);
    }
    else if(strcmp(request,"W") == 0)
    {
        printf("2 %s\n",request);
        deal_upload(fd);
    }
    else if(strcmp(request,"R") == 0)
    {
        deal_download(fd);
    }
    printf("end\n");
}
