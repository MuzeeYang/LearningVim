#include "skiSort.h"

uint32 searchArr(T *arr, uint32 length, T stand)
{
	int start = -1, end = length, mid;
	while(end-start > 1){
		mid = (start + end) >> 1;
		if(arr[mid] > stand)
			end = mid;
		else if(arr[mid] < stand)
			start = mid;
		else
			break;
	}

	return mid;
}

void reverseArr(T* arr, uint32 length)
{
	uint32 i, j;
	for(i = 0, j = length-1; i < j; i++, j--)
		SWAP(arr[i], arr[j]);
}

void shellSort(T* arr, uint32 length)
{
	uint32 gap;
	int i, j;
	T temp;

	for(gap = length>>1; gap > 0; gap = gap>>1){
		for(i = gap; i < length; i++){
			temp = arr[i];
			for(j = i-gap; j >= 0 && temp < arr[j]; j -= gap)
				arr[j+gap] = arr[j];
			arr[j+gap] = temp;
		}
	}
}

void bubbleSort(T* arr, uint32 length)
{
	uint32 i, j;
	for(i = 0; i < length; i++){
		for(j = i; j > 0; j--){
			if(arr[j-1] > arr[j])
				SWAP(arr[j-1], arr[j]);
		}
	}
}


void quickSort(T* arr, uint32 length)
{
	if(length < 2)
		return;

	uint32 i, j;
	i = 0;
	j = length;

	while(1){
		while(i < j && arr[0] <= arr[--j]);

		while(i < j && arr[++i] <= arr[0]);

		if(i < j)
			SWAP(arr[i], arr[j]);
		else
			break;
	}

	SWAP(arr[0], arr[j]);

	quickSort(arr, j);
	quickSort(arr+j+1, length-j-1);
}


//=======heapSort=======
#define GET_PARENT(ch) (((ch)-1)>>1)
#define GET_LEFTCHLD(par) (((par)<<1) + 1)
#define GET_RGHTCHLD(par) (((par)<<1) + 2)

#ifdef _HEAP_BIGTERM
#define HEAP_CMP(x, y) ((x) < (y))
#else
#define HEAP_CMP(x, y) ((x) > (y))
#endif

static uint32 gHeapSorted = 0;

static void _heapAjust(T* arr, uint32 length, uint32 start)
{
	uint32 par, child;
	for(par = start; (child = GET_LEFTCHLD(par)) < length; par = child){
		if(child+1 < length && HEAP_CMP(arr[child], arr[child+1]))
			child++;

		if(HEAP_CMP(arr[par], arr[child]))
			SWAP(arr[par], arr[child]);
		else
			break;
	}
}

void heapConstruct(T* arr, uint32 length)
{
	uint32 par;
	//construct heap
	for(par = GET_PARENT(length-1); par+1 > 0; par--)
		_heapAjust(arr, length, par);
}

void heapSort(T* arr, uint32 length)
{
	uint32 j;
	gHeapSorted = 0;
	for(j = length-1; j > 0; j--){
		SWAP(arr[0], arr[j]);
		_heapAjust(arr, j, 0);
		gHeapSorted++;
	}
}

uint32 heapGetSorted()
{
	return gHeapSorted;
}

