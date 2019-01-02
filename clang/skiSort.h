#ifndef __COMSORT_H__
#define __COMSORT_H__

typedef uint32 T;

#define SWAP_XOR(x,y) do{if((x) != (y))(x)=(x)^(y), (y)=(x)^(y), (x)=(x)^(y);}while(0)
#define SWAP_TMP(x,y) do{typeof(x) _x = x; x = y; y = _x;}while(0)

#define SWAP(x,y) SWAP_TMP(x,y)

void reverseArr(T*, uint32);
void bubbleSort(T*, uint32);
void shellSort(T* , uint32);
void quickSort(T*, uint32);

//=====================================

#define GET_PARENT(ch) (((ch)-1)>>1)
#define GET_LEFTCHLD(par) (((par)<<1) + 1)
#define GET_RGHTCHLD(par) (((par)<<1) + 2)

void heapSort(T*, uint32);
static void _heapAjust(T*, uint32, uint32);

//=====================================
//#define __LISTSORT__
#ifdef __LISTSORT__

#include "listTrain.h"

typedef struct sortNode
{
	T data;
	ListTrain train;
}SortNode;
#define STRUCT_OF(addr) CONTAINER_OF(addr, SortNode, train)

DECLARE_HEAD(head);

void listSort(PTrainHead);
static void _addTreeNode(PTrainNode, PTrainNode)
static void _getListFrmTree(PTrainHead, PTrainHead);

#endif //__LISTSORT__

#endif //comSort.h
