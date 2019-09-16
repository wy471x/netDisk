#include "func.h"
//-----------------------------用户信息插入------------------------------
int db_insert_userInfo(char* username,char* salt,char* crypt,char* passwd){
    MYSQL *conn;
    char* server = "localhost";
    char* user = "root";
    char* password = "123";
    char* database = "infor_netdisk";
    char query[200]={0};
    sprintf(query,"insert into user_infor(username,salt,crypt,password) values('%s','%s','%s','%s');",username,salt,crypt,passwd);
    int t;
    //   printf("%s\n",query);
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    //  {
    //      printf("Error connecting to database:%s\n",mysql_error(conn));
    //  }else{
    //      printf("\nConnected...\n");
    //  }
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error :%s\n",mysql_error(conn));
        return -1;
    }else{
        // printf("insert success!\n");
        return 0;
    }
    mysql_close(conn);
}
//-------------------用户信息查询------------------------
int db_query_userInfo(char* username,char** ressalt,char** rescrypt)
{
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select salt,crypt from user_infor where username \
            like '%s';",username);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    // printf("000000\n");
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        // printf(" %d %8s %ld",t,row[t],strlen(row[t]));
                        // if(strlen(row[t]) == 0) return -1;
                        if(t == 0){
                            strncpy(*ressalt,row[t],strlen(row[t]));
                            //      printf("%d /%s/ ",t,row[t]);
                        }
                        if(t == 1){
                            strncpy(*rescrypt,row[t],strlen(row[t]));
                            //      printf("%d /%s/ ",t,row[t]);
                        }
                    }
                    // printf("\n");
                }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//-------------------------用户请求插入-------------------------
int db_insert_userRequest(char* username,char* request,char* enrolltime\
                          ,char* token){
    MYSQL *conn;
    char* server = "localhost";
    char* user = "root";
    char* password = "123";
    char* database = "infor_netdisk";
    char query[200]={0};
    sprintf(query,"insert into user_request(username,request,time,token) \
            values('%s','%s','%s','%s');",username,request,enrolltime,token);
    int t;
    //   printf("%s\n",query);
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    //  {
    //      printf("Error connecting to database:%s\n",mysql_error(conn));
    //  }else{
    //      printf("\nConnected...\n");
    //  }
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error :%s\n",mysql_error(conn));
        return -1;
    }else{
        // printf("insert success!\n");
        return 0;
    }
    mysql_close(conn);
}
//-----------------------------用户注册创建对应目录的插入操作------------------
int db_insert_userPath(char* fname){
    MYSQL *conn;
    char* server = "localhost";
    char* user = "root";
    char* password = "123";
    char* database = "infor_netdisk";
    char query[200]={0};
    sprintf(query,"insert into user_path(prenum,fname,ftype,pfname,fsize) \
            values(0,'%s','d','root',4096);",fname);
    int t;
    //   printf("%s\n",query);
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    //  {
    //      printf("Error connecting to database:%s\n",mysql_error(conn));
    //  }else{
    //      printf("\nConnected...\n");
    //  }
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error :%s\n",mysql_error(conn));
        return -1;
    }else{
        // printf("insert success!\n");
        return 0;
    }
    mysql_close(conn);
}
//---------------------------在当前目录中出入一条记录--------------------
int db_allinsert_userPath(int prenum,char *fname,char *pfname,\
                          char *md5,int fsize,char *vfname){
    MYSQL *conn;
    char* server = "localhost";
    char* user = "root";
    char* password = "123";
    char* database = "infor_netdisk";
    char query[200]={0};
    sprintf(query,"insert into user_path(prenum,fname,ftype,pfname,md5,fsize,\
            vfname)values(%d,'%s','f','%s','%s',%d,'%s');",prenum,fname,\
            pfname,md5,fsize,vfname);
    int t;
    //   printf("%s\n",query);
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    //  {
    //      printf("Error connecting to database:%s\n",mysql_error(conn));
    //  }else{
    //      printf("\nConnected...\n");
    //  }
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error :%s\n",mysql_error(conn));
        return -1;
    }else{
        // printf("insert success!\n");
        return 0;
    }
    mysql_close(conn);
}

