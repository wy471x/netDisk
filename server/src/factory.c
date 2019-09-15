#include "factory.h"
#include "func.h"
//线程池初始化
int factoryInit(pFactory_t pf,int threadNum,int capacity){
    queInit(&pf->que,capacity);
    pthread_cond_init(&pf->cond,NULL);
    pf->pthid = (pthread_t*)calloc(threadNum,sizeof(pthread_t));
    pf->threadNum = threadNum;
    pf->startFlag = 0;
    return 0;
}
//清理函数
void cleanup(void *p){
  pQue_t pq = (pQue_t)p;
  pthread_mutex_unlock(&pq->mutex);
}
//线程执行函数
void *threadFunc(void *p){
    pFactory_t pf = (pFactory_t)p;
    pQue_t pq = &pf->que;
    pNode_t pCur;
    int getSuccess;
    while(1){
        pthread_mutex_lock(&pq->mutex);
        pthread_cleanup_push(cleanup,pq);
        if(!pq->size){
            pthread_cond_wait(&pf->cond,&pq->mutex);
        }
        getSuccess = queGet(pq,&pCur);
        pthread_cleanup_pop(1);
        if(!getSuccess){
            requestDeal(pCur->newFd);
            free(pCur);
        }
        pCur = NULL;
    }
}
//生成线程池
int factoryStart(pFactory_t pf){
   int i;
   if(!pf->startFlag){
       for(i = 0; i < pf->threadNum;i++){
           pthread_create(pf->pthid+i,NULL,threadFunc,pf);
       }
       pf->startFlag = 1;
   }
   return 0;
}
