#include "func.h"

void deal_Register(int sockfd)
{
    char passwd[20] = {0};
    char username[20] = {0};
    char salt[30] = {0};
    char crypt[50] = {0};
    int dataLen = 0;
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,username,dataLen,0);
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,passwd,dataLen,0);
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,salt,dataLen,0);
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,crypt,dataLen,0);
    int ret = db_insert_userInfo(username,salt,crypt,passwd);
    if(ret == 0){
        char request[10] = {0};
        send(sockfd,&ret,4,0);
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,request,dataLen,0);
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,username,dataLen,0);
        db_insert_userRequest(username,request);
        db_insert_userPath(username);
    }else{
        send(sockfd,&ret,4,0);
    } 
}

void deal_Login(int sockfd,char** name,int* dirnum){
    char username[20] = {0};
    int dataLen = 0;
    train_t data;
    memset(data.buf,0,sizeof(data.buf));
    data.dataLen = 0;
label:
    recv(sockfd,&dataLen,4,0);
    recv(sockfd,username,dataLen,0);
    // printf("%s\n",username);
    char* ressalt = (char*)calloc(300,sizeof(char));
    char* rescrypt = (char*)calloc(300,sizeof(char));
    int ret = db_query_userInfo(username,&ressalt,&rescrypt);
    if( ret == -1 ){
        send(sockfd,&ret,4,0);
        recv(sockfd,&ret,4,0);
        if(ret == 1){
            memset(username,0,sizeof(username));
            dataLen = 0;
            data.dataLen = 0;
            memset(data.buf,0,sizeof(data.buf));
            goto label;   
        }
    }else{
        send(sockfd,&ret,4,0);
        data.dataLen = strlen(ressalt);
        strcpy(data.buf,ressalt);
        send(sockfd,&data,4+data.dataLen,0);
        data.dataLen = strlen(rescrypt);
        strcpy(data.buf,rescrypt);
        send(sockfd,&data,4+data.dataLen,0);
        int sig;
        recv(sockfd,&sig,4,0);
        if(sig == 0){
            char request[10] = {0};
            recv(sockfd,&dataLen,4,0);
            recv(sockfd,request,dataLen,0);
            recv(sockfd,&dataLen,4,0);
            recv(sockfd,username,dataLen,0);
            strcpy(*name,username);
            db_insert_userRequest(username,request);
            char rootname[MAXSIZE] = {0};
            char prootname[MAXSIZE] = {0};
            int rootnum = 0;
            rootnum = db_numquery_userPath(0,rootname,prootname);
            send(sockfd,&rootnum,4,0);
            *dirnum = rootnum;
        }else{
            recv(sockfd,&sig,4,0);
            if(sig == 1){
                memset(username,0,sizeof(username));
                dataLen = 0;
                data.dataLen = 0;
                memset(data.buf,0,sizeof(data.buf));
                goto label;
            }
        }
    }
}
