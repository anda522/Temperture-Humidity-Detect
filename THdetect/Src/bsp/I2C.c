
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "I2C.h"
static void I2C_Delay(void) /* ��������: I2C����λ�ӳ٣����400KHz*/
{
	
	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
		CPU��Ƶ72MHzʱ�����ڲ�Flash����, MDK���̲��Ż�
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us     
    IAR���̱���Ч�ʸߣ���������Ϊ7
	*/
	
	HAL_Delay(10);
	
}

void I2C_Start(void) /*��������: CPU����I2C���������ź�*/
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� */
	I2C_SDA_HIGH();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_LOW();
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
}
void I2C_Stop(void) /* ��������: CPU����I2C����ֹͣ�ź�*/
{
	/* ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_HIGH();
}
void I2C_SendByte(uint8_t Byte) /*��������: CPU��I2C�����豸����8bit���� �������: Byte �� �ȴ����͵��ֽ�*/
{
	uint8_t i;
	for (i = 0; i < 8; i++) /* �ȷ����ֽڵĸ�λbit7 */
	{		
		if (Byte & 0x80)I2C_SDA_HIGH();
		else	I2C_SDA_LOW();
		I2C_Delay();
		I2C_SCL_HIGH();
		I2C_Delay();	
		I2C_SCL_LOW();
		if (i == 7)I2C_SDA_HIGH(); // �ͷ�����
		Byte <<= 1;	/* ����һ��bit */
		I2C_Delay();
	}
}
/* CPU��I2C�����豸��ȡ8bit����   �� �� ֵ: ����������*/
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	value = 0; /* ������1��bitΪ���ݵ�bit7 */
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_HIGH();
		I2C_Delay();
		if (I2C_SDA_READ())value++;	
		I2C_SCL_LOW();
		I2C_Delay();
	}
	return value;
}
/* ��������: CPU����һ��ʱ�ӣ�����ȡ������ACKӦ���ź�
  * �� �� ֵ: ����0��ʾ��ȷӦ��1��ʾ��������Ӧ*/
uint8_t I2C_WaitAck(void)
{
	uint8_t re;
	I2C_SDA_HIGH();	/* CPU�ͷ�SDA���� */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	I2C_Delay();
	if (I2C_SDA_READ())re = 1;	/* CPU��ȡSDA����״̬ */
	else	re = 0;
	I2C_SCL_LOW();
	I2C_Delay();
	return re;
}
void I2C_Ack(void) /*��������: CPU����һ��ACK�ź�*/
{
	I2C_SDA_LOW();	/* CPU����SDA = 0 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU����1��ʱ�� */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
	I2C_SDA_HIGH();	/* CPU�ͷ�SDA���� */
}
void I2C_NAck(void) /*��������: CPU����1��NACK�ź�*/
{
	I2C_SDA_HIGH();	/* CPU����SDA = 1 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU����1��ʱ�� */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();	
}
void I2C_Init(void) /* ��������: ����I2C���ߵ�GPIO������ģ��IO�ķ�ʽʵ��*/
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
	I2C_GPIO_CLK_ENABLE(); /* ��GPIOʱ�� */

  GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);

  I2C_Stop();  /* ��һ��ֹͣ�ź�, ��λI2C�����ϵ������豸������ģʽ */
}

/*��������: ���I2C�����豸��CPU�����豸��ַ��Ȼ���ȡ�豸Ӧ�����жϸ��豸�Ƿ����
  * �������: _Address���豸��I2C���ߵ�ַ
  * �� �� ֵ: ����ֵ 0 ��ʾ��ȷ�� ����1��ʾδ̽�⵽
  * ˵    �����ڷ���I2C�豸ǰ�����ȵ��� I2C_CheckDevice() ���I2C�豸�Ƿ��������ú���������GPIO*/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_Init();		               /* ����GPIO */	
	I2C_Start();		                 /* ���������ź� */
	I2C_SendByte(_Address | I2C_WR); /* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
	ucAck = I2C_WaitAck();	/* ����豸��ACKӦ�� */
	I2C_Stop();			                 /* ����ֹͣ�ź� */
	return ucAck;
}
///** �жϴ���EERPOM�Ƿ����� ����ֵ: 1 ��ʾ������ 0 ��ʾ������*/
//uint8_t EEPROM_CheckOk(void)
//{
//	if(I2C_CheckDevice(EEPROM_DEV_ADDR) == 0)return 1;
//	else
//	{
//		I2C_Stop();		 /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
//		return 0;
//	}
//}
///**��������: �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
//  * �������: ReadBuf : ��Ŷ��������ݵĻ�����ָ��
//  *           Address : ��ʼ��ַ  
//  *           Size : ���ݳ��ȣ���λΪ�ֽ�
//  * �� �� ֵ:  0 ��ʾʧ�ܣ�1��ʾ�ɹ�*/
//uint8_t EEPROM_ReadBytes(uint8_t *ReadBuf, uint16_t Address, uint16_t Size)
//{
//	uint16_t i;
//	
//	if(Size>=64||Size==0)return 0;
//	
//	I2C_Start();	
//	
//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	 /*����д����*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;	
//	I2C_SendByte((uint16_t)Address>>8);	     /*���͵�ַ*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	
//  I2C_SendByte((uint16_t)Address%256);	
//	if (I2C_WaitAck() != 0)goto cmd_fail;	
//	
//	I2C_Start();	

//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_RD);	/*���Ͷ����� */
//	if (I2C_WaitAck() != 0)goto cmd_fail;		
//	
//	/* ��9����ѭ����ȡ���� */
//	for (i = 0; i < Size; i++)
//	{
//		ReadBuf[i] = I2C_ReadByte();	
//		//if (i != Size - 1) 
//		I2C_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA = 0) */
//		//else	I2C_NAck();	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
//		
//	}
//	I2C_Stop(); /* ����I2C����ֹͣ�ź� */
//	return 1;	/* ִ�гɹ� */

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	I2C_Stop();
//	return 0;
//}
///*����EEPROMָ����ַд���������ݣ�����ҳд�������д��Ч��
//  * �������: WriteBuf : ��Ŵ�д������ݵĻ�����ָ��
//  *           Address : ��ʼ��ַ
//  *           Size : ���ݳ��ȣ���λΪ�ֽ� 
//  * �� �� ֵ:  0 ��ʾʧ�ܣ�1��ʾ�ɹ�*/
//uint8_t EEPROM_WriteBytes(uint8_t *WriteBuf, uint16_t Address, uint16_t Size)
//{
//	uint16_t i;	
//	if(Size>=64||Size==0)return 0;
//	I2C_Start();
//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	 /*����д����*/	
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	I2C_SendByte((uint16_t)Address>>8);	     /*���͵�ַ*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	I2C_SendByte((uint16_t)Address%256);
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	
//	for (i = 0; i < Size; i++)               /*��������*/
//	{
//		I2C_SendByte(WriteBuf[i]);
//		I2C_Ack();
//	}
//	
//	I2C_Stop();  /* ����ִ�гɹ�������I2C����ֹͣ�ź� */
//	return 0;

//cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
//	/* ����I2C����ֹͣ�ź� */
//	I2C_Stop();
//	return 0;
//}

/****END OF FILE****/
