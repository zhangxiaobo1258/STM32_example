#ifndef    	__UAERCMD_H__
#define    	__UAERCMD_H__

#define    	RTU_START_BYTE 					0x23
#define    	RTU_END_BYTE_HI					0x0A
#define    	RTU_END_BYTE_LO					0x0D

#define 	RTU_TIMEOUT						5			//单位ms

#define    	RTU_BASE_ADD              		0          //通信缓冲区 地址码位置
#define    	RTU_CMD_FUN               		1          //通信缓冲区 功能码位置
#define    	RTU_PAR_ADD_HI            		2          //通信缓冲区 参数起始地址H位置
#define    	RTU_PAR_ADD_LO            		3          //通信缓冲区 参数起始地址L位置
#define    	RTU_PAR_NUM_HI            		4          //通信缓冲区 参数连续字节H或数据H位置  0x06功能码是数据
#define    	RTU_PAR_NUM_LO            		5          //通信缓冲区 参数连续字节L或数据L位置  0x06功能码是数据
#define    	RTU_FUN10_DATA_LEN        		6          //通信缓冲区 0x10命令数据连续字节长度数据位置
#define    	RTU_FUN10_DATA_HI         		7          //通信缓冲区 0x10命令数据连续字节开始数据H位置
#define    	RTU_FUN10_DATA_LO         		8          //通信缓冲区 0x10命令数据连续字节开始数据L位置

#define    	RTU_R01_CMD               		0x01       	//读功能码03
#define    	RTU_R02_CMD               		0x02       	//读功能码03
#define    	RTU_R03_CMD               		0x03       	//读功能码03
#define    	RTU_R04_CMD               		0x04       	//读功能码03
#define    	RTU_W06_CMD               		0x06       	//写单笔数据功能码06
#define    	RTU_W10_CMD               		0x10       	//写多笔数据功能码10
#define    	RTU_REG_BASE              		0x0581     	//寄存器的通信基址
#define    	RTU_ZONE_H                		0x15       	//RTU数据帧的数据区高地址
#define    	RTU_DATA_H                		0x00       	//RTU数据帧的数据高字节

//-----------------------------------------------------------------------------------------------------------------------------------------
#define    	SLAVE_BASE_ADD             		0x32       	//主监测板地址
#define    	LOCAL_BASE_ADD               	0x30       	//本监测板地址

#define    	RXBUFF_LENGTH                	24         	//接收缓冲区长度  
#define    	RXBUFF_MIN                   	8          //接收数据帧最小长度
#define    	TXBUFF_LENGTH                	65         	//发送缓冲区长度  
#define    	TXBUFF_MIN                   	8          //发送数据帧最小长度

//------------------------------------------------------------------------------------------------------------------------------------
#define    	COM_BUFF_LENGTH           		42         	//通信数据缓冲地址区长度    Max=14

#define    	COM_BUFF_UPOFFSET_FRONT_TMP  	0          	//通信数据缓冲    上偏电机前端温度
#define    	COM_BUFF_UPOFFSET_END_TMP    	1		   	//通信数据缓冲    上偏电机后端温度
#define    	COM_BUFF_UPOFFSET_MONTOR_TMP 	2			//通信数据缓冲    上偏电机本体温度
#define    	COM_BUFF_DOWNOFFSET_FRONT_TMP  	3          	//通信数据缓冲    下偏电机前端温度
#define    	COM_BUFF_DOWNOFFSET_END_TMP    	4		   	//通信数据缓冲    下偏电机后端温度
#define    	COM_BUFF_DOWNOFFSET_MONTOR_TMP 	5			//通信数据缓冲    下偏电机本体温度
#define    	COM_BUFF_UPMOVE_FRONT_TMP  		6          	//通信数据缓冲    上移电机前端温度
#define    	COM_BUFF_UPMOVE_END_TMP    		7		   	//通信数据缓冲    上移电机后端温度
#define    	COM_BUFF_UPMOVE_MONTOR_TMP 		8			//通信数据缓冲    上移电机本体温度
#define    	COM_BUFF_DOWNMOVE_FRONT_TMP  	9          	//通信数据缓冲    下移电机前端温度
#define    	COM_BUFF_DOWNMOVE_END_TMP    	10		   	//通信数据缓冲    下移电机后端温度
#define    	COM_BUFF_DOWNMOVE_MONTOR_TMP 	11			//通信数据缓冲    下移电机本体温度
#define    	COM_BUFF_SHT21TEMP           	12          //通信数据缓冲    本检测板板上SHT21温度
#define    	COM_BUFF_SHT21HUM           		13          //通信数据缓冲    本检测板板上SHT21湿度
#define    	COM_BUFF_CLEAN_PRESS   			14			//通信数据缓冲    洁净气压
#define    	COM_BUFF_WORK_PRESS         		15			//通信数据缓冲    工作气压
#define    	COM_BUFF_BACKUP_PRESS			16			//通信数据缓冲    备用气压
#define    	COM_BUFF_TEMP_THRESHOLD			17			//通信数据缓冲    本检测板绕组温度阈值

//-------------------------------------------------------------------------------------------------
#define    	MCU_INIT_FLAG             		0x00       //系统状态---初始化
#define    	MCU_RUN_FLAG      	 	 		0x05       //系统状态---正常运行
#define    	MCU_ERR_FLAG          	 		0x0a       //系统状态---故障

//----------------------------------------------------上位机 相关 user end

//----从检测板：USART端口2  RB4/RB3=> 收/发  RS485  MODBUS RTU---------------------------------------------------------------//
//----------------------------------------------------从检测板  相关 user-------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
#define    	RXD_LENGTH              	  	20           //接收缓冲区长度  正常接收 
#define    	RXD_MIN                   		17           //接收数据帧最小长度  异常接收
#define    	TXD_LENGTH                		16           //发送缓冲区长度  CurrMax=7 不含/n
#define    	TXD_MIN                   		8            //发送数据帧最小长度

#endif 

