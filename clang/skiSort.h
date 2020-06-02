#ifndef __SKISORT_H__
#define __SKISORT_H__

#include "utypes.h"

typedef uint32 T;

uint32 searchArr(T*, uint32, T);
void reverseArr(T*, uint32);
void bubbleSort(T*, uint32);
void shellSort(T*, uint32);
void quickSort(T*, uint32);

//=====================================

//#define _HEAP_BIGTERM
//#define _HEAP_LITTERM
void heapConstruct(T* arr, uint32 length);
void heapSort(T*, uint32);
uint32 heapGetSorted(void);


#endif //skiSort.h
