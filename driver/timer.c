#include "main.h"
/*----TIM2---TIM6-----*/
void TIM5_Configuration(void)										//TIM2��Ϊϵͳʱ�ӣ�CNT�Ĵ����е�Ϊ������ʼ�����ڵ�΢����
{
   TIM_TimeBaseInitTypeDef tim;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
    tim.TIM_Period = 0xFFFFFFFF;
    tim.TIM_Prescaler = 84 - 1;	 //1M ��ʱ��  
    tim.TIM_ClockDivision = TIM_CKD_DIV1;	
    tim.TIM_CounterMode = TIM_CounterMode_Up;  
    TIM_ARRPreloadConfig(TIM5, ENABLE);	
    TIM_TimeBaseInit(TIM5, &tim);
    TIM_Cmd(TIM5,ENABLE);	
}

void TIM5_IRQHandler(void)										
{
	  if (TIM_GetITStatus(TIM5,TIM_IT_Update)!= RESET) 
		{
			  TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
        TIM_ClearFlag(TIM5, TIM_FLAG_Update);
		}
} 

void Delay_us(uint32_t us)										//��TIM2�ļ���ֵ��������ȷ��ʱ
{
    uint32_t now = Get_Time_Micros();
    while (Get_Time_Micros() - now < us);
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
        Delay_us(1000);
}




void TIM6_Configuration(void)							
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
    
    nvic.NVIC_IRQChannel = TIM6_DAC_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 84-1;        //84M internal clock
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 1000 - 1;  //1ms,1000Hz
    TIM_TimeBaseInit(TIM6,&tim);
}

void TIM6_Start(void)
{
    TIM_Cmd(TIM6, ENABLE);	 
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}
void TIM6_DAC_IRQHandler(void)									//TIM6�Ļص�����1ms����һ�Σ����ھ�ȷ����ControlLoop
{
	 if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	  {
			TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
			TIM_ClearFlag(TIM6, TIM_FLAG_Update);
			ControlLoop();
    }

}
void InnerLoopInit(void)
{
	int i=0;
	for (i=0;i<INERLOOPLENGTH;i++)
	{
		GetInnerLoop(i);
	}
}	
uint32_t GetInnerLoop(int loop)								//���ڻ�þ�ȷ�ĺ������õ�����
{
	static uint32_t Time[2][20]={0};//Time[0] is the last time, Time[1] is the new time;
	Time[0][loop] = Time[1][loop];
	Time[1][loop] = Get_Time_Micros();
	return Time[1][loop]-Time[0][loop];
}