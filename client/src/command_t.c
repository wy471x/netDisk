#include "func.h"
//-----------------------------------------客户端命令处理--------------------------------
//-----------------cd命令处理-------------------
int deal_cd(char* path,int sockfd,char** curPath,int* curdirnum)
{
    static char oldpath[MAXSIZE] = {0};
    char newpath[MAXSIZE] = {0};
    strcpy(oldpath,*curPath);
    train_t data1;
    data1.dataLen = strlen(oldpath);
    strcpy(data1.buf,oldpath);
    send(sockfd,&data1,4+data1.dataLen,0);
    printf("%s\n",oldpath);
    train_t data3;
    data3.dataLen = strlen(*curPath);
    strcpy(data3.buf,*curPath);
    send(sockfd,&data3,4+data3.dataLen,0);
    train_t data2;
    if(strcmp(path,"") == 0){
        strcpy(path," ");
        data2.dataLen = strlen(path);
        strcpy(data2.buf,path);
        send(sockfd,&data2,4 + data2.dataLen,0);
    }else{
        data2.dataLen = strlen(path);
        strcpy(data2.buf,path);
        printf("%d %s\n",data2.dataLen,data2.buf);
        send(sockfd,&data2,4+data2.dataLen,0);
    }
    int sig;
    recv(sockfd,&sig,4,0);
    int dataLen;
    if(sig == 1){
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,newpath,dataLen,0);
        recv(sockfd,&*curdirnum,4,0);
        strcpy(*curPath,newpath);
        return 0;
    }
    if(sig == 2){
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,newpath,dataLen,0);
        recv(sockfd,&*curdirnum,4,0);
        strcpy(*curPath,newpath);
        return 0;
    }
    if(sig == 3){
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,newpath,dataLen,0);
        recv(sockfd,&*curdirnum,4,0);
        strcpy(*curPath,newpath);
        return 0;
    }
    if(sig == 4){
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,newpath,dataLen,0);
        recv(sockfd,&*curdirnum,4,0);
        strcpy(*curPath,newpath);
        return 0;
    }
    if(sig == 0){
        recv(sockfd,&*curdirnum,4,0);
        recv(sockfd,&dataLen,4, 0);
        recv(sockfd,newpath,dataLen,0);
        strcpy(*curPath,newpath);
        return 0;
    }
    if(sig == -1){
        char buf[MAXSIZE] = {0};
        recv(sockfd,buf,sizeof(buf),0);
        printf("%s\n",buf);
        return -1;
    }
    return 0;
}
//---------------------ls命令处理---------------------
int deal_ls(int sockfd){
    char buf[MAXSIZE] = {0};
    recv(sockfd,buf,sizeof(buf),0);
    if(strcmp(buf,"\n") != 0) printf("%s",buf);
    return 0;
}
//----------------------------pwd命令处理-------------------
void deal_pwd(char** curPath){
    char curDir[MAXSIZE] = "/netdisk";//易产生bug处
    strcat(curDir,&(*curPath)[9]);
    curDir[strlen(curDir) - 1] = '\0';
    printf("%s\n",curDir);
}
//-------------------------remove命令处理------------------
int deal_remove(int sockfd,char* data){
    char buf[MAXSIZE] = {0};
    send(sockfd,data,strlen(data),0);
    read(sockfd,buf,sizeof(buf));
    if(buf[0] == 'N'){
        printf("%s\n",buf);
        return -1;
    }
    return 0;
}
//----------------------puts命令处理----------------------
int deal_puts(int sockfd,char* data,char* filename){
    send(sockfd,data,strlen(data),0);
    char buf[MAXSIZE] = {0};
    read(sockfd,buf,sizeof(buf));
    if(buf[0] == 'S'){
        send_file(sockfd,filename);
    }
    return 0;
}
//--------------------gets命令处理----------------------
int deal_gets(int sockfd,char* data,char* filename){
    // printf("%s %s\n",data,filename);
    send(sockfd,data,strlen(data),0);
    char buf[MAXSIZE] = {0};
    //建议使用recv和send组合来接收信息，write和read组合易造成阻塞
    recv(sockfd,buf,sizeof(buf),0);
    if(buf[0] == 'N'){
        printf("%s\n",buf);
        return -1;
    }else if(buf[0] == 'S'){
        int fd = open(filename,O_RDWR|O_CREAT|O_NONBLOCK,0666);
        int dataLen;
        off_t filesize;
        //接收下载文件的大小
        recv_file(sockfd,&dataLen,4);
        recv_file(sockfd,&filesize,dataLen);
        printf("downloaded filesize: %ld\n",filesize);
        ftruncate(fd,filesize);
        //接收文件内容
        char* pMap = (char*)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        ERROR_CHECK(pMap,(char*)-1,"mmap");
        recv_file(sockfd,pMap,filesize);
        int ret = munmap(pMap,filesize);
        printf("100%%,Success download\n");
        ERROR_CHECK(ret,-1,"munmap");
        close(fd);
        return 0;   
    }
    return 0;
}
