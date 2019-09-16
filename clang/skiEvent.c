#include "sys/time.h"
#include "string.h"
#include "malloc.h"
#include "pthread.h"
#include "utypes.h"
#include "skiList.h"
#include "skiEvent.h"

typedef struct _eventList{
	TTrainNode list;
	int nice;
	int (*cb)();
	int argsize;
	char args[0];
}TThreadEvent, *PThreadEvent;

typedef struct _arg32{char buf[32];}TArgs32;
typedef struct _arg64{char buf[64];}TArgs64;
typedef struct _arg128{char buf[128];}TArgs128;
typedef struct _arg256{char buf[256];}TArgs256;

static PTrainNode pEventHead = NULL;
static pthread_mutex_t tpMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tpCond = PTHREAD_COND_INITIALIZER;

static PThreadEvent getThreadCallback()
{
	PTrainNode shiftNode = NULL;
	PThreadEvent event = NULL;

	pthread_mutex_lock(&tpMutex);
	while(pEventHead && IS_EMPTY_LIST(pEventHead)){
		pthread_cond_wait(&tpCond, &tpMutex);
	}
	if(pEventHead)shiftNode = shiftList(pEventHead);
	pthread_mutex_unlock(&tpMutex);

	if(shiftNode){
		event = CONTAINER_OF(shiftNode, TThreadEvent, list);
	}

	return event;
}

int setThreadCallbackRaw(int (*cb)(), int argsize, void* args)
{
	PThreadEvent pEvent = NULL;

	if((pEvent = malloc(sizeof(TThreadEvent) + argsize)) == NULL)return -1;
	memset(pEvent, 0, sizeof(TThreadEvent) + argsize);

	pEvent->cb = cb;
	pEvent->argsize = argsize;
	memcpy(pEvent->args, args, argsize);

	pthread_mutex_lock(&tpMutex);
	pushList(pEventHead, &pEvent->list);
	pthread_mutex_unlock(&tpMutex);
	pthread_cond_signal(&tpCond);

	return 0;
}

static int _getArgSize(char* argfmt)
{
	int argsize = 0;

	while(argfmt && *argfmt){
		switch(*argfmt++){
			case 's':
			case 'p':
				argsize += sizeof(void*);
				break;
			case 'd':
			case 'i':
			case 'u':
			case 'c':
				argsize += sizeof(int);
				break;
			case 'f':
			case 'g':
				argsize += sizeof(double);
				break;
		}
	}
	return argsize;
}

int setThreadCallback(int (*cb)(), char* argfmt, ...)
{
	PThreadEvent pEvent = NULL;
	int argsize = _getArgSize(argfmt);

	/*
	if((pEvent = malloc(sizeof(TThreadEvent) + argsize)) == NULL)return -1;
	memset(pEvent, 0, sizeof(TThreadEvent) + argsize);

	pEvent->cb = cb;
	pEvent->argsize = argsize;
	memcpy(pEvent->args, &argfmt + 1, argsize);

	pthread_mutex_lock(&tpMutex);
	pushList(pEventHead, &pEvent->list);
	pthread_mutex_unlock(&tpMutex);
	pthread_cond_signal(&tpCond);
	*/

	return setThreadCallbackRaw(cb, argsize, &argfmt + 1);
}

static void* threadFunc(void* nothing)
{
	PThreadEvent pEvent = NULL;
	while(pEventHead){
		if((pEvent = getThreadCallback()) == NULL)continue;

		if(pEvent->argsize == 0)
			pEvent->cb();
		else if(pEvent->argsize <= 32)
			pEvent->cb(*((TArgs32*)pEvent->args));
		else if(pEvent->argsize <= 64)
			pEvent->cb(*((TArgs64*)pEvent->args));
		else if(pEvent->argsize <= 128)
			pEvent->cb(*((TArgs128*)pEvent->args));
		else
			pEvent->cb(*((TArgs256*)pEvent->args));

	}
	pthread_detach(pthread_self());
	return NULL;
}

int initThreadPool(int threadNum)
{
	int i = 0;
	pthread_t tid = 0;
	if((pEventHead = malloc(sizeof(TTrainNode))) == NULL){
		goto HEAD_MALLOC_FAILED;
	}

	pEventHead->prev = pEventHead->next = pEventHead;
	for(i = 0; i < threadNum; i++){
		if(pthread_create(&tid, NULL, threadFunc, pEventHead)){
			break;
		}
	}

HEAD_MALLOC_FAILED:
	return i;
}

int freeThreadPool()
{
	PTrainNode cursor = NULL;
	PThreadEvent event = NULL;
	pthread_mutex_lock(&tpMutex);
	FOR_EACH(pEventHead, cursor){
		event = CONTAINER_OF(cursor, TThreadEvent, list);
		free(event);
	}
	free(pEventHead);
	pEventHead = NULL;
	pthread_mutex_unlock(&tpMutex);
	pthread_cond_broadcast(&tpCond);

	return 0;
}

int isThreadPool()
{
	return !(pEventHead);
}

int msleep(unsigned int msec)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = msec * 1000 - msec % 1024;
	return select(0, NULL, NULL, NULL, &tv);
}

unsigned int getmseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000 % 1000);
}

