#include "string.h"
#include "stdint.h"
#include "services.h"
#include "stm32f1xx_hal.h"
#include "crc.h"
#include "userstruct.h"
#include "usercmd.h"
#include "time.h"

extern uint8_t ModbusRxData[RXBUFF_LENGTH];
extern uint8_t ModbusTxData[TXBUFF_LENGTH];
extern struct ModbusRegister DataBuffer[18];
extern uint16_t  TemptureThreshold; 									//����¶���ֵ��������Ҫ����,����Ĭ���¶���ֵΪ50��
extern UART_HandleTypeDef huart2;

void Host01Service(void)
{

}

void Host02Service(void)
{

}
//===============================================================
//  ��������  ��  Host03Service()
//  ��������  ��  
//  ��ڲ���  ��  ��
//  ��������  ��  ��
//  �޸ļ�¼  ��  ����
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
	ModbusTxData[DataLength]  =  SLAVE_BASE_ADD;
    DataLength++;
    ModbusTxData[DataLength]  =  RTU_R03_CMD;
    DataLength++;
    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_HI]; 
    DataLength++;
	ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_LO];
	DataLength++;
	TempForTxBuffer.Bytes.LSB  =  ModbusRxData[RTU_PAR_ADD_LO];		//��Ҫ��ȡ�ļĴ�����ַ�ĵ��ֽ�
	TempForTxBuffer.Bytes.MSB  =  ModbusRxData[RTU_PAR_ADD_HI];		//��Ҫ��ȡ�ļĴ�����ַ�ĸ��ֽ�
	TempData = TempForTxBuffer.Word - RTU_REG_BASE; 				//

    for(i = 0; i < ModbusRxData[RTU_PAR_NUM_LO]; i++)				//ѭ��������д�뷢�ͻ�����
    {
   	 	 TempForTxBuffer.Word  =  DataBuffer[TempData + i].RegisterData.Word;
     	 ModbusTxData[DataLength] = TempForTxBuffer.Bytes.MSB;
      	 DataLength++;
       	 ModbusTxData[DataLength]    =  TempForTxBuffer.Bytes.LSB;
       	 DataLength++;
    }    

    CRCData.Word=CRC16Bit(ModbusTxData, DataLength);
    ModbusTxData[DataLength]   =  CRCData.Bytes.LSB;  
    DataLength++;
    ModbusTxData[DataLength]   =  CRCData.Bytes.MSB; 
    DataLength++;
/*ͨ�����ڷ�������*/
	HAL_UART_Transmit_DMA(&huart2,ModbusTxData,DataLength);
}
void Host04Service(void)
{
	uint16_t  DataLength = 0;
    uint16_t  TempData = 0;
    uint8_t   i;
	uint16_t count;
	union WordtoBytes TempForTxBuffer={0};
	uint16_t CRCData={0};
	
    DataLength  =  0;
	memset(ModbusTxData,0x00,TXBUFF_LENGTH);
//-------------------------------------------
	ModbusTxData[DataLength]  =  LOCAL_BASE_ADD;
    DataLength++;
    ModbusTxData[DataLength]  =  RTU_R04_CMD;
    DataLength++;
//    ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_HI]; 
//    DataLength++;
	ModbusTxData[DataLength]  =  ModbusRxData[RTU_PAR_NUM_LO]<<1;				//MODBUS���ݸ���
	DataLength++;
	TempForTxBuffer.Bytes.MSB  =  ModbusRxData[RTU_PAR_ADD_HI];					//��Ҫ��ȡ�ļĴ�����ַ�ĵ��ֽ�
	TempForTxBuffer.Bytes.LSB  =  ModbusRxData[RTU_PAR_ADD_LO]+1;				//��Ҫ��ȡ�ļĴ�����ַ�ĸ��ֽ�
	TempData = TempForTxBuffer.Word - RTU_REG_BASE; 							//
    count = ModbusRxData[RTU_PAR_NUM_LO];
	for(i = 0; i < count; i++)						//ѭ��������д�뷢�ͻ�����
    {
   	 	 TempForTxBuffer.Word  =  DataBuffer[TempData + i].RegisterData.Word;
     	 ModbusTxData[DataLength] = TempForTxBuffer.Bytes.MSB;
      	 DataLength++;
       	 ModbusTxData[DataLength] = TempForTxBuffer.Bytes.LSB;
       	 DataLength++;
    }    
    CRCData=CRC16Bit(&ModbusTxData[0],DataLength);
	TempForTxBuffer.Word = CRCData;
    ModbusTxData[DataLength] = TempForTxBuffer.Bytes.MSB;  
    DataLength++;
    ModbusTxData[DataLength] = TempForTxBuffer.Bytes.LSB; 
    DataLength++;
/*ͨ�����ڷ�������*/
	HAL_UART_Transmit(&huart2,ModbusTxData,DataLength,1000);	
}
//===============================================================
//  ��������  ��  Host06Service()
//  ��������  ��  
//  ��ڲ���  ��  ��
//  ��������  ��  ��
//  �޸ļ�¼  ��  ����
//================================================================
void Host06Service(void)
{
    uint8_t   DataLength;
//    uint16_t  tempdata;
//	uint8_t   i;
	union WordtoBytes CRCData;
		
    DataLength  =  0;
//-------------------------------------------
    ModbusTxData[DataLength]  =  SLAVE_BASE_ADD;
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
	HAL_UART_Transmit_DMA(&huart2,ModbusTxData,DataLength); 
}
//===============================================================
//  ��������  ��  Host10Service()
//  ��������  ��  
//  ��ڲ���  ��  ��
//  ��������  ��  ��
//  �޸ļ�¼  ��  ����
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
    ModbusTxData[DataLength]  =  SLAVE_BASE_ADD;
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
//------------------------------------------------
	HAL_UART_Transmit_DMA(&huart2,ModbusTxData,DataLength);
}
