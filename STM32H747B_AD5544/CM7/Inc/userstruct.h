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

union LongtoBytes
{
    uint32_t LongData;
    uint8_t  Bytes[4];
};

struct ModbusRegister
{
	union WordtoBytes RegisterAddress;
	union WordtoBytes RegisterData;
};

#endif
