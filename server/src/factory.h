#pragma once
#include "head.h"
#include "work_que.h"
typedef struct {
    Que_t que;//任务队列
    pthread_cond_t cond;//条件变量
    pthread_t *pthid;
    int threadNum;      //线程数
    int startFlag;      //开始标志
}Factory_t ,*pFactory_t;
int factoryInit(pFactory_t,int,int);
int factoryStart(pFactory_t);
int tcpInit(int*,char*,char*);
