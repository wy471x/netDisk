#include "work_que.h"
//队列初始化
void queInit(pQue_t pq,int capacity){
    memset(pq,0,sizeof(Que_t));
    pq->capacity = capacity;
    pthread_mutex_init(&pq->mutex,NULL);
}
//连接请求添加
void queInsert(pQue_t pq,pNode_t pNew){
    if(NULL == pq->queHead){
        pq->queHead = pNew;
        pq->queTail = pNew;
    }else{
        pq->queTail->pNext = pNew;
        pq->queTail = pNew;
    }
    pq->size++;
}
//获取请求队列头部的请求
int queGet(pQue_t pq,pNode_t *pGet){
  *pGet = pq->queHead;
  if(NULL == *pGet){
      return -1;
  }
  pq->queHead = pq->queHead->pNext;
  if(NULL == pq->queHead){
    pq->queTail = NULL;
  }
  pq->size--;
  return 0;
}

