#ifndef _SCCB_H_
#define _SCCB_H_

#define SCCB_SDA_IN()  {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=0<<4*2;}	//PD7 输入  //7*2 to 2*2
#define SCCB_SDA_OUT() {GPIOE->MODER&=~(3<<(4*2));GPIOE->MODER|=1<<4*2;} 	//PD7 输出

//IO操作函数	 
#define SCCB_SCL    		PEout(3)	 	//SCL
#define SCCB_SDA    		PEout(4) 		//SDA	 

#define SCCB_READ_SDA    	PEin(4)  		//输入SDA    
#define SCCB_ID   			0X42  			//OV7670的ID

void SCCB_Configuration(void);
u8 SCCB_WR_Reg(u8 reg,u8 data);
u8 SCCB_RD_Reg(u8 reg);
#endif
