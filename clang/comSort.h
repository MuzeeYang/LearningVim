#ifndef __COMSORT_H__
#define __COMSORT_H__

typedef int T;
typedef unsigned int uint;

#define GET_PARENT(ch) (((ch)-1)>>1)
#define GET_LEFTCHLD(par) (((par)<<1) + 1)
#define GET_RGHTCHLD(par) (((par)<<1) + 2)

#define SWAP_XOR(x,y) do{x = x^y; y = x^y; x = x^y;}while(0)
#define SWAP_TYPEOF(x,y) do{typeof(x) temp = x; x = y; y = temp;}while(0)

#define SWAP(x,y) SWAP_TYPEOF(x,y)

void bubbleSort(T*, uint);
void shellSort(T* , uint);
void quickSort(T*, uint);
void heapSort(T*, uint);

//static function
static void _heapAjust(T*, uint, uint);

#endif //comSort.h
