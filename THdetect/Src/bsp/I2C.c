
/* 包含头文件 ----------------------------------------------------------------*/
#include "I2C.h"
static void I2C_Delay(void) /* 函数功能: I2C总线位延迟，最快400KHz*/
{
	
	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
		CPU主频72MHz时，在内部Flash运行, MDK工程不优化
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us     
    IAR工程编译效率高，不能设置为7
	*/
	
	HAL_Delay(10);
	
}

void I2C_Start(void) /*函数功能: CPU发起I2C总线启动信号*/
{
	/* 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 */
	I2C_SDA_HIGH();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_LOW();
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
}
void I2C_Stop(void) /* 函数功能: CPU发起I2C总线停止信号*/
{
	/* 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 */
	I2C_SDA_LOW();
	I2C_SCL_HIGH();
	I2C_Delay();
	I2C_SDA_HIGH();
}
void I2C_SendByte(uint8_t Byte) /*函数功能: CPU向I2C总线设备发送8bit数据 输入参数: Byte ： 等待发送的字节*/
{
	uint8_t i;
	for (i = 0; i < 8; i++) /* 先发送字节的高位bit7 */
	{		
		if (Byte & 0x80)I2C_SDA_HIGH();
		else	I2C_SDA_LOW();
		I2C_Delay();
		I2C_SCL_HIGH();
		I2C_Delay();	
		I2C_SCL_LOW();
		if (i == 7)I2C_SDA_HIGH(); // 释放总线
		Byte <<= 1;	/* 左移一个bit */
		I2C_Delay();
	}
}
/* CPU从I2C总线设备读取8bit数据   返 回 值: 读到的数据*/
uint8_t I2C_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	value = 0; /* 读到第1个bit为数据的bit7 */
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
/* 函数功能: CPU产生一个时钟，并读取器件的ACK应答信号
  * 返 回 值: 返回0表示正确应答，1表示无器件响应*/
uint8_t I2C_WaitAck(void)
{
	uint8_t re;
	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	I2C_Delay();
	if (I2C_SDA_READ())re = 1;	/* CPU读取SDA口线状态 */
	else	re = 0;
	I2C_SCL_LOW();
	I2C_Delay();
	return re;
}
void I2C_Ack(void) /*函数功能: CPU产生一个ACK信号*/
{
	I2C_SDA_LOW();	/* CPU驱动SDA = 0 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();
	I2C_SDA_HIGH();	/* CPU释放SDA总线 */
}
void I2C_NAck(void) /*函数功能: CPU产生1个NACK信号*/
{
	I2C_SDA_HIGH();	/* CPU驱动SDA = 1 */
	I2C_Delay();
	I2C_SCL_HIGH();	/* CPU产生1个时钟 */
	I2C_Delay();
	I2C_SCL_LOW();
	I2C_Delay();	
}
void I2C_Init(void) /* 函数功能: 配置I2C总线的GPIO，采用模拟IO的方式实现*/
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
	I2C_GPIO_CLK_ENABLE(); /* 打开GPIO时钟 */

  GPIO_InitStruct.Pin = I2C_SCL_PIN|I2C_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);

  I2C_Stop();  /* 给一个停止信号, 复位I2C总线上的所有设备到待机模式 */
}

/*函数功能: 检测I2C总线设备，CPU向发送设备地址，然后读取设备应答来判断该设备是否存在
  * 输入参数: _Address：设备的I2C总线地址
  * 返 回 值: 返回值 0 表示正确， 返回1表示未探测到
  * 说    明：在访问I2C设备前，请先调用 I2C_CheckDevice() 检测I2C设备是否正常，该函数会配置GPIO*/
uint8_t I2C_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;

	I2C_Init();		               /* 配置GPIO */	
	I2C_Start();		                 /* 发送启动信号 */
	I2C_SendByte(_Address | I2C_WR); /* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
	ucAck = I2C_WaitAck();	/* 检测设备的ACK应答 */
	I2C_Stop();			                 /* 发送停止信号 */
	return ucAck;
}
///** 判断串行EERPOM是否正常 返回值: 1 表示正常， 0 表示不正常*/
//uint8_t EEPROM_CheckOk(void)
//{
//	if(I2C_CheckDevice(EEPROM_DEV_ADDR) == 0)return 1;
//	else
//	{
//		I2C_Stop();		 /* 失败后，切记发送I2C总线停止信号 */
//		return 0;
//	}
//}
///**函数功能: 从串行EEPROM指定地址处开始读取若干数据
//  * 输入参数: ReadBuf : 存放读到的数据的缓冲区指针
//  *           Address : 起始地址  
//  *           Size : 数据长度，单位为字节
//  * 返 回 值:  0 表示失败，1表示成功*/
//uint8_t EEPROM_ReadBytes(uint8_t *ReadBuf, uint16_t Address, uint16_t Size)
//{
//	uint16_t i;
//	
//	if(Size>=64||Size==0)return 0;
//	
//	I2C_Start();	
//	
//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	 /*发送写命令*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;	
//	I2C_SendByte((uint16_t)Address>>8);	     /*发送地址*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	
//  I2C_SendByte((uint16_t)Address%256);	
//	if (I2C_WaitAck() != 0)goto cmd_fail;	
//	
//	I2C_Start();	

//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_RD);	/*发送读命令 */
//	if (I2C_WaitAck() != 0)goto cmd_fail;		
//	
//	/* 第9步：循环读取数据 */
//	for (i = 0; i < Size; i++)
//	{
//		ReadBuf[i] = I2C_ReadByte();	
//		//if (i != Size - 1) 
//		I2C_Ack();	/* 中间字节读完后，CPU产生ACK信号(驱动SDA = 0) */
//		//else	I2C_NAck();	/* 最后1个字节读完后，CPU产生NACK信号(驱动SDA = 1) */
//		
//	}
//	I2C_Stop(); /* 发送I2C总线停止信号 */
//	return 1;	/* 执行成功 */

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	I2C_Stop();
//	return 0;
//}
///*向串行EEPROM指定地址写入若干数据，采用页写操作提高写入效率
//  * 输入参数: WriteBuf : 存放带写入的数据的缓冲区指针
//  *           Address : 起始地址
//  *           Size : 数据长度，单位为字节 
//  * 返 回 值:  0 表示失败，1表示成功*/
//uint8_t EEPROM_WriteBytes(uint8_t *WriteBuf, uint16_t Address, uint16_t Size)
//{
//	uint16_t i;	
//	if(Size>=64||Size==0)return 0;
//	I2C_Start();
//	I2C_SendByte(EEPROM_DEV_ADDR | I2C_WR);	 /*发送写命令*/	
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	I2C_SendByte((uint16_t)Address>>8);	     /*发送地址*/
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	I2C_SendByte((uint16_t)Address%256);
//	if (I2C_WaitAck() != 0)goto cmd_fail;
//	
//	for (i = 0; i < Size; i++)               /*发送数据*/
//	{
//		I2C_SendByte(WriteBuf[i]);
//		I2C_Ack();
//	}
//	
//	I2C_Stop();  /* 命令执行成功，发送I2C总线停止信号 */
//	return 0;

//cmd_fail: /* 命令执行失败后，切记发送停止信号，避免影响I2C总线上其他设备 */
//	/* 发送I2C总线停止信号 */
//	I2C_Stop();
//	return 0;
//}

/****END OF FILE****/
