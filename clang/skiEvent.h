#ifndef __SKIEVENT_H__
#define __SKIEVENT_H__

int initThreadPool(int threadNum);
int freeThreadPool();
int setThreadCallback(int (*cb)(), char* argfmt, ...);
int setThreadCallbackRaw(int (*cb)(), int argsize, void* args);
int isThreadPool();
int msleep(unsigned int msec);
unsigned int getmseconds();

#endif //skiList.h
