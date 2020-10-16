#include "string.h"
#include "stdint.h"
#include "services.h"
#include "stm32h7xx_hal.h"
#include "crc.h"
#include "userstruct.h"
#include "usercmd.h"
#include "time.h"

#define 	COM_DATA_LENGTH  17

extern uint8_t ModbusRxData[RXBUFF_LENGTH];
extern uint8_t ModbusTxData[TXBUFF_LENGTH];
extern uint16_t ComDatabuffer[COM_DATA_LENGTH];
extern struct ModbusRegister DataBuffer[18];
extern uint16_t  TemptureThreshold; 									//电机温度阈值，超过需要报警,设置默认温度阈值为50℃
extern UART_HandleTypeDef huart1;


//===============================================================
//  函数名称  ：  Host03Service()
//  功能描述  ：  
//  入口参数  ：  无
//  返回数据  ：  无
//  修改记录  ：  暂无
//================================================================
void Host03Service(void)
{
    uint8_t   DataLength;
    uint16_t  TempData;
    uint8_t   i;
	union WordtoBytes CRCData;
	union WordtoBytes TempForTxBuffer;
	
    DataLength  =  0;
//-------------------------------------------
	ModbusTxData[DataLength]  =  MASTER_BASE_ADD;                   //响应地址
    DataLength++;
    ModbusTxData[DataLength]  =  RTU_R03_CMD;                       //功能码
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_HI];      
    DataLength++;
	ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_LO];
	DataLength++;
	TempForTxBuffer.Bytes.LSB  =  ModbusRxData[RTU_PAR_ADD_LO];		//需要读取的寄存器地址的低字节
	TempForTxBuffer.Bytes.MSB  =  ModbusRxData[RTU_PAR_ADD_HI];		//需要读取的寄存器地址的高字节
	TempData = TempForTxBuffer.Word - RTU_REG_BASE; 				//
    for(i = 0; i < ModbusRxData[RTU_PAR_NUM_LO]; i++)				//循环将数据写入发送缓冲区
    {
     	 ModbusTxData[DataLength] = DataBuffer[TempData + i].RegisterData.Bytes.MSB ;
      	 DataLength++;
       	 ModbusTxData[DataLength] = DataBuffer[TempData + i].RegisterData.Bytes.LSB;
       	 DataLength++;
    }    
    CRCData.Word=CRC16Bit(ModbusTxData, DataLength);
    ModbusTxData[DataLength]   =  CRCData.Bytes.LSB;  
    DataLength++;
    ModbusTxData[DataLength]   =  CRCData.Bytes.MSB; 
    DataLength++;
/*通过串口发送数据*/
	HAL_UART_Transmit(&huart1,ModbusTxData,26,1000);
    memset(ModbusTxData,0x00,DataLength);
}
//===============================================================
//  函数名称  ：  Host06Service()
//  功能描述  ：  
//  入口参数  ：  无
//  返回数据  ：  无
//  修改记录  ：  暂无
//================================================================
void Host06Service(void)
{
    uint8_t   DataLength;
	union WordtoBytes CRCData;
	
	
    DataLength  =  0;
//-------------------------------------------
	ModbusTxData[DataLength]  =  MASTER_BASE_ADD;
    DataLength++;
    ModbusTxData[DataLength]  =  RTU_W06_CMD;
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_ADD_HI];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_ADD_LO];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_HI];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_LO];
    DataLength++;

    CRCData.Word=CRC16Bit(ModbusTxData, DataLength);
    ModbusTxData[DataLength]   =  CRCData.Bytes.LSB;  
    DataLength++;
    ModbusTxData[DataLength]   =  CRCData.Bytes.MSB; 
    DataLength++;
//------------------------------------------------
	HAL_UART_Transmit(&huart1,ModbusTxData,DataLength,100); 
}
//===============================================================
//  函数名称  ：  Host10Service()
//  功能描述  ：  
//  入口参数  ：  无
//  返回数据  ：  无
//  修改记录  ：  暂无
//================================================================
void Host10Service(void)
{
    uint8_t   DataLength;
//    uint16_t  tempdata;
//    uint8_t   i;
//    uint8_t   j;
	union WordtoBytes CRCData;
    DataLength  =  0;
//-------------------------------------------
    ModbusTxData[DataLength]   =  RTU_START_BYTE;
	DataLength++;
	ModbusTxData[DataLength]  =  MASTER_BASE_ADD;
    DataLength++;
    ModbusTxData[DataLength]  =  RTU_W10_CMD;
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_ADD_HI];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_ADD_LO];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_HI];
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_LO];
    DataLength++;
    
    CRCData.Word=CRC16Bit(ModbusTxData, DataLength);
    ModbusTxData[DataLength]   =  CRCData.Bytes.LSB;  
    DataLength++;
    ModbusTxData[DataLength]   =  CRCData.Bytes.MSB; 
    DataLength++;
	ModbusTxData[DataLength]   =  RTU_END_BYTE_HI;
	DataLength++;
	ModbusTxData[DataLength]   =  RTU_END_BYTE_LO;
	DataLength++;
//------------------------------------------------
	HAL_UART_Transmit(&huart1,ModbusTxData,DataLength,100);
}