//-----------------------目录查询操作-----------------------
int db_numquery_userPath(int num,char* curpathname,char* pname){
    int newdirnum = 0;
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    if(curpathname == NULL&&pname == NULL){
     sprintf(query,"select num from user_path where prenum = %d;",num);
    }else{
     sprintf(query,"select num from user_path where  fname like '%s' and \
             pfname like '%s' and prenum = %d ;",curpathname,pname,num);
    }
    printf("%s\n",query);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    // printf("000000\n");
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        newdirnum =atoi(row[t]);
                        printf("**%d\n",newdirnum);
                    }
                }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return newdirnum;
}
//-----------------------------文件名查找-------------------
int db_namequery_userPath(int num,char *name)
{ 
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select fname  from user_path where num = %d;",num);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    // printf("000000\n");
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        strncpy(name,row[t],strlen(row[t]));                  
                    }
                }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//-----------------------------查找文件的父目录-----------------
int db_prenumquery_userPath(int num){
    int newdirnum = 0;
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select prenum  from user_path where num = %d",num);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    // printf("000000\n");
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        newdirnum =atoi(row[t]);                  
                    }
                }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return newdirnum;
}
//------------------------------当前目录下文件查询-------------------- 
int db_filequery_userPath(int num,char**filelist){
    int newdirnum = 0;
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select  ftype,fname  from user_path where prenum = %d"\
            ,num);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    // printf("000000\n");
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        if(t == 0){
                            strncat(*filelist,row[t],strlen(row[t]));
                        }else{
                            strcat(*filelist," ");
                            strncat(*filelist,row[t],strlen(row[t]));
                            strcat(*filelist,"\n");
                        }                  
                    }
                    printf("%s",*filelist);
                }
            }else {
                strcpy(*filelist,"\n");
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return newdirnum;
}
//----------------------------------用户请求信息表中根据token值查询用户名--------------
int db_usernamequery_userRequest(char* token){
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select  username  from user_request where token like '%s';"\
            ,token);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        // printf("Error:%s\n",mysql_error(conn));
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                    }                  
                 }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//---------------------当前目录下文件名查询-----------------
int db_fexistquery_userPath(int dirnum,char *fname){
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select *  from user_path where prenum = %d and fname like \
            '%s';",dirnum,fname);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                    }
                }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//-----------------------查询服务器上的是否存在md5的文件------------------ 
int db_md5query_userPath(char *md5,char *fname,int *fsize){
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select fname ,fsize from user_path where md5 like '%s'\
            limit 1",md5);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                        if(0 == t)
                        {
                           strncpy(fname,row[t],strlen(row[t]));
                                       
                        }else{
                            *fsize = atoi(row[t]);
                        }
                 
                }
             }
            }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//--------------------查找当前目录的目录名-----------------
int db_dirnamequery_userPath(int curdirnum,char *fname){
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select fname from user_path where  num = %d;"\
            ,curdirnum);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                           strncpy(fname,row[t],strlen(row[t]));
                    }
                 
                }
             }else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
//----------------------查询当前目录下文件名的真实文件名------------------
int db_afnamequery_userPath(int curdirnum,char *fname,char *vfname){
    MYSQL *conn = NULL;
    MYSQL_RES *res = NULL;
    MYSQL_ROW row = NULL;
    char* server="localhost";
    char* user="root";
    char* password="123";
    char* database="infor_netdisk";//要访问的数据库名称
    char query[300]={0};
    sprintf(query,"select fname from user_path where  prenum = %d \
            and vfname like '%s';",curdirnum,vfname);
    unsigned int t;
    conn=mysql_init(NULL);
    mysql_real_connect(conn,server,user,password,database,0,NULL,0);
    t=mysql_query(conn,query);
    res=mysql_store_result(conn);
    mysql_free_result(res);
    t=mysql_query(conn,query);
    if(t)
    {
        return -1;
    }else{
        res=mysql_store_result(conn);
        if(res)
        {  // printf("111111\n");
            if((row=mysql_fetch_row(res))!=NULL)
            {
                mysql_data_seek(res,0);
                // printf("222222\n");
                while((row=mysql_fetch_row(res))!=NULL)
                {
                    for(t=0;t<mysql_num_fields(res);t++)
                    {
                           strcpy(fname,row[t]);
                    }
                 
                }
             } else {
                return -1;          
            }
        }else{
            printf("No data!\n");
        }
        mysql_free_result(res);
    }
    mysql_close(conn);
    return 0;
}
