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
            db_namequery_userPath(pathnum,pathname);
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
void deal_upload(int socketfd){
    printf("puts newFd = %d\n",socketfd);
    char token[50] = {0};
    char md5[50] = {0};
    int dataLen;
    //接收token
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,token,dataLen,0);
    printf("%s\n",token);
    char curdirnum[50]={0};
    //接收当前所处的目录
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,curdirnum,dataLen,0);
    printf("%s\n",curdirnum);
    //接收MD5
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,md5,dataLen,0);
    printf("%s\n",md5);
    //查询token的用户
    if(db_usernamequery_userRequest(token) < 0){
        return;
    }
    printf("1111\n");
    char fname[30] ={0};
    //类型要一致才可
    int fsize = 0;
    //当文件名在本目录下不存在且该文件的内容即md5也不存在时
    if(db_md5query_userPath(md5,fname,&fsize) < 0)
    {
        printf("222\n");
        send(socketfd,"No",2,0);
        char filename[20] = {0};
        recv(socketfd,&dataLen,4,0);
        recv(socketfd,filename,dataLen,0);
        printf("%s\n",filename);
        recv(socketfd,&dataLen,4,0);
        recv(socketfd,&fsize,dataLen,0);
        printf("%d\n",fsize);
        //使用mmap技术接收文件内容
        int fd;
        fd = open(filename,O_WRONLY|O_CREAT,0666);
        if(fsize >= 1073741824){
            ftruncate(fd,fsize);
            char *pMap = (char*)mmap(NULL,fsize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
            printf("33333\n");
            recv_file(socketfd,pMap,fsize);
            printf("mid3333\n");
            munmap(pMap,fsize);
            printf("3333\n");
        }else{
            char buf[1000] = {0};
            while(1){
                recv_file(socketfd,&dataLen,4);
                printf("%d\n",dataLen);
                if(dataLen > 0){
                    recv_file(socketfd,buf,dataLen);
                    printf("%s",buf);
                    write(fd,buf,dataLen);
                }else{
                    printf("100.00%%\n");
                    break;
                }
            }
        }
        char pfname[30] = {0};
        db_dirnamequery_userPath(atoi(curdirnum),pfname);
        printf("**%s\n",pfname);
        train_t buf;
        if(db_fexistquery_userPath(atoi(curdirnum),filename) < 0){
            buf.dataLen = strlen("NE");
            strcpy(buf.buf,"NE");
            send(socketfd,&buf,4 + buf.dataLen,0);
            printf("%d %s %s %s %d %s\n",atoi(curdirnum),filename,pfname,md5,fsize,filename);
            db_allinsert_userPath(atoi(curdirnum),filename,pfname,md5,fsize\
                                  ,filename);
        }else{
            buf.dataLen = strlen("E");
            strcpy(buf.buf,"E");
            send(socketfd,&buf,4 + buf.dataLen,0);
            char newfname[20] = {0};
            recv(socketfd,&dataLen,4,0);
            recv(socketfd,newfname,dataLen,0);
            db_allinsert_userPath(atoi(curdirnum),filename,pfname,md5,fsize\
                                  ,newfname);
        }
        close(fd);
    }
    else
    {
        send(socketfd,"Yes",3,0);
        char filename[30] = {0};
        recv(socketfd,&dataLen,4,0);
        recv(socketfd,filename,dataLen,0);
        if(db_fexistquery_userPath(atoi(curdirnum),filename) < 0){
            char pfname[30] = {0};
            db_namequery_userPath(atoi(curdirnum),pfname); 
            printf("%d %s %s %s %d %s\n",atoi(curdirnum),fname,pfname,md5,fsize,filename);
            db_allinsert_userPath(atoi(curdirnum),fname,pfname,md5,fsize\
                                  ,filename);
        }
    }
    close(socketfd);
}
//----------------------------downloads命令处理-------------------
void deal_download(int socketfd){
    int dataLen;
    char token[50] = {0};
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,token,dataLen,0);
    if(db_usernamequery_userRequest(token) < 0){
        return;
    }
    char filename[30] = {0};
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,filename,dataLen,0);
    char curdir[30] = {0};
    recv(socketfd,&dataLen,4,0);
    recv(socketfd,curdir,dataLen,0);
    char afname[50] = {0};
    train_t buf;
    if(db_afnamequery_userPath(atoi(curdir),afname,filename) < 0)
    {
        buf.dataLen = strlen("No such file!");
        strcpy(buf.buf,"No such file!");
        send(socketfd,&buf,4 + buf.dataLen,0);
    }
    else
    {
        buf.dataLen = strlen("Yes");
        strcpy(buf.buf,"Yes");
        send(socketfd,&buf,4 + buf.dataLen,0);
        printf("%s\n",afname);
        send_file(socketfd,afname);
    }
    close(socketfd);
}
