#ifndef __SKI_HEAP_H__
#define __SKI_HEAP_H__

#include "utypes.h"

typedef uint32 T;
typedef int (*HEAP_CMPFUNC)(T, T);

typedef struct{
	T *heapHead;
	uint32 size;
	uint32 length;
	uint32 sorted;
	uint32 heapTerm;
	HEAP_CMPFUNC heapCmp;
}tHEAP, *pHEAP;

#define BIG_TERM (0)
#define LITTLE_TERM (1)

#define GET_PARENT(ch) (((ch)-1)>>1)
#define GET_LEFTCHLD(par) (((par)<<1) + 1)
#define GET_RGHTCHLD(par) (((par)<<1) + 2)


#define SWAP_XOR(x,y) do{if((x) != (y))(x)=(x)^(y), (y)=(x)^(y), (x)=(x)^(y);}while(0)
#define SWAP_TMP(x,y) do{typeof(x) _x = x; x = y; y = _x;}while(0)

#define SWAP(x,y) SWAP_TMP(x,y)

#endif //skiHeap.h
