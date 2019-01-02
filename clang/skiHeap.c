#include "skiHeap.h"

int heapInit(pHEAP pHeap, uint32 size, uint32 heapTerm, HEAP_CMPFUNC func)
{
	pHeap->heapHead = malloc(size * sizeof(T));
	if(!pHeap->heapHead)
		return -1;
	pHeap->size = size;
	pHeap->heapTerm = heapTerm;
	pHeap->heapCmp = func;
	pHeap->length = 0;
	pHeap->sorted = 0;
	return 0;
}

void heapConstruct(pHEAP pHeap, T* arr, uint32 length)
{
	uint32 par;
	memcpy(pHeap->heapHead, arr, length * sizeof(T));
	pHeap->length = length;

	for(par = GET_PARENT(length-1); par+1 > 0; par--)
	{
		_heapAjust(pHeap, pHeap->heapHead, length, par);
	}
}

void heapDestroy(pHEAP pHeap)
{
	if(pHeap->heapHead)
	{
		free(pHeap->heapHead);
		pHeap->heapHead = NULL;
	}
}

T heapPop(pHEAP pHeap)
{
	if(pHeap->length > 0)
	{
		pHeap->length--;
		if(pHeap->sorted == 0)
		{
			_swap(pHeap->heapHead[0], pHeap->heapHead[pHeap->length]);
			_heapAjust(pHeap, pHead->heapHead, pHead->length, 0);
		}
		return pHeap->heapHead[pHeap->length];
	}
	else
		return (T)0;
}

void heapPush(pHEAP pHeap, T data)
{
	if(pHeap->sorted == 0)
		_heapSort(pHeap);

	if(pHeap->length + 1 <= pHeap->size)
	{
		pHeap->heapHead[pHeap->length] = data;
		_swap(pHeap->heapHead[0], pHeap->heapHead[pHeap->length]);
		_heapAjust(pHeap, pHead->heapHead, pHead->length, 0);
		pHeap->length++;
	}
}

static int _compare(pHEAP pHeap, T a, T b)
{
	if(BIG_TERM == pHeap->heapTerm)
	{
		if(pHeap->heapCmp)
			return (pHeap->heapCmp(a, b) < 0);
		else
			return (a < b);
	}
	else
	{
		if(pHeap->heapCmp)
			return (pHeap->heapCmp(a, b) > 0);
		else
			return (a > b);
	}
}

static void _swap(T *a, T *b)
{
	T tmp = *a;
	*a = *b;
	*b = tmp;
}

static void _heapSort(pHEAP pHeap)
{
	uint32 j;

	for(j = pHeap->length-1; j > 0; j--)
	{
		_swap(pHeap->heapHead[0], pHeap->heapHead[j]);
		_heapAjust(pHeap, pHead->heapHead, j, 0);
	}
	pHeap->sorted = pHeap->length;
}

static void _heapAjust(pHEAP pHeap, T* arr, uint32 length, uint32 start)
{
	uint32 par, child;
	for(par = start; (child = GET_LEFTCHLD(par)) < length; par = child)
	{
		if(child+1 < length && _compare(pHeap, arr[child], arr[child+1]))
			child++;

		if(_compare(pHeap, arr[par], arr[child]))
			_swap(arr[par], arr[child]);
		else
			break;
	}
}
