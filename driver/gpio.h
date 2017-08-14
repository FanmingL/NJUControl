#ifndef _GPIO_H_
#define _GPIO_H_
void GPIO_Configuration(void);


 #define Pitch_ON() GPIO_SetBits(GPIOC,GPIO_Pin_0);\
										GPIO_ResetBits(GPIOC,GPIO_Pin_1);

#define Pitch_OFF() GPIO_ResetBits(GPIOC,GPIO_Pin_0);\
										GPIO_SetBits(GPIOC,GPIO_Pin_1);
#define Roll_ON() GPIO_SetBits(GPIOC,GPIO_Pin_10);\
									GPIO_ResetBits(GPIOB,GPIO_Pin_12);
#define Roll_OFF() GPIO_ResetBits(GPIOC,GPIO_Pin_10);\
									GPIO_SetBits(GPIOB,GPIO_Pin_12);


#define Motor_Enable() GPIO_SetBits(GPIOB,GPIO_Pin_1);
#define Motor_Disable() GPIO_ResetBits(GPIOB,GPIO_Pin_1);

#endif

