#ifndef __FUNC_H__
#define __FUNC_H__
#include "head.h"
#define MAXSIZE  2000
typedef struct{
    int dataLen;
    char buf[1000];
}train_t;
typedef struct {
    int cmdlen;
    char* name;
    char* argv[MAXSIZE];
}command_t;
typedef struct {
    int usrLen;
    char usrname[20];
}usr_t;
typedef struct {
    char ip[30];
    int port;
    char filename[50];
    char token[50];
    int curdirnum;
}connectargs_t;
int init_client();
int Login_T(int,char**,int*,char*);
int Register_T(int);
int tcp_init(int*,char*,char*);
void init_command();
int separation(char*,command_t*);
void GenerateStr(char**);
void createThread(char*,connectargs_t*);
int deal_cd(char*,int,char**,int*);
int deal_ls(int);
void deal_pwd(char**);
int deal_remove(int,char*);
int deal_puts(int,char*,char*);
int deal_gets(int,char*,char*);
int send_file(int,char*);
int recv_file(int,void*,int);
int db_insert_userInfo(char*,char*,char*,char*);
int db_query_userInfo(char*,char**,char**);
void gets_t(void*);
void puts_t(void*);
#endif
