#include "func.h"

int Register_T(int sockfd)
{
    train_t data;
    char username[20];
    memset(username,0,sizeof(username));
label:
    fflush(stdin);
    printf("username:");
    fgets(username,sizeof(username),stdin);
    username[strlen(username) - 1] = '\0';
    data.dataLen = strlen(username);
    strcpy(data.buf,username);
    send(sockfd,&data,4+data.dataLen,0);
    printf("\npassword:");
    char passwd[20] = {0};
    fgets(passwd,sizeof(passwd),stdin);
    passwd[strlen(passwd) -1] = '\0';
label2:
    printf("\nconfirm password:");
    char passwd2[20] = {0};
    fgets(passwd2,sizeof(passwd2),stdin);
    passwd2[strlen(passwd2) - 1] = '\0';
    if(strcmp(passwd,passwd2) != 0){
        goto label2;  
    }
    data.dataLen = strlen(passwd);
    strcpy(data.buf,passwd);
    send(sockfd,&data,4+data.dataLen,0);
    // printf("%s %s\n",passwd,passwd2);
    char* salt = (char*)calloc(30,sizeof(char));
    GenerateStr(&salt);
    data.dataLen = strlen(salt);
    strcpy(data.buf,salt);
    send(sockfd,&data,4+data.dataLen,0);
    //  printf("%s",salt);
    char cyt[50] = {0};
    char tmp = 0;
    strcpy(cyt,crypt(passwd,salt));
    data.dataLen = strlen(cyt);
    strcpy(data.buf,cyt);
    send(sockfd,&data,4+data.dataLen,0);
    int ret;
    recv(sockfd,&ret,sizeof(ret),0);
    if(ret == 0){
        printf("Success Register!\n");
        printf("Do you want to login?(Y(y)/N(n)):");
        scanf("%c",&tmp);
        getchar();
        if(tmp == 'Y'||tmp == 'y'){
            system("clear");
        }else{
            return -1;
        }
    }else{
        printf("Username is duplicate!\n ");
        printf("Do you want to register again?(Y(y)/N(n)):");
        scanf("%c",&tmp);
        getchar();
        if(tmp == 'Y'|| tmp == 'y'){
            system("clear");
            goto label;
        }else{
            return -1;
        }
    }
    return 0;
}

int Login_T(int sockfd,char** curUser,int* curdirnum,char *token)
{
    train_t data;
    char username[20];
    memset(username,0,sizeof(username));  
label:
    printf("username:");
    fgets(username,sizeof(username),stdin);
    username[strlen(username) - 1] = '\0';
    data.dataLen = strlen(username);
    strcpy(data.buf,username);
    send(sockfd,&data,4+data.dataLen,0);
    char passwd[20] = {0};
    printf("\npassword:");
    fgets(passwd,sizeof(passwd),stdin);
    passwd[strlen(passwd) - 1] = '\0';
    char tmp = 0;
    int ret ;
    recv(sockfd,&ret,4,0);
    if(ret == -1){
        printf("Username doesn't exist!\n");
        printf("Do you want to login again?(Y(y)/N(n)):");
        scanf("%c",&tmp);
        getchar();
        if( tmp == 'Y'||tmp == 'y' ){
            int sig = 1;
            send(sockfd,&sig,4,0);
            goto label;      
        }else{
            return -1;
        }
    }else{
        int dataLen = 0;
        char ressalt[50] = {0};
        char rescrypt[50] = {0};
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,ressalt,dataLen,0);
        recv(sockfd,&dataLen,4,0);
        recv(sockfd,rescrypt,dataLen,0);
        if(strcmp(rescrypt,crypt(passwd,ressalt)) == 0){
            strcpy(*curUser,username);
            int sig = 0;
            send(sockfd,&sig,4,0);
            data.dataLen = strlen("Login");
            strcpy(data.buf,"Login");
            send(sockfd,&data,4+data.dataLen,0);
            data.dataLen =strlen(username);
            strcpy(data.buf,username);
            send(sockfd,&data,4+data.dataLen,0);
            recv(sockfd,&dataLen,4,0);
            recv(sockfd,token,dataLen,0);
            printf("%s\n",token);
            int num;
            recv(sockfd,&num,4,0);
            *curdirnum = num;
            printf("%d\n",num);
        }else{
            printf("Password is wrong!\n");
            printf("Do you want to login again?(Y(y)/N(n)):");
            scanf("%c",&tmp);
            getchar();
            if(tmp == 'Y'|| tmp == 'y'){
                int sig =1;
                send(sockfd,&sig,4,0);
                goto label;
            }else{
                return -1;
            }
        }
        return 0;
    }
}
