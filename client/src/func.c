#include "func.h"
#include "md5.h"
#include "work_que.h"
#define del_blank(pos,s){ while(s[pos]!='\0'&&(s[pos] == ' '|| s[pos] == '\t')){ pos++; }}
#define get_args(argv,pos,s){ int j = 0; while(s[pos]!='\0'&&s[pos]!='\t'&&s[pos]!=' ') argv[j++] = s[pos++];}
#define strLen 100
#define STR_LEN 10
#define READ_DATA_SIZE  1024                                                
#define MD5_SIZE        16                                                  
#define MD5_STR_LEN     (MD5_SIZE * 2)
char str[STR_LEN];
//连接初始化
int init_client()
{
    printf("Login And Register\n");
    printf("1.login\n");
    printf("2.register\n");
    printf("please input number:");
    int num; 
    scanf("%d",&num);
    getchar();
    fflush(stdin);
    return num;
}
void  init_command(){
    printf("1.cd\n");
    printf("2.ls\n");
    printf("3.puts\n");
    printf("4.gets\n");
    printf("5.rm\n");
    printf("6.pwd\n");
    printf("7.exit\n");
    printf("please input a command:\n");
}
int tcp_init(int* sFd,char* ip,char* port){
    int socketFd;
    socketFd = socket(AF_INET,SOCK_STREAM,0);
    ERROR_CHECK(socketFd,-1,"socket");
    printf("socketFd = %d\n",socketFd);
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(atoi(port));
    serAddr.sin_addr.s_addr = inet_addr(ip);
    int ret = connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    ERROR_CHECK(ret,-1,"connect");
    *sFd = socketFd;
    printf("socketfd = %d\n",socketFd);
    return 0;
}

int separation(char *str, command_t* cmd){
    int i = 0,p = 0;
    // printf("i = %d p = %d\n",i,p);
    str[strlen(str)-1] = '\0';
    del_blank(p,str);
    while(str[p] != '\0'){
        if((cmd->argv[i] = (char*)calloc(strLen,sizeof(char))) == NULL){
            perror("fail malloc");
            return -1;
        }
        get_args(cmd->argv[i],p,str);
        i++;
        del_blank(p,str);
    }
    cmd->argv[i] = NULL;
    cmd->name = cmd->argv[0];
    cmd->cmdlen = strlen(cmd->name);
    //  printf("%d\n",i);
    //  printf("%s %s\n",cmd->name,cmd->argv[1]);
    return i;
}

