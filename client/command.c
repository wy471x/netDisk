#include "func.h"
char newpath[MAXSIZE]= {0};
static char oldpath[MAXSIZE] = {0};
//-----------------------------------------客户端命令处理--------------------------------
//-----------------cd命令处理-------------------
int deal_cd(char* data,int sockfd,char** curPath)
{
    char buf[MAXSIZE]={0};
    int ret =  send(sockfd,data,strlen(data),0);
    ERROR_CHECK(ret,-1,"send");
    ret = read(sockfd,buf,sizeof(buf));
    // printf("%s*",buf);
    ERROR_CHECK(ret,-1,"read");
    if(buf[0] == 'N') {
        puts(buf);
        //   printf("%s\n",oldpath);
        *curPath = oldpath;
        return -1;
    }
    if(strcmp(buf,"") == 0){
        sprintf(newpath,"netdisk:~%s$",buf);
        strcpy(oldpath,newpath);
        //  printf("%s",newpath);
        *curPath = newpath;
        //  printf("%s\n",newpath);
    }else if(strcmp(buf,"~") == 0){ 
        sprintf(newpath,"netdisk:%s$",buf);
        strcpy(oldpath,newpath);
        *curPath = newpath;
        //   printf("%s\n",newpath);
    }else{
        sprintf(newpath,"netdisk:~/%s$",buf);
        strcpy(oldpath,newpath);
        *curPath = newpath;
        // printf("%s\n",newpath);
    }
    return 0;
}
//---------------------ls命令处理---------------------
int deal_ls(int sockfd,char* data){
    char buf[MAXSIZE] = {0};
  //  printf("%s\n",data);
    send(sockfd,data,strlen(data),0);
  //  printf("%s\n",data);
    int ret =  read(sockfd,buf,sizeof(buf));
    ERROR_CHECK(ret,-1,"read");
    if(buf[0] == 'c'){
        printf("%s\n",buf);
        return -1;
    }
    // read(sockfd,buf,sizeof(buf)); 两次读在一起会造成读阻塞
    printf("%s",buf);
    // char temp[MAXSIZE]= {0};
    // int i=0,j=0;
    // while(buf[i]!='\0'){
    //     if(buf[i] == 'd'){
    //         while(buf[i]!='-'){
    //             temp[j++] = buf[i++];
    //         }
    //         j = 0;
    //         printf("%s",temp);
    //         memset(temp,0,sizeof(temp));
    //     } else if(buf[i] == '-'){
    //         while(buf[i]!='-'){
    //             temp[j++] = buf[i++];
    //         }
    //         j = 0;
    //         printf("%s",temp);
    //         memset(temp,0,sizeof(temp));
    //     }
    // }
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
