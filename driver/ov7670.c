#include "main.h"
#include "sccb.h"
#include "ov7670.h"
static void my_Configuration(void)
{
	//OV_WriteReg(0x0D,0x18);
//	OV_WriteReg(0x11,0x00);
//	OV_WriteReg(0x12,0x46);
//	OV_WriteReg(0x40,0xf0);
//		OV_WriteReg(0x35,0x3A);
//		OV_WriteReg(0x3A,0x15);
	
	
}

int OV7670_Configuration(void){
		int i;
//		char buffer[256];
		GPIO_InitTypeDef GPIO_InitStructure;
  	DCMI_InitTypeDef DCMI_InitStructure;
  	DMA_InitTypeDef  DMA_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		SCCB_Configuration();
		delay_ms(100);
  	RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);//DCMI 
  	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);//DMA2
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);//使能DCMI的GPIO时钟
		
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
  	GPIO_Init(GPIOA, &GPIO_InitStructure);	   
		GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_MCO);//MCO1:PA8  
    RCC_MCO1Config(RCC_MCO1Source_HSI, RCC_MCO1Div_2);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;//PA7:PWRDOWN
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);//power on

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;	//复位
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);

		GPIO_SetBits(GPIOC, GPIO_Pin_13);//power on
		
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);//DCMI_HSYNC 
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_PIXCLK
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D5 			  
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_VSYNC 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI);//DCMI_D6 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D7 			  
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI);//DCMI_D0 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI);//DCMI_D1 			  
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_DCMI);//DCMI_D2 
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_DCMI);//DCMI_D3 
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_DCMI);//DCMI_D4 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; 
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 
    GPIO_Init(GPIOC, &GPIO_InitStructure); 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_5 | GPIO_Pin_6; 
    GPIO_Init(GPIOE, &GPIO_InitStructure);     
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 ; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_6; 
    GPIO_Init(GPIOA, &GPIO_InitStructure);		 

  	DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  	DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  	DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
  	DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  	DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  	DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  	DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  	DCMI_Init(&DCMI_InitStructure); 
  	DMA_DeInit(DMA2_Stream1);
  	DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;
  	DMA_InitStructure.DMA_Memory0BaseAddr = FSMC_LCD_ADDRESS;//camera//USART2->DR
  	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  	DMA_InitStructure.DMA_BufferSize = 50;
  	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  	DMA_Init(DMA2_Stream1, &DMA_InitStructure);
		OV_Reset();
		delay_ms(5);
		DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//开启帧中断 
	

		NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
		
  	for(i=0;i<sizeof(OV7670_reg)/sizeof(OV7670_reg[0]);i++)
  	{
   	if(OV_WriteReg(OV7670_reg[i][0],OV7670_reg[i][1])){
			return 1;
			}
  	}
		my_Configuration();
		DCMI_Cmd(ENABLE);	//DCMI使能
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();		//开始写入GRAM
		DMA_Cmd(DMA2_Stream1, ENABLE);//开启DMA2,Stream1 
		DCMI_CaptureCmd(ENABLE);//DCMI捕获使能  
		return 0; 
	
}

float Camera_fps=0.0f;
void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)//捕获到一帧图像
	{
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//清除帧中断
		LED_Duty();
		Camera_fps=1/(GetInnerLoop(DCMI_Time)/1000000.0f);
	}
} 


