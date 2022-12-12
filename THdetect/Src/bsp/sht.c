#include "sht.h"
#include "I2C.h"
#include<stdio.h>
#include "usart.h"
SHT2x_data SHT20;


/*******************************************************************************
* ��������  :  SHT2x_Init
* ��������  :  ��ʼ��SHT20
* �������  :  ��
* ������  :  ��
* ���ص�ֵ  :  ��
*******************************************************************************/
uint8_t SHT2x_Init(void)
{
	uint8_t err;
	I2C_Init();
	err = SHT2x_SoftReset();
	printf("�����λ�Ľ����:%d\r\n",err);
	return err;
}

/*******************************************************************************
* ��������  :  SHT2x_SoftReset
* ��������  :  SHT20�����λ
* �������  :  ��
* ������  :  ��
* ���ص�ֵ  :  ��
*******************************************************************************/
uint8_t SHT2x_SoftReset(void) //SHT20�����λ
{
	uint8_t err=0;
	I2C_Start();
	printf("I2C start1\n");
	I2C_SendByte(0x80);
	printf("I2C send2\n");
	printf("I2C wait3\n");
	err = I2C_WaitAck();
	printf("I2C send4\n");
	I2C_SendByte(0xFE);
	printf("5\n");
	err = I2C_WaitAck();
	printf("6\n");
	I2C_Stop();
	return err;
}

/*******************************************************************************
* ��������  :  SHT2x_GetTempPoll
* ��������  :  ��ȡ�¶�ֵ
* �������  :  ��
* ������  :  ��
* ���ص�ֵ  :  ��
*******************************************************************************/
float SHT2x_GetTempPoll(void)
{
	
    float TEMP;
    uint8_t ack, tmp1, tmp2;
    uint16_t ST;
    uint16_t i=0;
    printf("���ڲ���!\r\n");
    I2C_Start();						//����I2C��ʼ�ź�
    I2C_SendByte(I2C_ADR_W);			//I2C����һ���ֽ� 
		ack = I2C_WaitAck();	
		I2C_SendByte(TRIG_TEMP_MEASUREMENT_POLL);
		ack = I2C_WaitAck();
    do 
		{
			HAL_Delay(100);               
			I2C_Start();				//����I2C��ʼ�ź�
			I2C_SendByte(I2C_ADR_R);	
			i++;
			ack = I2C_WaitAck();
			if(i==1000)break;
    } while(ack!=0);
    tmp1 = I2C_ReadByte();
    tmp2 = I2C_ReadByte();
		I2C_ReadByte();
    I2C_Stop();

    ST = (tmp1 << 8) | (tmp2 << 0);
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85;
    return (TEMP);	  
}

/*******************************************************************************
* ��������  :  SHT2x_GetHumiPoll
* ��������  :  ��ȡʪ��
* �������  :  ��
* ������  :  ��
* ���ص�ֵ  :  ��
*******************************************************************************/
float SHT2x_GetHumiPoll(void)
{
    float HUMI;
    uint8_t ack, tmp1, tmp2;
    uint16_t SRH;
		uint16_t i=0;
    //printf("����ʪ��\n");
    I2C_Start();						//����I2C��ʼ�ź�
    I2C_SendByte(I2C_ADR_W);			//I2C����һ���ֽ� 
		ack = I2C_WaitAck();	
		I2C_SendByte(TRIG_HUMI_MEASUREMENT_POLL);
		ack = I2C_WaitAck();    
    do 
		{
			HAL_Delay(100);               
			I2C_Start();				//����I2C��ʼ�ź�
			I2C_SendByte(I2C_ADR_R);	
			i++;
			ack = I2C_WaitAck();
			if(i==100)
				break;
    } while(ack!=0);//��ack=1 ѭ������ack=0����
    
    tmp1 = I2C_ReadByte();//READ BYTE AND ACK
   
    tmp2 = I2C_ReadByte();//READ BYTE AND ACK
    I2C_ReadByte();//READ BYTE BUT NACK
    I2C_Stop();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = ((float)SRH * 0.00190735) - 6;
    return (HUMI);
}
int16_t SHT2x_GetTempPolltest(void)						//��ȡSHT20 �¶�(������)
{
	int16_t faskData=22;
	return faskData;
}
int16_t SHT2x_GetHumiPolltest(void)						//��ȡSHT20 ʪ��(������)
{
	int16_t faskData=55;
	return faskData;
}

