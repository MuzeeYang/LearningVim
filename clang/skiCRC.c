#include "skiCRC.h"

static width_t crcTable[256];

width_t* crcGetTable()
{
	return crcTable;
}

void crcReverseBytes(uint8 *data, uint32 length)
{
	uint8 tmp;
	int i, j;
	for(i = 0, j = length-1; i < j; i++, j--)
	{
		tmp = data[i];
		data[i] = data[j];
		data[j] = tmp;
	}
}

width_t crcReverseBits(width_t data)
{
	uint32 len = sizeof(data) * 8;
	width_t res = 0;
	while(len--)
	{
		res <<= 1;
		res |= data & 0x01;
		data >>= 1;
	}
}

void crcInit()  
{
    int i;  
    int j;  
    width_t sum;  
    //width_t sympoly = bitrevs(POLYNOMIAL, sizeof(POLYNOMIAL));  

    for(i=0; i<256; i++)
    {  
        sum = i;  
        for(j=0; j<8; j++)  
        {  
            if(sum & 1)  
            {  
                sum = POLYNOMIAL ^ (sum >> 1);  
            }  
            else  
            {  
                sum >>= 1;
            }  
        }  
        crcTable[i] = sum;  
    }  
}

width_t crcCompute(uint8* data, uint32 length, width_t initVal)
{
	width_t reg = initVal;
	reg ^= INITIAL_REMAINDER;
	while(length--)
	{
		reg = (reg>>8) ^ crcTable[(reg^*data++) & 0xff];
	}

	return (reg ^ FINAL_XOR_VALUE);
}
