#include "sys/time.h"
#include "stdarg.h"
#include "skiEvent.h"

int msleep(unsigned int msec)
{
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = msec * 1000 - msec % 1024;
	select(0, NULL, NULL, NULL, &tv);
}

int getmseconds()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000 % 1000);
}

static int _timeoutDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc <= 2){
		goto FUNC_ARGS_FAILED;
	}
	unsigned int delay = (int)argv[0];
	int (*cb)() = argv[1];
	void** cbArgs = &argv[2];

	msleep(delay);

	ret = cb(argc-2, cbArgs);

FUNC_ARGS_FAILED:
	return ret;
}

int setTimeout(unsigned int delay, int (*cb)(), int argc, ...)
{
	va_list ap;
	void* argv[argc = (argc < 0? 0: argc+2)];

	argv[0] = (void*)delay;
	argv[1] = cb;

	va_start(ap, argc);
	int i;
	for(i = 2; i < argc; i++){
		argv[i] = va_arg(ap, void*);
	}
	va_end(ap);
	return setThreadCallbackRaw(_timeoutDecorator, sizeof(argv), argv);
}

static int _intervalDecorator(int argc, void** argv)
{
	int ret = -1;
	if(argc <= 2){
		goto FUNC_ARGS_FAILED;
	}
	unsigned int delay = (int)argv[0];
	int (*cb)() = argv[1];
	void** cbArgs = &argv[2];

	do{
		msleep(delay);
	}while((ret = cb(argc-2, cbArgs)) == 0);

FUNC_ARGS_FAILED:
	return ret;
}

int setInterval(unsigned int delay, int (*cb)(), int argc, ...)
{
	va_list ap;
	void* argv[argc = (argc < 0? 0: argc+2)];

	argv[0] = (void*)delay;
	argv[1] = cb;

	va_start(ap, argc);
	int i;
	for(i = 2; i < argc; i++){
		argv[i] = va_arg(ap, void*);
	}
	va_end(ap);
	return setThreadCallbackRaw(_intervalDecorator, sizeof(argv), argv);
}