//随机字符串生成
void GenerateStr(char** str)
{
    int i,flag;
    srand(time(NULL));
    for(i = 0;i < STR_LEN;i++){
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            (*str)[i] = rand()%26 + 'a';
            break;
        case 1:
            (*str)[i] = rand()%26 + 'A';
            break;
        case 2:
            (*str)[i] = rand()%10 + '0';
            break;
        }
    }
    // printf("%s\n",*str);
}
//计算文件md5码
int Compute_file_md5(const char *file_path, char *md5_str)                  
{                                                                           
    int i;                                                                  
    int fd;                                                                 
    int ret;                                                                
    unsigned char data[READ_DATA_SIZE];                                     
    unsigned char md5_value[MD5_SIZE];                                      
    MD5_CTX md5;                                                            

    fd = open(file_path, O_RDONLY);                                         
    if (-1 == fd)                                                           
    {                                                                       
        perror("open");                                                     
        return -1;                                                          
    }                                                                     
    MD5Init(&md5);                                                          
    while (1)                                                               
    {                                                                       
        ret = read(fd, data, READ_DATA_SIZE);                               
        if (-1 == ret)                                                      
        {                                                                   
            perror("read");                                                 
            return -1;
        }                                                                   

        MD5Update(&md5, data, ret);                                         

        if (0 == ret || ret < READ_DATA_SIZE)                               
        {                                                                   
            break;                                                          
        }                                                                   
    }                                                                       
    close(fd);                                                              
    MD5Final(&md5, md5_value);                                              
    for(i = 0; i < MD5_SIZE; i++)                                           
    {                                                                       
        snprintf(md5_str + i*2, 2+1, "%02x", md5_value[i]);                 

    }                                                                       
    md5_str[MD5_STR_LEN] = '\0'; // add end                                 
    return 0;                                                               
}               
//对应于puts命令，主要是上传操作
void puts_t(void* args){
    //重新建立socket
    int socketFd;
    pNode_t conn = (pNode_t)args;
    socketFd = socket(AF_INET,SOCK_STREAM,0);
    printf("socketFd = %d\n",socketFd);
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(conn->port);
    serAddr.sin_addr.s_addr = inet_addr(conn->ip);
    //与服务器建立TCP连接
    connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    train_t data;
    data.dataLen = strlen("W");
    strcpy(data.buf,"W");
    send(socketFd,&data,4 + data.dataLen,0);
    //传输token值
    train_t tokendata;
    tokendata.dataLen = strlen(conn->token);
    strcpy(tokendata.buf,conn->token);
    send(socketFd,&tokendata,tokendata.dataLen + 4,0);
    //传输当前目录值
    train_t curdir;
    char curdirnum[1024] = {0};
    sprintf(curdirnum,"%d",conn->curdirnum);
    curdir.dataLen = strlen(curdirnum);
    strcpy(curdir.buf,curdirnum);
    send(socketFd,&curdir,4 + curdir.dataLen,0);
    //传输MD5值
    char md5str[50] = {0};
    Compute_file_md5(conn->filename,md5str);
    printf("%s\n",md5str);
    train_t md5;
    md5.dataLen = strlen(md5str);
    strcpy(md5.buf,md5str);
    send(socketFd,&md5,4 + md5.dataLen,0);
    char answer[10] = {0};
    recv(socketFd,answer,sizeof(answer),0);
    if(strcmp(answer,"No") == 0){
        train_t filedata;
        filedata.dataLen = strlen(conn->filename);
        strcpy(filedata.buf,conn->filename);
        send(socketFd,&filedata,4 + filedata.dataLen,0);
        int fd = open(conn->filename,O_RDONLY);
        struct stat buf;
        fstat(fd,&buf);
        filedata.dataLen = sizeof(buf.st_size);
        memcpy(filedata.buf,&buf.st_size,filedata.dataLen);
        send(socketFd,&filedata,4 + filedata.dataLen,0);
        int ret;
        if(buf.st_size >= 1073741824){
            char *pMap = (char*)mmap(NULL,buf.st_size,PROT_READ,MAP_SHARED,fd,0);
            send(socketFd,pMap,buf.st_size,0);
        }else{
            while((filedata.dataLen = read(fd,filedata.buf,sizeof(filedata.buf))))
            {
                ret = send(socketFd,&filedata,4 + filedata.dataLen,0);
                if(-1 == ret)
                {
                    goto end;
                }
            }
            send(socketFd,&filedata,4 + filedata.dataLen,0);
end:
            close(fd);
        }

        printf("upload success!\n");
        char isExist[30] = {0};
        recv(socketFd,isExist,sizeof(isExist),0);
        if( strcmp(isExist,"E") == 0){
            printf("filename existed!\n");
            char newfile[30];
            scanf("%s",newfile);
            train_t newfname;
            newfname.dataLen = strlen(newfile);
            strcpy(newfname.buf,newfile);
            send(socketFd,&newfname,4 + newfname.dataLen,0);
        }
    
    }else{
        train_t filedata;
        filedata.dataLen =  strlen(conn->filename);
        strcpy(filedata.buf,conn->filename);
        send(socketFd,&filedata,4 + filedata.dataLen,0);
    }
    close(socketFd);
}
int recvCycle(int sfd,void* pStart,int recvLen){
    char *p = (char*)pStart;
    int total;
    int ret;
    while(total < recvLen){
        ret = recv(sfd,p+total,recvLen-total,0);
        total = total + ret;
        printf("%5.2f%%\r",(double)total/recvLen * 100);
        fflush(stdout);
    }
    printf("\n");
    return 0;
}
//对应于gets命令的线程函数,主要是处理下载
void gets_t(void* args){
    connectargs_t* conn = (connectargs_t*) args;
    int socketFd;
    socketFd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in serAddr;
    bzero(&serAddr,sizeof(serAddr));
    serAddr.sin_family = AF_INET;
    serAddr.sin_port = htons(conn->port);
    serAddr.sin_addr.s_addr = inet_addr(conn->ip);
    connect(socketFd,(struct sockaddr*)&serAddr,sizeof(serAddr));
    send(socketFd,"R",1,0);
    //token值传回服务器端进行验证
    train_t tokendata;
    tokendata.dataLen = strlen(conn->token);
    strcpy(tokendata.buf,conn->token);
    send(socketFd,&tokendata,tokendata.dataLen + 4,0);
    //文件名传至服务器端
    train_t filename;
    filename.dataLen = strlen(conn->filename);
    strcpy(filename.buf,conn->filename);
    send(socketFd,&filename,4 + filename.dataLen,0);
    //当前目录传回至服务器端
    train_t curdir;
    char curdirnum[1024] = {0};
    sprintf(curdirnum,"%d",conn->curdirnum);
    curdir.dataLen = strlen(curdirnum);
    strcpy(curdir.buf,curdirnum);
    send(socketFd,&curdir,4 + curdir.dataLen,0);
    char answer[30] = {0};
    recv(socketFd,answer,sizeof(answer),0);
    if(strcmp(answer,"No such file!") == 0){
        printf("%s\n",answer);
    }else{
        int dataLen;
        char buf[1000] = {0};
        //文件接收
        recv(socketFd,&dataLen,4,MSG_WAITALL);
        recv(socketFd,buf,dataLen,MSG_WAITALL);
        off_t filesize;
        recv(socketFd,&dataLen,4,MSG_WAITALL);
        recv(socketFd,&filesize,dataLen,MSG_WAITALL);
        printf("filesize=%ld\n",filesize);
        int fd;
        fd = open(buf,O_RDWR|O_CREAT,0666);
        ftruncate(fd,filesize);
        char *pMap=(char*)mmap(NULL,filesize,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
        recvCycle(socketFd,pMap,filesize);
        munmap(pMap,filesize);
        close(fd);
    }
    close(socketFd);
}
