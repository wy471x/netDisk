#include "func.h"
//#define strLen 20
//---------------------------------服务器命令处理--------------------
//-----------------cd 命令处理--------------------
int deal_cd(int sockfd,int* curdirnum,char* username){
    static char oldpath[MAXSIZE] = {0};
    static char rootpath[MAXSIZE] = {0};
    static int  olddirnum = 0;
    static char curpathname[MAXSIZE] = {0};
    static char oldpathname[MAXSIZE] = {0};
    strcpy(oldpathname,username);
    int rootdirnum = *curdirnum;
    char curPath[MAXSIZE] = {0};
    sprintf(rootpath,"%s:~$",username);
    int dataLen = 0;
    train_t data;
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,oldpath,dataLen,0);
    printf("1%s\n",oldpath);
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,curPath,dataLen,0);
    printf("2%s\n",curPath);
    olddirnum = *curdirnum;
    printf("%d\n",olddirnum);
    int pathLen = 0;
    char path[MAXSIZE] = {0};
    recv(sockfd,&pathLen,4,0);
    recv(sockfd,path,pathLen,0);
    printf("*%s\n",path);
    if((strcmp(path," ") == 0&&strcmp(curPath,rootpath) == 0)\
       ||(strcmp(path,".")==0&&strcmp(curPath,rootpath) == 0)\
       ||(strcmp(path,"..") == 0&&strcmp(curPath,rootpath) == 0)) {
        int sig = 1;
        send(sockfd,&sig,4,0);
        strcpy(curPath,rootpath);
        data.dataLen = strlen(curPath);
        strcpy(data.buf,curPath);
        send(sockfd,&data,4 + data.dataLen,0);
        *curdirnum = rootdirnum;
        send(sockfd,&*curdirnum,4,0);
        return 0;
    }
    if(strcmp(path," ") == 0&&strcmp(curPath,oldpath) == 0){
        int sig = 2;
        send(sockfd,&sig,4,0);
        strcpy(curPath,rootpath);
        data.dataLen = strlen(curPath);
        strcpy(data.buf,curPath);
        send(sockfd,&data,4+data.dataLen,0);
        *curdirnum = olddirnum;
        send(sockfd,&*curdirnum,4,0);
        return 0;
    }
    if(strcmp(path,".") == 0&&strcmp(curPath,oldpath) == 0){
        int sig = 3;
        send(sockfd,&sig,4,0);
        strcpy(curPath,oldpath);
        data.dataLen = strlen(curPath);
        strcpy(data.buf,curPath);
        send(sockfd,&data,4 + data.dataLen,0);
        *curdirnum = olddirnum;
        send(sockfd,&*curdirnum,4,0);
        return 0;
    }
    if(strcmp(path,"..") == 0&&strcmp(curPath,oldpath) == 0){
        int sig =4;
        send(sockfd,&sig,4,0);
        int i = strlen(curPath) - 1;
        char tmpPath[MAXSIZE] = {0};
        strcpy(tmpPath,curPath);
        while(tmpPath[i] != '/'){
            i--;
        }
        tmpPath[i] = '\0';
        strcat(tmpPath,"$");
        strcpy(curPath,tmpPath);
        int tmpdirnum = db_prenumquery_userPath(olddirnum);
        *curdirnum = tmpdirnum;
        data.dataLen = strlen(curPath);
        strcpy(data.buf,curPath);
        send(sockfd,&data,4+data.dataLen,0);
        send(sockfd,&*curdirnum,4,0);
        return 0;
    }
    if(strcmp(path," ") !=0&&strcmp(curPath,oldpath) == 0){
        int pathnum = 0;
        printf("%s\n",path);
        strcpy(curpathname,path);
        pathnum = db_numquery_userPath(olddirnum,curpathname,oldpathname);
        printf("%d %s\n",olddirnum,curpathname);
        printf("%d\n",pathnum);
        if(pathnum != -1){
            strcpy(curpathname,path);
            int sig = 0;
            send(sockfd,&sig,4,0);
            char* pathname = (char*)calloc(30,sizeof(char));
            db_namequery_userPath(pathnum,&pathname);
            *curdirnum = pathnum;
            send(sockfd,&*curdirnum,4,0);
            sprintf(curPath,"%s:~/%s$",username,pathname);
            data.dataLen = strlen(curPath);
            strcpy(data.buf,curPath);
            send(sockfd,&data,4 + data.dataLen,0);
            return 0;
        }else{
            int sig = -1;
            send(sockfd,&sig,4,0);
            send(sockfd,"No such file or directory",25,0);

        }
    }
    return 0;
}
//--------------------ls命令处理---------------------
int deal_ls(int sockfd,int curdirnum){
    char* filelist = (char*)calloc(MAXSIZE,sizeof(char));
    db_filequery_userPath(curdirnum,&filelist);
    send(sockfd,filelist,strlen(filelist),0);
    return 0;
}
//-----------------------remove命令处理------------------
int deal_remove(int sockfd,char* filename){
    struct dirent* pdirinfo;
    struct stat st;
    char buf[MAXSIZE] = {0};
    int ret = 0;
    DIR* pdir = opendir(".");
    if(NULL == pdir){
        perror("No such file or directory");
        strcpy(buf,"No such file or directory");                                            
        write(sockfd,buf,strlen(buf));
        return -1;
    }
    while((pdirinfo = readdir(pdir)) != NULL){
        stat(pdirinfo->d_name,&st);
        if(S_ISDIR(st.st_mode)&&strcmp(pdirinfo->d_name,filename) == 0){
            ret = rmdir(pdirinfo->d_name);
            printf("%d",ret);
            write(sockfd,"Success",7);
            return 0;
        }
        if(S_ISREG(st.st_mode)&& strcmp(pdirinfo->d_name,filename) == 0){
            unlink(pdirinfo->d_name);
            write(sockfd,"Success",7);
            return 0;
        }
    }
    return 0 ;
}
//---------------------------uploads命令处理-----------------
int deal_upload(int sockfd,char* filename){
    send(sockfd,"Success",7,0);
    int fd = open(filename,O_RDWR|O_CREAT,0666);
    int dataLen;
    off_t filesize;
    recv_file(sockfd,&dataLen,4);
    recv_file(sockfd,&filesize,dataLen);
    printf("received filesize：%ld\n",filesize);
    ftruncate(fd,filesize);
    char* pMap = (char*)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    ERROR_CHECK(pMap,(char*)-1,"mmap");
    recv_file(sockfd,pMap,filesize);
    int ret =  munmap(pMap,filesize);
    ERROR_CHECK(ret,-1,"munmap");
    close(fd);
    //close(sockfd)子进程将套接字关闭导致返回 bad file descriptor错误
    return 0;
}
//----------------------------downloads命令处理-------------------
int deal_download(int sockfd,char* filename){
    int fd = open(filename,O_RDONLY);
    if(fd == -1){
        perror("open");
        send(sockfd,"No such file or directory",25,0);
        return -1;
    }else{
        send(sockfd,"Success",7,0);
        send_file(sockfd,filename);
        return 0;    
    }
}
