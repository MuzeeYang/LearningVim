#ifndef __SKIEVENT_H__
#define __SKIEVENT_H__

#include "skiList.h"

typedef struct _bcStruct{
	int (*callback)();
	int argc;
	void* argv[0];
}TCBStruct, *PCBStruct;

typedef struct _eventList{
	PCBStruct cb;
	int nice;
	TTrainNode list;
}TThreadEvent, *PThreadEvent;


int initThreadPool(int threadNum);
int freeThreadPool();
int setThreadCallbackRaw(int (*cb)(), int argc, void** argv);
int setThreadCallback(int (*cb)(), int argc, ...);
int isThreadPool();

typedef struct _stThrdCon{
	void* send;
	void* recv;
	pthread_mutex_t mtx;
	pthread_cond_t cnd;
}TThreadCon, *PThreadCon;

void* initTC();
int freeTC(PThreadCon pTC);
void* waitTC(PThreadCon pTC, void* recv);
void* sendTC(PThreadCon pTC, void* send);

#endif //skiList.h
