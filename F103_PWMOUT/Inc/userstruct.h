#ifndef __USERSTRUCT_H_
#define __USERSTRUCT_H_

#include "stdint.h"

union  WordtoBytes
{
	uint16_t Word;
	struct Bytes
	{
		uint8_t LSB;
		uint8_t MSB;
	}Bytes;
};

union Short2Byte
{
	uint32_t  ShortData;
	uint8_t   ByteData[4];
};


struct ModbusRegister
{
	union WordtoBytes RegisterAddress;
	union WordtoBytes RegisterData;
};

#endif
