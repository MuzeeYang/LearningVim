#include "stdio.h"
#include "stdlib.h"
#include "time.h"

#define ARC_SIZE 4
#define PUT_CANDY_NUM 8
#define ARC_SCALE 1000000
#define SWAP(x, y) do{typeof(x) _x = x; x = y; y = _x;}while(0)

int printARC(int arc[][ARC_SIZE])
{
	int i, j;
	for(i = 0; i < ARC_SIZE; i++){
		for(j = 0; j < ARC_SIZE; j++){
			printf("%d ", arc[i][j]);
		}
		putchar(10);
	}
	putchar(10);
	return 0;
}

int randomARC(int arc[][ARC_SIZE])
{
	int i, j;
	for(i = 0; i < ARC_SIZE * ARC_SIZE; i++){
		j = rand() % (ARC_SIZE*ARC_SIZE);
		SWAP(arc[0][i], arc[0][j]);
	}

	for(i = 0; i < ARC_SIZE * ARC_SIZE; i++){
		if(arc[0][i] > PUT_CANDY_NUM)arc[0][i] = 0;
		else arc[0][i] = 1;
	}
}

int identifyARC(int arc[][ARC_SIZE])
{
	int count = 0;
	int line = 0, colum = 0;
	int i, j;
	for(i = 0; i < ARC_SIZE; i++){
		line = 0, colum = 0;
		for(j = 0; j < ARC_SIZE; j++){
			line += arc[i][j];
			colum += arc[j][i];
		}
		if(line == ARC_SIZE)count++;
		if(colum == ARC_SIZE)count++;
	}

	for(i = 0, j = 0; i < ARC_SIZE || j < ARC_SIZE; i++, j++){
		if(arc[i][j] == 0)break;
	}
	if(i == ARC_SIZE && j == ARC_SIZE)count++;

	for(i = 0, j = ARC_SIZE; i < ARC_SIZE || j > 0; i++, j--){
		if(arc[i][j-1] == 0)break;
	}
	if(i == ARC_SIZE && j == 0)count++;

	return count;
}

int main(int argc, char** argv)
{
	int arc[ARC_SIZE][ARC_SIZE];
	int scale = ARC_SCALE;
	int i;
	double rate[4] = {0, 0, 0, 0};
	srand(time(NULL));

	while(scale--){
		for(i = 0; i < ARC_SIZE*ARC_SIZE; i++){
			arc[0][i] = i + 1;
		}
		randomARC(arc);

		//printARC(arc);
		i = identifyARC(arc);
		//if(i > 2)printARC(arc);// printf("count: %d\n", i);
		rate[i]++;
	}

	printf("======solusion=======\n");
	for(i = 0; i < sizeof(rate)/sizeof(double); i++)
		printf("%d: %.2f%%,\t", i, rate[i]/ARC_SCALE*100);
	putchar(10);

	return 0;
}
