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

#endif //skiList.h
