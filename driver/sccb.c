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
    SCCB_SID_H();     //�����߸ߵ�ƽ
    delay_us(50);
    SCCB_SIC_H();	   //��ʱ���߸ߵ�ʱ���������ɸ�����
    delay_us(50);
    SCCB_SID_L();
    delay_us(50);
    SCCB_SIC_L();	 //�����߻ָ��͵�ƽ��������������Ҫ
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

	for(j=0;j<8;j++) //ѭ��8�η�������
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
	if(SCCB_READ_SDA)tem=0;//SDA=1����ʧ��
	else tem=1;//SDA=0���ͳɹ�������1
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
	for(j=8;j>0;j--) //ѭ��8�ν�������
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

//дOV7670�Ĵ���
u8 OV_WriteReg(u8 regID, u8 regDat)
{
	SCCB_Start();//����SCCB ���߿�ʼ��������
	if(SCCB_Write(0x42)==0)//д��ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 1;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//������ID
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 2;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regDat)==0)//д���ݵ�������
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 3;//���󷵻�
	}
  	SCCB_Stop();//����SCCB ����ֹͣ��������	
  	return 0;//�ɹ�����
}

//��OV7660�Ĵ���
u8 OV_ReadReg(u8 regID, u8 *regDat)
{
	//ͨ��д�������üĴ�����ַ
	SCCB_Start();
	if(SCCB_Write(0x42)==0)//д��ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 1;//���󷵻�
	}
	delay_us(10);
  	if(SCCB_Write(regID)==0)//������ID
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 2;//���󷵻�
	}
	SCCB_Stop();//����SCCB ����ֹͣ��������	
	delay_us(10);	
	//���üĴ�����ַ�󣬲��Ƕ�
	SCCB_Start();
	if(SCCB_Write(0x43)==0)//����ַ
	{
		SCCB_Stop();//����SCCB ����ֹͣ��������
		return 3;//���󷵻�
	}
	delay_us(10);
  	*regDat=SCCB_Read();//���ض�����ֵ
  	noAck();//����NACK����
  	SCCB_Stop();//����SCCB ����ֹͣ��������
  	return 0;//�ɹ�����
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
