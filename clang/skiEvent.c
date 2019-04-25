#include "stdlib.h"
#include "stdarg.h"
#include "string.h"
#include "utypes.h"
#include "skiEvent.h"

static PTrainNode pEventHead = NULL;
static pthread_mutex_t tpMutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t tpCond = PTHREAD_COND_INITIALIZER;

static PCBStruct getThreadCallback()
{
	PTrainNode shiftNode = NULL;
	PThreadEvent event = NULL;
	PCBStruct cbStruct = NULL;

	pthread_mutex_lock(&tpMutex);
	while(IS_EMPTY_LIST(pEventHead)){
		pthread_cond_wait(&tpCond, &tpMutex);
		if(pEventHead == NULL){
			pthread_mutex_unlock(&tpMutex);
			return NULL;
		}
	}
	shiftNode = shiftList(pEventHead);
	pthread_mutex_unlock(&tpMutex);

	if(shiftNode){
		event = CONTAINER_OF(shiftNode, TThreadEvent, list);
		cbStruct = event->cb;
		free(event);
		event = NULL;
	}
	return cbStruct;
}


int setThreadCallbackRaw(int (*cb)(), int argc, void** argv)
{
	PCBStruct newCallBack = NULL;
	PThreadEvent newEvent = NULL;
	if(newCallBack = malloc(sizeof(TCBStruct) + sizeof(void*)*argc)){
		newCallBack->callback = cb;
		newCallBack->argc = argc;
		memcpy(newCallBack->argv, argv, sizeof(void*)*argc);
	}else{
		goto CALLBACK_MALLOC_FAILED;
	}

	if(newEvent = malloc(sizeof(TThreadEvent))){
		newEvent->cb = newCallBack;
		newEvent->nice = 0;
		pthread_mutex_lock(&tpMutex);
		pushList(pEventHead, &newEvent->list);
		pthread_mutex_unlock(&tpMutex);
		pthread_cond_signal(&tpCond);
		//pthread_cond_broadcast(&tpCond);
	}else{
		goto EVENT_MALLOC_FAILED;
	}

	return 0;
EVENT_MALLOC_FAILED:
	free(newCallBack);
CALLBACK_MALLOC_FAILED:
	return -1;
}

int setThreadCallback(int (*cb)(), int argc, ...)
{
	va_list ap;
	//if(argc < 0)argc = 0;
	void* argv[argc = (argc < 0? 0: argc)];

	va_start(ap, argc);
	int i;
	for(i = 0; i < argc; i++){
		argv[i] = va_arg(ap, void*);
	}
	va_end(ap);
	return setThreadCallbackRaw(cb, argc, argv);
}

static void* threadFunc(void* nothing)
{
	PCBStruct cbStruct = NULL;
	while(pEventHead){
		if(cbStruct = getThreadCallback()){
			cbStruct->callback(cbStruct->argc, cbStruct->argv);
			free(cbStruct);
			cbStruct = NULL;
		}
	}
	pthread_detach(pthread_self());
	return NULL;
}

int initThreadPool(int threadNum)
{
	int i = 0;
	pthread_t tid = 0;
	if(pEventHead = malloc(sizeof(TTrainNode))){
		pEventHead->prev = pEventHead->next = pEventHead;
		for(i = 0; i < threadNum; i++){
			if(pthread_create(&tid, NULL, threadFunc, pEventHead)){
				break;
			}
		}
	}
	return i;
}

int freeThreadPool()
{
	PTrainNode cursor = NULL;
	PThreadEvent event = NULL;
	pthread_mutex_lock(&tpMutex);
	FOR_EACH(pEventHead, cursor){
		event = CONTAINER_OF(cursor, TThreadEvent, list);
		free(event->cb);
		event->cb = NULL;
		free(event);
		event = NULL;
	}
	free(pEventHead);
	pEventHead = NULL;
	pthread_mutex_unlock(&tpMutex);
	pthread_cond_broadcast(&tpCond);

	return 0;
}

int isThreadPool()
{
	return !IS_EMPTY_LIST(pEventHead);
}

void* initTC()
{
	PThreadCon pTC = NULL;
	if((pTC = malloc(sizeof(TThreadCon))) == NULL){
		goto CTRLLER_MALLOC_FAILED;
	}

	pTC->send = NULL;
	pTC->recv = NULL;
	pthread_mutex_init(&pTC->mtx, NULL);
	pthread_cond_init(&pTC->cnd, NULL);

	return pTC;

CTRLLER_MALLOC_FAILED:
	return NULL;
}

int freeTC(PThreadCon pTC)
{
	pthread_mutex_lock(&pTC->mtx);
	pTC->send = NULL;
	pTC->recv = NULL;
	pthread_mutex_unlock(&pTC->mtx);
	pthread_cond_broadcast(&pTC->cnd);

	pthread_cond_destroy(&pTC->cnd);
	pthread_mutex_destroy(&pTC->mtx);
	free(pTC);
	pTC = NULL;

	return 0;
}

void* waitTC(PThreadCon pTC, void* recv)
{
	void* ret = NULL;
	pthread_mutex_lock(&pTC->mtx);
	while(pTC->recv){
		pthread_cond_wait(&pTC->cnd, &pTC->mtx);
	}
	ret = pTC->send;
	pTC->recv = recv;
	pthread_mutex_unlock(&pTC->mtx);
	pthread_cond_broadcast(&pTC->cnd);
	return ret;
}

void* sendTC(PThreadCon pTC, void* send)
{
	void* ret = NULL;
	pthread_mutex_lock(&pTC->mtx);
	while(!pTC->recv){
		pthread_cond_wait(&pTC->cnd, &pTC->mtx);
	}
	ret = pTC->recv;
	pTC->recv = NULL;
	pTC->send = send;
	pthread_mutex_unlock(&pTC->mtx);
	pthread_cond_signal(&pTC->cnd);
	return ret;
}
