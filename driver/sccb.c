#include "main.h"
#include "sccb.h"
/*PE3 SIOC*/

/*PE4 SIOD*/

void SCCB_Configuration(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;  
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			 
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
		SCCB_SDA_OUT();
}

void SCCB_Start(void)
{
    SCCB_SID_H();     //数据线高电平
    delay_us(50);
    SCCB_SIC_H();	   //在时钟线高的时候数据线由高至低
    delay_us(50);
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_L();	 //数据线恢复低电平，单操作函数必要
    delay_us(50);
}

void SCCB_Stop(void)
{
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_H();	
    delay_us(50);  
    SCCB_SID_H();	
    delay_us(50);  
}

void noAck(void)
{	
	SCCB_SID_H();	
	delay_us(50);	
	SCCB_SIC_H();	
	delay_us(50);	
	SCCB_SIC_L();	
	delay_us(50);	
	SCCB_SID_L();	
	delay_us(50);
}

u8 SCCB_Write(u8 m_data)
{
	u8 j,tem;

	for(j=0;j<8;j++) //循环8次发送数据
	{
		if((m_data<<j)&0x80)SCCB_SID_H();
		else SCCB_SID_L();
		delay_us(50);
		SCCB_SIC_H();	
		delay_us(50);
		SCCB_SIC_L();	
		delay_us(50);
	}
	delay_us(10);
	SCCB_SDA_IN();
	delay_us(50);
	SCCB_SIC_H();	
	delay_us(10);
	if(SCCB_READ_SDA)tem=0;//SDA=1发送失败
	else tem=1;//SDA=0发送成功，返回1
	SCCB_SIC_L();	
	delay_us(50);	
  SCCB_SDA_OUT();
	return tem;  
}

u8 SCCB_Read(void)
{
	u8 read,j;
	read=0x00;
	
	 SCCB_SDA_IN();
	delay_us(50);
	for(j=8;j>0;j--) //循环8次接收数据
	{		     
		delay_us(50);
		SCCB_SIC_H();
		delay_us(50);
		read=read<<1;
		if(SCCB_READ_SDA)read=read+1; 
		SCCB_SIC_L();
		delay_us(50);
	}	
     SCCB_SDA_OUT();
	return read;
}

//写OV7670寄存器
u8 OV_WriteReg(u8 regID, u8 regDat)
{
	SCCB_Start();//发送SCCB 总线开始传输命令
	if(SCCB_Write(0x42)==0)//写地址
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 1;//错误返回
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//积存器ID
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 2;//错误返回
	}
	delay_us(10);
  	if(SCCB_Write(regDat)==0)//写数据到积存器
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 3;//错误返回
	}
  	SCCB_Stop();//发送SCCB 总线停止传输命令	
  	return 0;//成功返回
}

//读OV7660寄存器
u8 OV_ReadReg(u8 regID, u8 *regDat)
{
	//通过写操作设置寄存器地址
	SCCB_Start();
	if(SCCB_Write(0x42)==0)//写地址
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 1;//错误返回
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//积存器ID
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 2;//错误返回
	}
	SCCB_Stop();//发送SCCB 总线停止传输命令	
	delay_us(10);	
	//设置寄存器地址后，才是读
	SCCB_Start();
	if(SCCB_Write(0x43)==0)//读地址
	{
		SCCB_Stop();//发送SCCB 总线停止传输命令
		return 3;//错误返回
	}
	delay_us(10);
  	*regDat=SCCB_Read();//返回读到的值
  	noAck();//发送NACK命令
  	SCCB_Stop();//发送SCCB 总线停止传输命令
  	return 0;//成功返回
}

void OV_Reset(void)
{
	OV_WriteReg(0x12,0x80);
}

u8 OV_ReadID(void)
{
	u8 temp;
	OV_ReadReg(0x0b,&temp);
  	return temp;
}
