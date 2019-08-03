#ifndef __FUNC_H__
#define __FUNC_H__
#include "head.h"
#define MAXSIZE 1000
typedef struct {
    int dataLen;
    char buf[MAXSIZE];
}train_t;
typedef struct{
    int cmdlen;
    char* name;
    char* argv[MAXSIZE];
}command_t;
typedef struct {
    int usrLen;
    char usrname[20];
}usr_t;
int tcp_init(int*,char*,char*);
int deal_cd(int,int*,char*);
int deal_ls(int,int);
int deal_remove(int,char*);
int deal_upload(int,char*);
int deal_download(int,char*);
int recv_file(int,void*,int);
int send_file(int,char*);
void deal_Login(int sockfd,char**name,int* dirnum);
void deal_Register(int);
int db_insert_userInfo(char*,char*,char*,char*);
int db_query_userInfo(char*,char**,char**);
int db_insert_userRequest(char*,char*);
int db_insert_userPath(char*);
int db_numquery_userPath(int,char*,char*);
int db_prenumquery_userPath(int );
int db_namequery_userPath(int ,char**);
int db_filequery_userPath(int,char**);
#endif
