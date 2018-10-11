/********************************************************************************
Copyright (C), 2018-2029, Huahuitongda Co., Ltd.
File name:	device_bmp280.c
Athor: Lee Hansson    Version: V1.0.0				Date:2018.4.16
Description: device bmp280 C file
Others:
History:
********************************************************************************/
#include "device_bmp280.h"

static bool bmp280_readByte(uint8_t rxAddr,uint8_t *rxBuf)
{
	if(gIicBusyFlag)
		return false;
	
	gIicBusyFlag = true;
	
	IIC_Start();
	IIC_Send_Byte(BMP280_WRITE_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(rxAddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(BMP280_READ_ADDR);
	IIC_Wait_Ack();
	*rxBuf = IIC_Read_Byte();
	IIC_NAck();
	IIC_Stop();
	
	gIicBusyFlag = false;
	
	return true;
}
static bool bmp280_readBytes(uint8_t rxAddr,uint8_t *rxBuf,uint16_t len)
{
	if(gIicBusyFlag)
		return false;
	
	gIicBusyFlag = true;
	
	IIC_Start();
	IIC_Send_Byte(BMP280_WRITE_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(rxAddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte(BMP280_READ_ADDR);
	IIC_Wait_Ack();
	
	while(len--)
	{
		if(len)
		{
			*rxBuf++ = IIC_Read_Byte();
			IIC_Ack();
		}
		else
		{
			*rxBuf = IIC_Read_Byte();
			IIC_NAck();
		}
	}
	
	IIC_Stop();
	
	gIicBusyFlag = false;
	
	return true;
}

static bool bmp280_writeByte(uint8_t txAddr,uint8_t txBuf)
{
	if(gIicBusyFlag)
		return false;
	
	gIicBusyFlag = true;
	
	IIC_Start();
	IIC_Send_Byte(BMP280_WRITE_ADDR);
	IIC_Wait_Ack();
	IIC_Send_Byte(txAddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(txBuf);
	IIC_Wait_Ack();
	IIC_Stop();
	
	gIicBusyFlag = false;
	
	return true;
}

static bool bmp280_writeBytes(uint8_t txAddr,uint8_t *txBuf,uint16_t len)
{
	if(gIicBusyFlag)
		return false;
	
	gIicBusyFlag = true;
	
	IIC_Start();
	IIC_Send_Byte(BMP280_WRITE_ADDR);
	IIC_Wait_Ack();
	while(len--)
	{
		IIC_Send_Byte(txAddr);
		IIC_Wait_Ack();
		IIC_Send_Byte(*txBuf);
		IIC_Wait_Ack();
		if(len)
		{
			txAddr++;
			txBuf++;
		}
	}
	IIC_Stop();
	
	gIicBusyFlag = false;
	
	return true;
}

bool bmp280_init(void)
{
	uint8_t rxBuf;
	
	if(!bmp280_readByte(BMP280_REG_ID,&rxBuf))
		return false;
	
	if(rxBuf == BMP280_DEVICE_ID)
		return true;
	else
		return false;
}

bool bmp280_setPowerMode(Bmp280_PowerMode Mode)
{
	uint8_t rxBuf;
	uint8_t txBuf;
	
	if(!bmp280_readByte(BMP280_REG_CTRL_MEAS,&rxBuf))
		return false;
	
	txBuf = rxBuf & 0xFC | Mode;
	
	if(!bmp280_writeByte(BMP280_REG_CTRL_MEAS,txBuf))
		return false;
	
	if(!bmp280_readByte(BMP280_REG_CTRL_MEAS,&rxBuf))
		return false;
	
	if(rxBuf == txBuf)
		return true;
	else
		return false;
}

