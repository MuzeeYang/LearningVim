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

//===============static function==================
