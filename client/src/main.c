#include "func.h"
#include "factory.h"
int main(int argc,char* argv[])
{
    //------------------配置文件获取IP和PORT--------------
    ARGS_CHECK(argc,2); 
    char * ip =(char*) calloc(20,sizeof(char));
    char * port = (char*)calloc(6,sizeof(char));
    int threadnum,capacity;
    FILE* fp = fopen(argv[1],"rb+");
    ERROR_CHECK(fp,NULL,"fopen");
    int ret;
    ret = fscanf(fp,"%s",ip);
    if(ret < 0){
        perror("fscanf");
    }
    printf("%s\n",ip);
    ret = fscanf(fp,"%s",port);
    if(ret <0){
        perror("fscanf");
    }
    printf("%s\n",port);
    fscanf(fp,"%d",&threadnum);
    fscanf(fp,"%d",&capacity);
    //------------------连接与通信-------------------
    int socketFd;
    char* curUser = (char*)calloc(20,sizeof(char));
    int curdirnum;
    char token[50] = {0};
    tcp_init(&socketFd,ip,port);
    train_t data;
    data.dataLen = strlen("LR");
    strcpy(data.buf,"LR");
    send(socketFd,&data,4 + data.dataLen,0);
    //----------------------用户注册和登陆----------------- 
label:
    //system("clear");
    ret =  init_client();
    fflush(stdin);
    send(socketFd,&ret,4,0);
    if(ret == 1){
        ret = Login_T(socketFd,&curUser,&curdirnum,token);
        if(ret == -1){
            return 0;
        }else{
            system("clear");
            printf("Success Login!\n");
        }
    }else if(ret == 2){
        ret = Register_T(socketFd);
        if(ret == 0){
            ret = 1;
            // printf("%d\n",ret);
            send(socketFd,&ret,4,0);
            ret =  Login_T(socketFd,&curUser,&curdirnum,token);
            if(ret == -1){
                return 0;
            }
            else{
                system("clear");
                printf("Success Login!\n");
            }
        }
        else return 0;
    }else {
        printf("wrong number!\n");
        goto label;
    }
    printf("%d\n",curdirnum);
    char cmd[MAXSIZE] = {0};
    command_t command;
    command.name = NULL;
    memset(command.argv,0,sizeof(command.argv));
    init_command();
    char* curPath = (char *)calloc(50,sizeof(char));
    char tempPath[MAXSIZE]={0};
    strcpy(tempPath,curUser);
    strcat(tempPath,":~$");
    curPath =tempPath;
    Factory_t threadMainData;
    factoryInit(&threadMainData,threadnum,capacity);
    factoryStart(&threadMainData);
    pQue_t pq = &threadMainData.que;
    pNode_t pNew;
    while(1){
        printf("%s",curPath);
        fgets(cmd,sizeof(cmd),stdin);
        ret = separation(cmd,&command);
        ERROR_CHECK(ret,-1,"separation");
        send(socketFd,command.name,strlen(command.name),0);
        if(strcmp(command.name,"cd") == 0||strcmp(command.name,"ls") == 0||\
           strcmp(command.name,"pwd") == 0)
        {
           
        }
        if(strcmp(command.name,"puts") == 0)
        {
            pNew = (pNode_t)calloc(1,sizeof(Node_t));
            strcpy(pNew->ip,ip);
            printf("%s\n",pNew->ip);
            pNew->port = atoi(port);
            printf("%d\n",pNew->port);
            strcpy(pNew->filename,command.argv[1]);
            printf("%s\n",pNew->filename);
            pNew->request = 1;
            pNew->curdirnum = curdirnum;
            printf("%d\n",pNew->curdirnum);
            strcpy(pNew->token,token);
            printf("%s\n",pNew->token);
            pthread_mutex_lock(&pq->mutex);
            printf("front mutext\n");
            queInsert(pq,pNew);
            printf("back mutex\n");
            pthread_mutex_unlock(&pq->mutex);
            pthread_cond_signal(&threadMainData.cond);
            printf("puts\n");
        }
        if(strcmp(command.name,"gets") == 0)
        {
            pNew = (pNode_t)calloc(1,sizeof(Node_t));
            strcpy(pNew->ip,ip);
            pNew->port = atoi(port);
            strcpy(pNew->filename,command.argv[1]);
            pNew->request = 0;
            pNew->curdirnum = curdirnum;
            strcpy(pNew->token,token);
            pthread_mutex_lock(&pq->mutex);
            queInsert(pq,pNew);
            pthread_mutex_unlock(&pq->mutex);
            pthread_cond_signal(&threadMainData.cond);
        }
    }
    close(socketFd);
    return 0;
}

