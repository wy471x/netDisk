#include "func.h"
//保存当前工作根目录
char invarPath[] = "/home/dunk/netdisk/server/src";
//#define strLen 20
//---------------------------------服务器命令处理--------------------
//-----------------cd 命令处理--------------------
int deal_cd(char* path,int sockfd){
    char buf[MAXSIZE] = {0};
    static char oldpath[MAXSIZE] = "/home/dunk/netdisk/server/src";
    static int level = 0; 
    if(strcmp(path,"")==0&&strcmp(getcwd(NULL,0),oldpath) == 0){
        strcpy(buf,"~");
        write(sockfd,buf,strlen(buf)); 
        while(level--){
            chdir("..");
            // printf("%s\n",getcwd(NULL,0));
        }
        // printf("oldlevel = %d\n",level);
        level += 1;
        // printf("3%s\n",getcwd(NULL,0));
        return 0;
    }else if((strcmp(path,"") == 0&&strcmp(invarPath,oldpath) == 0)||(strcmp(path,"..") == 0&&strcmp(invarPath,oldpath)==0)||(strcmp(path,".") == 0&&strcmp(invarPath,oldpath)==0)) {
        write(sockfd,"",1);
        // printf("1000000\n");
        return 0;
    }else if(strcmp(path,"..") == 0&&strcmp(getcwd(NULL,0),oldpath) == 0){
        chdir("..");
        char cur[MAXSIZE] ={0};
        strcpy(cur,getcwd(NULL,0));
        // printf("%s\n",cur);
        int len = strlen(invarPath);
        strcpy(buf,&cur[len]);
        //  printf("1%s*\n",buf);
        if(strcmp(buf,"") == 0){
            write(sockfd,"",1);
        }else{
            write(sockfd,buf,strlen(buf));
        }
        return 0;
    }
    //printf("1%s\n",path);
    if(chdir(path) < 0){
        perror("chdir");
        strcpy(buf,"No such file or directory");
        write(sockfd,buf,strlen(buf));
        return -1;
    }
    else{
        level += 1;
        // printf("newlevel = %d\n",level);
        strcpy(buf,path);
        // printf("%s\n",getcwd(NULL,0));
        strcpy(oldpath,getcwd(NULL,0));
        //printf("%s",buf);
        write(sockfd,buf,strlen(buf));
        return 0;
    }
}
//--------------------ls命令处理---------------------
int deal_ls(int sockfd,char* dir){
    char buf[MAXSIZE]={0};
    struct dirent* pdirinfo;
    DIR* pdir;
    if(strcmp(dir,"") == 0){
        pdir = opendir(".");
    }else{
        pdir = opendir(dir);
    }
    if(NULL == pdir)
    {
        perror("cannot access,open dir fail!");
        strcpy(buf,"cannot access,open dir fail!");
        write(sockfd,buf,strlen(buf));
        return -1;
    }
    // printf("------\n");
    while((pdirinfo = readdir(pdir)) != NULL){
        if((strcmp(pdirinfo->d_name,".") != 0)&&(strcmp(pdirinfo->d_name,"..") != 0)){
            if(pdirinfo->d_type == 4&&strcmp(buf,"") == 0){
                strcpy(buf,"d ");
                strcat(buf,pdirinfo->d_name);
                strcat(buf,"\n");   
            }else if(pdirinfo->d_type == 8&&strcmp(buf,"") == 0){
                strcpy(buf,"- ");
                strcat(buf,pdirinfo->d_name);
                strcat(buf,"\n");
            }else if(strcmp(buf,"")!=0&&pdirinfo->d_type == 4){
                strcat(buf,"d ");
                strcat(buf,pdirinfo->d_name);
                strcat(buf,"\n");   
            }else if(strcmp(buf,"")!=0&&pdirinfo->d_type == 8){
                strcat(buf,"- ");
                strcat(buf,pdirinfo->d_name);
                strcat(buf,"\n");   
            }
        }
    }
    write(sockfd,buf,strlen(buf));
    // printf("%s",buf);
    closedir(pdir);
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
