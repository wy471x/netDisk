#include "func.h"
int main(int argc,char* argv[])
{
    //-----------配置文件获取IP和PORT----------
    ARGS_CHECK(argc,2);
    char* ip = (char*)calloc(20,sizeof(char));
    char* port = (char*)calloc(6,sizeof(char));
    FILE* fp = fopen(argv[1],"rb+");
    ERROR_CHECK(fp,NULL,"fopen");
    int ret = fscanf(fp,"%s",ip);
    if(ret < 0){
        perror("fscanf");
        return -1;
    }
    printf("%s\n",ip);
    ret = fscanf(fp,"%s",port);
    if(ret < 0){
        perror("fscanf");
    }
    printf("%s\n",port);
    //---------------连接与通信------------
    int socketFd;
    ret = tcp_init(&socketFd,ip,port);
    ERROR_CHECK(ret,-1,"tcp_init");
    int newFd;
    while(1){
        char* curname = (char*)calloc(20,sizeof(char));
        int rootnum ;
        //------------------登录与注册处理------------------
        newFd =accept(socketFd,NULL,NULL);
        recv(newFd,&ret,sizeof(ret),0);
label:
        // printf("%d\n",ret);
        if(ret == 1){
            deal_Login(newFd,&curname,&rootnum);
        }else{
            deal_Register(newFd);
            recv(newFd,&ret,4,0);
            goto label;
        }
        printf("newFd = %d\n",newFd);
        int curdirnum = 0;
        char rootname[MAXSIZE] = {0};
        strcpy(rootname,"root");
        curdirnum = db_numquery_userPath(rootnum,curname,rootname);
        printf("%d\n",curdirnum);
        send(newFd,&curdirnum,4,0);
        if(!fork()) {
            while(1){
                int dataLen = 0 ;
                char buf[MAXSIZE] = {0};
                recv(newFd,&dataLen,4,0);
                recv(newFd,buf,dataLen,0);
                ERROR_CHECK(ret,-1,"recv");
                printf("%d %s\n",dataLen,buf);
                if(strcmp(buf,"cd") == 0){
                      deal_cd(newFd,&curdirnum,curname);
                       continue;
                }
                printf("%d\n",curdirnum);
                if(strcmp(buf,"ls") == 0){
                    printf("%d\n",curdirnum);
                    deal_ls(newFd,curdirnum);
                    continue;
                }
                //                 if(strstr(buf,"rm") == buf){
                //                       char filename[MAXSIZE] = {0};
                //                    if(buf[2] == '\0') strcpy(filename,"");
                //                    else{
                //                        strcpy(filename,&buf[2]);
                //                    }
                //                    if(deal_remove(newFd,filename) < 0) continue;
                //                }
                //                if(strstr(buf,"puts") == buf){
                //                    char filename[MAXSIZE] = {0};
                //                    strcpy(filename,&buf[4]);
                //                    if(deal_upload(newFd,filename) < 0) continue;
                //                }
                //                if(strstr(buf,"gets") == buf){
                //                    char filename[MAXSIZE] ={0};
                //                    // printf("%s",buf);
                //                    strcpy(filename,&buf[4]);
                //                    //  printf("%s\n",filename);
                //                    if(deal_download(newFd,filename) < 0) continue;
                //                }
            }
        }else close(newFd);
    }
    return 0;
}

