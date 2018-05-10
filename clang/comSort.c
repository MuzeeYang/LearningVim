#include "comSort.h"

void shellSort(T* arr, uint length)
{
	uint gap;
	int i, j;
	T temp;

	for(gap = length>>1; gap > 0; gap = gap>>1)
	{
		for(i = gap; i < length; i++)
		{
			temp = arr[i];
			for(j = i-gap; j >= 0 && temp < arr[j]; j -= gap)
			{
				arr[j+gap] = arr[j];
			}
			arr[j+gap] = temp;
		}
	}
}

void bubbleSort(T* arr, uint length)
{
	uint i, j;
	for(i = 0; i < length; i++)
	{
		for(j = i; j > 0; j--)
		{
			if(arr[j-1] > arr[j])
			{
				SWAP(arr[j-1], arr[j]);
			}
		}
	}
}


void quickSort(T* arr, uint length)
{
	if(length < 2)
		return;

	uint i, j;
	i = 0;
	j = length;

	while(1)
	{
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


void heapSort(T* arr, uint length)
{
	uint j;
	uint par;
	//construct heap
	for(par = GET_PARENT(length-1); par+1 > 0; par--)
	{
		_heapAjust(arr, length, par);
	}

	for(j = length-1; j > 0; j--)
	{
		SWAP(arr[0], arr[j]);
		_heapAjust(arr, j, 0);
	}
}

//===============static function==================

static void _heapAjust(T* arr, uint length, uint start)
{
	uint par, child;
	for(par = start; (child = GET_LEFTCHLD(par)) < length; par = child)
	{
		if(child+1 < length && arr[child] < arr[child+1])
			child++;

		if(arr[par] < arr[child])
			SWAP(arr[par], arr[child]);
		else
			break;
	}
}
