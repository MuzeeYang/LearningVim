#ifndef __SKICRC_H__
#define __SKICRC_H__
 
#include "utypes.h"

//#define _CRC_CCITT
//#define _CRC_16
#define _CRC_32

#ifdef _CRC_CCITT
#define POLYNOMIAL          0x8408//0x1021
#define INITIAL_REMAINDER   0xFFFF
#define FINAL_XOR_VALUE     0x0000
typedef uint16 width_t;
#define VERIFY_RESULT		0x0000
#endif

#ifdef _CRC_16
#define POLYNOMIAL          0xA001//0x8005
#define INITIAL_REMAINDER   0x0000
#define FINAL_XOR_VALUE     0x0000
typedef uint16 width_t;
#define VERIFY_RESULT		0x0000
#endif

#ifdef _CRC_32
#define POLYNOMIAL          0xEDB88320//0x04C11DB7
#define INITIAL_REMAINDER   0xFFFFFFFF
#define FINAL_XOR_VALUE     0xFFFFFFFF
typedef uint32 width_t;
#define VERIFY_RESULT		0x2144DF1C
#endif

 
void crcInit(void);
width_t crcCompute(uint8* data, uint32 length, width_t initVal);

width_t* crcGetTable();
void crcReverse();
#endif // skiCRC.h
