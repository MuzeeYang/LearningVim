#ifndef __SKITIME_H__
#define __SKITIME_H__

int msleep(unsigned int msec);
int getmseconds();
int setTimeout(unsigned int delay, int (*cb)(), int argc, ...);
int setInterval(unsigned int delay, int (*cb)(), int argc, ...);

#endif //skiTime.h
