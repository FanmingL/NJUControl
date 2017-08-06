#ifndef _SCCB_H_
#define _SCCB_H_

#define SCCB_SDA_IN()  {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=0<<4*2;}	//PE4 输入  //7*2 to 2*2
#define SCCB_SDA_OUT() {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=1<<4*2;} 	//PE4 输出

//IO操作函数	 
#define SCCB_SCL    		PEout(3)	 	//SCL
#define SCCB_SDA    		PEout(4) 		//SDA	 

#define SCCB_READ_SDA    	PEin(4)  		//输入SDA    
#define SCCB_ID   			0X42  			//OV7670的ID

#define SCCB_SID_H() (SCCB_SDA=1)
#define SCCB_SID_L() (SCCB_SDA=0)
#define SCCB_SIC_H() (SCCB_SCL=1)
#define SCCB_SIC_L() (SCCB_SCL=0)

void SCCB_Configuration(void);
void OV_Reset(void);
u8 OV_ReadID(void);
u8 OV_ReadReg(u8 regID, u8 *regDat);
u8 OV_WriteReg(u8 regID, u8 regDat);
#endif
