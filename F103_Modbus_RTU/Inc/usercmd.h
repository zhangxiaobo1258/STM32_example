#ifndef    	__UAERCMD_H__
#define    	__UAERCMD_H__

#define    	RTU_START_BYTE 					0x23
#define    	RTU_END_BYTE_HI					0x0A
#define    	RTU_END_BYTE_LO					0x0D

#define 	RTU_TIMEOUT						5			//��λms

#define    	RTU_BASE_ADD              		0          //ͨ�Ż����� ��ַ��λ��
#define    	RTU_CMD_FUN               		1          //ͨ�Ż����� ������λ��
#define    	RTU_PAR_ADD_HI            		2          //ͨ�Ż����� ������ʼ��ַHλ��
#define    	RTU_PAR_ADD_LO            		3          //ͨ�Ż����� ������ʼ��ַLλ��
#define    	RTU_PAR_NUM_HI            		4          //ͨ�Ż����� ���������ֽ�H������Hλ��  0x06������������
#define    	RTU_PAR_NUM_LO            		5          //ͨ�Ż����� ���������ֽ�L������Lλ��  0x06������������
#define    	RTU_FUN10_DATA_LEN        		6          //ͨ�Ż����� 0x10�������������ֽڳ�������λ��
#define    	RTU_FUN10_DATA_HI         		7          //ͨ�Ż����� 0x10�������������ֽڿ�ʼ����Hλ��
#define    	RTU_FUN10_DATA_LO         		8          //ͨ�Ż����� 0x10�������������ֽڿ�ʼ����Lλ��

#define    	RTU_R01_CMD               		0x01       	//��������03
#define    	RTU_R02_CMD               		0x02       	//��������03
#define    	RTU_R03_CMD               		0x03       	//��������03
#define    	RTU_R04_CMD               		0x04       	//��������03
#define    	RTU_W06_CMD               		0x06       	//д�������ݹ�����06
#define    	RTU_W10_CMD               		0x10       	//д������ݹ�����10
#define    	RTU_REG_BASE              		0x0581     	//�Ĵ�����ͨ�Ż�ַ
#define    	RTU_ZONE_H                		0x15       	//RTU����֡���������ߵ�ַ
#define    	RTU_DATA_H                		0x00       	//RTU����֡�����ݸ��ֽ�

//-----------------------------------------------------------------------------------------------------------------------------------------
#define    	SLAVE_BASE_ADD             		0x32       	//�������ַ
#define    	LOCAL_BASE_ADD               	0x30       	//�������ַ

#define    	RXBUFF_LENGTH                	24         	//���ջ���������  
#define    	RXBUFF_MIN                   	8          //��������֡��С����
#define    	TXBUFF_LENGTH                	65         	//���ͻ���������  
#define    	TXBUFF_MIN                   	8          //��������֡��С����

//------------------------------------------------------------------------------------------------------------------------------------
#define    	COM_BUFF_LENGTH           		42         	//ͨ�����ݻ����ַ������    Max=14

#define    	COM_BUFF_UPOFFSET_FRONT_TMP  	0          	//ͨ�����ݻ���    ��ƫ���ǰ���¶�
#define    	COM_BUFF_UPOFFSET_END_TMP    	1		   	//ͨ�����ݻ���    ��ƫ�������¶�
#define    	COM_BUFF_UPOFFSET_MONTOR_TMP 	2			//ͨ�����ݻ���    ��ƫ��������¶�
#define    	COM_BUFF_DOWNOFFSET_FRONT_TMP  	3          	//ͨ�����ݻ���    ��ƫ���ǰ���¶�
#define    	COM_BUFF_DOWNOFFSET_END_TMP    	4		   	//ͨ�����ݻ���    ��ƫ�������¶�
#define    	COM_BUFF_DOWNOFFSET_MONTOR_TMP 	5			//ͨ�����ݻ���    ��ƫ��������¶�
#define    	COM_BUFF_UPMOVE_FRONT_TMP  		6          	//ͨ�����ݻ���    ���Ƶ��ǰ���¶�
#define    	COM_BUFF_UPMOVE_END_TMP    		7		   	//ͨ�����ݻ���    ���Ƶ������¶�
#define    	COM_BUFF_UPMOVE_MONTOR_TMP 		8			//ͨ�����ݻ���    ���Ƶ�������¶�
#define    	COM_BUFF_DOWNMOVE_FRONT_TMP  	9          	//ͨ�����ݻ���    ���Ƶ��ǰ���¶�
#define    	COM_BUFF_DOWNMOVE_END_TMP    	10		   	//ͨ�����ݻ���    ���Ƶ������¶�
#define    	COM_BUFF_DOWNMOVE_MONTOR_TMP 	11			//ͨ�����ݻ���    ���Ƶ�������¶�
#define    	COM_BUFF_SHT21TEMP           	12          //ͨ�����ݻ���    ���������SHT21�¶�
#define    	COM_BUFF_SHT21HUM           		13          //ͨ�����ݻ���    ���������SHT21ʪ��
#define    	COM_BUFF_CLEAN_PRESS   			14			//ͨ�����ݻ���    �ྻ��ѹ
#define    	COM_BUFF_WORK_PRESS         		15			//ͨ�����ݻ���    ������ѹ
#define    	COM_BUFF_BACKUP_PRESS			16			//ͨ�����ݻ���    ������ѹ
#define    	COM_BUFF_TEMP_THRESHOLD			17			//ͨ�����ݻ���    �����������¶���ֵ

//-------------------------------------------------------------------------------------------------
#define    	MCU_INIT_FLAG             		0x00       //ϵͳ״̬---��ʼ��
#define    	MCU_RUN_FLAG      	 	 		0x05       //ϵͳ״̬---��������
#define    	MCU_ERR_FLAG          	 		0x0a       //ϵͳ״̬---����

//----------------------------------------------------��λ�� ��� user end

//----�Ӽ��壺USART�˿�2  RB4/RB3=> ��/��  RS485  MODBUS RTU---------------------------------------------------------------//
//----------------------------------------------------�Ӽ���  ��� user-------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
#define    	RXD_LENGTH              	  	20           //���ջ���������  �������� 
#define    	RXD_MIN                   		17           //��������֡��С����  �쳣����
#define    	TXD_LENGTH                		16           //���ͻ���������  CurrMax=7 ����/n
#define    	TXD_MIN                   		8            //��������֡��С����

#endif 

