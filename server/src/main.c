#include "func.h"
#include "factory.h"
int epollAdd(int epfd,int fd){
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = fd;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD,fd,&event);
    ERROR_CHECK(ret,-1,"epoll_ctl");
    return 0;
}
int main(int argc,char* argv[])
{
    //-----------配置文件获取IP和PORT----------
    ARGS_CHECK(argc,2);
    char* ip = (char*)calloc(20,sizeof(char));
    char* port = (char*)calloc(6,sizeof(char));
    FILE* fp = fopen(argv[1],"rb+");
    ERROR_CHECK(fp,NULL,"fopen");
    int ret = fscanf(fp,"%s",ip);
    if(ret < 0){
        perror("fscanf");
        return -1;
    }
    printf("%s\n",ip);
    ret = fscanf(fp,"%s",port);
    if(ret < 0){
        perror("fscanf");
    }
    printf("%s\n",port);
    int threadNum ,capacity;
    ret = fscanf(fp,"%d",&threadNum);
    if(ret < 0){
        perror("fscanf");
        return -1;
    }
    ret = fscanf(fp,"%d",&capacity);
    if(ret < 0){
        perror("fscanf");
        return -1;
    }
    //---------------连接与通信------------
    int socketFd;
    Factory_t threadMainData;
    factoryInit(&threadMainData,threadNum,capacity);
    factoryStart(&threadMainData);
    ret = tcp_init(&socketFd,ip,port);
    ERROR_CHECK(ret,-1,"tcp_init");
    int newFd;
    pQue_t pq = &threadMainData.que;
    pNode_t pNew;
    int epfd = epoll_create(1);
    struct epoll_event evs[10];
    epollAdd(epfd,socketFd);  
    int readyFdCount,i; 
    while(1){
        readyFdCount = epoll_wait(epfd,evs,10,-1);  
        //检查是否有可读事件
        for(i = 0; i < readyFdCount; ++i){
            if(evs[i].data.fd == socketFd){
                newFd = accept(socketFd,NULL,NULL);
                pNew = (pNode_t)calloc(1,sizeof(Node_t));
                pNew->newFd = newFd;
                pthread_mutex_lock(&pq->mutex);
                queInsert(pq,pNew);
                pthread_mutex_unlock(&pq->mutex);
                pthread_cond_signal(&threadMainData.cond);
            }
        }
    }
    return 0;
}

