#include "func.h"
#define del_blank(pos,s){ while(s[pos]!='\0'&&(s[pos] == ' '|| s[pos] == '\t')){ pos++; }}
#define get_args(argv,pos,s){ int j = 0; while(s[pos]!='\0'&&s[pos]!='\t'&&s[pos]!=' ') argv[j++] = s[pos++];}
#define strLen 100
#define STR_LEN 10
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
