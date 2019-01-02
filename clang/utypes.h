#ifndef __UTYPES_H__
#define __UTYPES_H__

#define _LOG_DEBUG
#ifdef _LOG_DEBUG
	#define printf_debug(fmt, ...) do{printf("[%s][%d]"fmt, __func__, __LINE__, ##__VA_ARGS__);}while(0)
	#define GET_TIME_SEC() ((double)(clock())/CLOCKS_PER_SEC)
#else
	#define printf_debug(fmt, ...)
#endif

#ifndef uint32
typedef unsigned int uint32;
#endif

#ifndef uint16
typedef unsigned short uint16;
#endif

#ifndef uint8
typedef unsigned char uint8;
#endif

#ifndef sint32
typedef signed int sint32;
#endif

#ifndef sint16
typedef signed short sint16;
#endif

#ifndef sint8
typedef signed char sint8;
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#endif //utypes.h
