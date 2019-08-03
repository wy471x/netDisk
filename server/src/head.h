#ifndef __HEAD_H__
#define __HEAD_H__
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<fcntl.h>
#include<signal.h>
#include<pthread.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<time.h>
#include<pwd.h>
#include<sys/time.h>
#include<sys/select.h>
#include<dirent.h>
#include<grp.h>
#include<sys/shm.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<sys/msg.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netdb.h>
#include<sys/epoll.h>
#include<sys/sendfile.h>
#include<aio.h>
#include<mysql/mysql.h>
#define ARGS_CHECK(argc,num){if(argc != num){printf("error args\n"); return -1;}}
#define ERROR_CHECK(ret,retval,funcName){if(ret == retval){perror(funcName);return -1;}}
#define THREAD_ERROR_CHECK(ret,funcName){if(ret != 0 ){printf("%s:%s",funcName,strerror(ret));return -1;}}
#endif
