#include "func.h"
//#define del_blank(pos,str){ while(str[pos]!='\0'&&(str[pos] == ' '|| str[pos] == '\t')){ pos++; }}
//#define get_args(argv,pos,str){ int j = 0; while(str[pos]!='\0'&&str[pos]!='\t'&&str[pos]!=' ') argv[j++] = str[pos++];}
#define strLen 100
//连接初始化
int tcp_init(int* sFd,char* ip,char* port){
  int socketFd; 
  socketFd = socket(AF_INET,SOCK_STREAM,0);
  ERROR_CHECK(socketFd,-1,"socket");
  printf("socketFd = %d\n",socketFd);
  int ret;
  int reuse = 1;
  ret = setsockopt(socketFd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(int));
  ERROR_CHECK(ret,-1,"setsockopt");
  struct sockaddr_in serAddr;
  bzero(&serAddr,sizeof(serAddr));
  serAddr.sin_family = AF_INET;
  serAddr.sin_port = htons(atoi(port));
  serAddr.sin_addr.s_addr = inet_addr(ip);
  ret = bind(socketFd,(struct sockaddr*)&serAddr,sizeof(struct sockaddr));
  ERROR_CHECK(ret,-1,"bind");
  listen(socketFd,10);
  *sFd = socketFd;
  return 0;
}
//命令与参数分离
//int separation(char *str, command_t* cmd){
//     int i = 0,p = 0;
//     del_blank(p,str);
//     while(str[p] != '\0'){
//        if((cmd->argv[i] = (char*)calloc(strLen,sizeof(char))) == NULL){
//                 perror("fail malloc");
//                 return -1;
//         }
//         get_args(cmd->argv[i],p,str);
//         i++;
//         del_blank(p,str);
//     }
//     cmd->argv[i] = NULL;
//     cmd->name = cmd->argv[0];
//     return 0;
//}

