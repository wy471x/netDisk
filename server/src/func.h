#ifndef __FUNC_H__
#define __FUNC_H__
#include "head.h"
#define MAXSIZE 1000
typedef struct{
    int cmdlen;
    char* name;
    char* argv[MAXSIZE];
}command_t;
typedef struct {
    int usrLen;
    char usrname[20];
}usr_t;
typedef struct {
    int dataLen;
    char buf[MAXSIZE];
}train_t;
int tcp_init(int*,char*,char*);
void GenerateStr(char*);
int deal_cd(int,int*,char*);
int deal_ls(int,int);
int deal_remove(int,char*);
void deal_upload(int);
void deal_download(int);
int recv_file(int,void*,int);
int send_file(int,char*);
void requestDeal(int);
void deal_Login(int sockfd,char *name,int* dirnum);
void deal_Register(int);
int db_insert_userInfo(char*,char*,char*,char*);
int db_query_userInfo(char*,char**,char**);
int db_insert_userRequest(char*,char*,char*,char*);
int db_insert_userPath(char*);
int db_numquery_userPath(int,char*,char*);
int db_prenumquery_userPath(int );
int db_namequery_userPath(int ,char*);
int db_filequery_userPath(int,char**);
int db_usernamequery_userRequest(char*);
int db_fexistquery_userPath(int,char*);
int db_md5query_userPath(char *md5,char *fname,int *);
int db_dirnamequery_userPath(int curdirnum,char *fname);
int db_allinsert_userPath(int prenum,char *fname,char *pfname,char *md5,int fsize,char *vfname);
int db_afnamequery_userPath(int curdirnum,char *fname,char *vfname);
#endif
