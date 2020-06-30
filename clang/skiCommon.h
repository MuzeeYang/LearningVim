#ifndef __SKICOMMON_H__
#define __SKICOMMON_H__

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

#define _name_var(name) #name
#define print_var(t, var) printf("[%s:%s:%d] %s(%p) = "_name_var(t)"\n", __FILE__, __func__, __LINE__, _name_var(var), &(var), (var))

#define SWAP_XOR(x,y) do{if((x) != (y))(x)=(x)^(y), (y)=(x)^(y), (x)=(x)^(y);}while(0)
#define SWAP_TMP(x,y) do{typeof(x) _x = x; x = y; y = _x;}while(0)

#define CONTAINER_OF(addr, type, name) ((type*)((unsigned long)addr - (unsigned long)(&((type*)0)->name)))

#endif //skiCommon.h
