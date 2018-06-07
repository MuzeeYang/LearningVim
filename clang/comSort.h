#ifndef __COMSORT_H__
#define __COMSORT_H__

typedef int T;
typedef unsigned int uint;

#define SWAP_XOR(x,y) do{x = x^y; y = x^y; x = x^y;}while(0)
#define SWAP_TYPEOF(x,y) do{typeof(x) temp = x; x = y; y = temp;}while(0)

#define SWAP(x,y) SWAP_TYPEOF(x,y)

void bubbleSort(T*, uint);
void shellSort(T* , uint);
void quickSort(T*, uint);

//=====================================

#define GET_PARENT(ch) (((ch)-1)>>1)
#define GET_LEFTCHLD(par) (((par)<<1) + 1)
#define GET_RGHTCHLD(par) (((par)<<1) + 2)

void heapSort(T*, uint);
static void _heapAjust(T*, uint, uint);

//=====================================
//#define __LISTSORT__
#ifdef __LISTSORT__

#include "listTrain.h"

typedef struct sortNode
{
	T data;
	ListTrain train;
}SortNode;
#define STRUCT_OF(addr) CONTANOR_OF(addr, SortNode, train)

DECLARE_HEAD(head);

void listSort(PTrainHead);
static void _addTreeNode(PTrainNode, PTrainNode)
static void _getListFrmTree(PTrainHead, PTrainHead);

#endif //__LISTSORT__

#endif //comSort.h
