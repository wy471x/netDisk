#include "func.h"
int main(int argc,char* argv[])
{
    //------------------配置文件获取IP和PORT--------------
    ARGS_CHECK(argc,2); 
    char * ip =(char*) calloc(20,sizeof(char));
    char * port = (char*)calloc(6,sizeof(char));
    FILE* fp = fopen(argv[1],"rb+");
    ERROR_CHECK(fp,NULL,"fopen");
    int ret;
    ret = fscanf(fp,"%s",ip);
    if(ret <0){
        perror("fscanf");
    }
    printf("%s\n",ip);
    ret = fscanf(fp,"%s",port);
    if(ret <0){
        perror("fscanf");
    }
    printf("%s\n",port);
    //------------------连接与通信-------------------
    int socketFd;
    char* curUser = (char*)calloc(20,sizeof(char));
    int curdirnum;
    tcp_init(&socketFd,ip,port);
    //----------------------用户注册和登陆----------------- 
label:
    system("clear");
    ret =  init_client();
    fflush(stdin);
    send(socketFd,&ret,4,0);
    if(ret == 1){
        ret = Login_T(socketFd,&curUser,&curdirnum);
        if(ret == -1){
            return 0;
        }else{
            system("clear");
            printf("Success Login!\n");
            // send(socketFd,&ret,4,0);
        }
    }else if(ret == 2){
        ret = Register_T(socketFd);
        if(ret == 0){
            ret = 1;
            // printf("%d\n",ret);
            send(socketFd,&ret,4,0);
            ret =  Login_T(socketFd,&curUser,&curdirnum);
            if(ret == -1){
                return 0;
            }
            else{
                system("clear");
                printf("Success Login!\n");
                //    send(socketFd,&ret,4,0);
            }
        }
        else return 0;
    }else {
        printf("wrong number!\n");
        goto label;
    }
    char cmd[MAXSIZE] = {0};
    command_t command;
    command.name = NULL;
    memset(command.argv,0,sizeof(command.argv));
    recv(socketFd,&curdirnum,4,0);
    init_command();
    // printf("%s\n",curUser);
    char* curPath = (char *)calloc(50,sizeof(char));
    char tempPath[MAXSIZE]={0};
    strcpy(tempPath,curUser);
    strcat(tempPath,":~$");
    // printf("%s\n",tempPath);
    curPath =tempPath;
    // puts(curPath);
    printf("%s",curPath);
    while(1){
        fgets(cmd,sizeof(cmd),stdin);
        // printf("1%s",cmd);
        ret = separation(cmd,&command);
        // printf("2%s %s\n",command.argv[0],command.argv[1]);
        train_t data;
        data.dataLen = command.cmdlen;
        strcpy(data.buf,command.name);
        //  printf("%s\n",data.buf);
        send(socketFd,&data,4+data.dataLen,0);
        // printf("%d\n",curdirnum);
        // printf("ret = %d\n",ret);
        //  printf("2%s %s\n",command.argv[0],command.argv[1]);
        // printf("3%s\n",buf);
        ERROR_CHECK(ret,-1,"separation");
        if(strcmp(command.name,"cd") == 0){
            //    printf("%d %s %s\n",command.cmdlen,command.name,command.argv[1]);
            char path[MAXSIZE] = {0};
            if(ret == 1){ 
                strcpy(path,"");       
            }
            else{  
                strcpy(path,command.argv[1]);
            }
            if(deal_cd(path,socketFd,&curPath,&curdirnum)<0){
                printf("%s",curPath);
                continue;
            }
            else{
                printf("%s",curPath);
                continue;
            }
        }
        if(strcmp(command.name,"ls") == 0){
           // printf("%d\n",curdirnum);
              deal_ls(socketFd);
              printf("%s",curPath);
              continue;
        }
        //        if(strcmp(command.name,"pwd") == 0){
        //            char data[MAXSIZE] = {0};
        //            strcpy(data,command.argv[0]);
        //            deal_pwd(&curPath);
        //            printf("%s",curPath);
        //            continue;
        //        }
        //        if(strcmp(command.name,"rm") == 0){
        //            char data[MAXSIZE] = {0};
        //            strcpy(data,command.argv[0]);
        //            strcat(data,command.argv[1]);
        //            if(deal_remove(socketFd,data)){
        //                printf("%s",curPath);
        //                continue;
        //            }else{
        //                printf("%s",curPath);
        //                continue;
        //            }
        //        }
        //        if(strcmp(command.name,"puts") == 0)
        //        {
        //            char data[MAXSIZE] = {0};
        //            strcpy(data,command.argv[0]);
        //            strcat(data,command.argv[1]);
        //            if(ret == 1)
        //            {
        //                printf("%s",curPath);
        //                continue;
        //
        //            }else if(ret == 2)
        //            {
        //                int fd;
        //                if((fd = open(command.argv[1],O_RDONLY)) == -1)
        //                {
        //                    perror("open");
        //                    printf("%s",curPath);
        //                    continue;
        //                }
        //                else
        //                {
        //                    deal_puts(socketFd,data,command.argv[1]);
        //                    printf("%s",curPath);
        //                    continue;
        //                }
        //            }
        //        }
        //        if(strcmp(command.name,"gets") == 0){
        //            char data[MAXSIZE] = {0};
        //            strcpy(data,command.argv[0]);
        //            if(ret == 1){
        //                printf("%s",curPath);
        //                continue;
        //            }
        //            else if(ret == 2){
        //                strcat(data,command.argv[1]);    
        //                if(deal_gets(socketFd,data,command.argv[1]) < 0) 
        //                {
        //                    printf("%s",curPath);
        //                    continue;
        //                }
        //                printf("%s",curPath);
        //                continue;
        //            }
        //            printf("%s",curPath);
        //            continue;
        //        }
        //        if(strcmp(command.name,"exit") == 0){
        //            return 0;
        //        }else{
        //            return 0;
        //        }
        // int i = 0;
        // while(ret--){
        //     free(command.argv[i++]);
        // }
        // printf("%s",curPath);
    }
    return 0;

}

