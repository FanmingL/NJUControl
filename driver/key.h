#ifndef _KEY_H_
#define _KEY_H_

void Key_Configuration(void);
char Get_KeyValue(void);

void Mode_Task(void);
void EXTI_Configuration(void);
extern int step;
#define Key1_GPIO GPIOB
#define Key2_GPIO GPIOC
#define Key3_GPIO GPIOC
#define Key4_GPIO GPIOD
#define Key5_GPIO GPIOB
#define Key6_GPIO GPIOB
#define Key7_GPIO GPIOB
#define Key8_GPIO GPIOB


#define Key1_Pin GPIO_Pin_3
#define Key2_Pin GPIO_Pin_4
#define Key3_Pin GPIO_Pin_5
#define Key4_Pin GPIO_Pin_6
#define Key5_Pin GPIO_Pin_13
#define Key6_Pin GPIO_Pin_14
#define Key7_Pin GPIO_Pin_15
#define Key8_Pin GPIO_Pin_0

#define Key1_IN {Key1_GPIO->MODER&=~(3<<(3*2));Key1_GPIO->MODER|=0<<(3*2);}
#define Key1_OUT {Key1_GPIO->MODER&=~(3<<(3*2));Key1_GPIO->MODER|=1<<(3*2);}
#define Key2_IN {Key2_GPIO->MODER&=~(3<<(4*2));Key2_GPIO->MODER|=0<<(4*2);}
#define Key2_OUT {Key2_GPIO->MODER&=~(3<<(4*2));Key2_GPIO->MODER|=1<<(4*2);}
#define Key3_IN {Key3_GPIO->MODER&=~(3<<(5*2));Key3_GPIO->MODER|=0<<(5*2);}
#define Key3_OUT {Key3_GPIO->MODER&=~(3<<(5*2));Key3_GPIO->MODER|=1<<(5*2);}
#define Key4_IN {Key4_GPIO->MODER&=~(3<<(6*2));Key4_GPIO->MODER|=0<<(6*2);}
#define Key4_OUT {Key4_GPIO->MODER&=~(3<<(6*2));Key4_GPIO->MODER|=1<<(6*2);}
#define Key5_IN {Key5_GPIO->MODER&=~((u32)3<<(13*2));Key5_GPIO->MODER|=(u32)0<<(13*2);}
#define Key5_OUT {Key5_GPIO->MODER&=~((u32)3<<(13*2));Key5_GPIO->MODER|=(u32)1<<(13*2);}
#define Key6_IN {Key6_GPIO->MODER&=~((u32)3<<(14*2));Key6_GPIO->MODER|=(u32)0<<(14*2);}
#define Key6_OUT {Key6_GPIO->MODER&=~((u32)3<<(14*2));Key6_GPIO->MODER|=(u32)1<<(14*2);}
#define Key7_IN {Key7_GPIO->MODER&=~((u32)3<<(15*2));Key7_GPIO->MODER|=(u32)0<<(15*2);}
#define Key7_OUT {Key7_GPIO->MODER&=~((u32)3<<(15*2));Key7_GPIO->MODER|=(u32)1<<(15*2);}
#define Key8_IN {Key8_GPIO->MODER&=~((u32)3<<(0*2));Key8_GPIO->MODER|=0;}
#define Key8_OUT {Key8_GPIO->MODER&=~((u32)3<<(0*2));Key8_GPIO->MODER|=1;}

#define Group1_IN Key1_IN;\
									Key2_IN;\
									Key3_IN;\
									Key4_IN;
#define Group1_OUT Key1_OUT;\
										Key2_OUT;\
										Key3_OUT;\
										Key4_OUT;\
										GPIO_ResetBits(Key1_GPIO,Key1_Pin);\
										GPIO_ResetBits(Key2_GPIO,Key2_Pin);\
										GPIO_ResetBits(Key3_GPIO,Key3_Pin);\
										GPIO_ResetBits(Key4_GPIO,Key4_Pin);
										
#define Group2_IN Key5_IN;\
									Key6_IN;\
									Key7_IN;\
									Key8_IN;
#define Group2_OUT Key5_OUT;\
										Key6_OUT;\
										Key7_OUT;\
										Key8_OUT;\
										GPIO_ResetBits(Key5_GPIO,Key5_Pin);\
										GPIO_ResetBits(Key6_GPIO,Key6_Pin);\
										GPIO_ResetBits(Key7_GPIO,Key7_Pin);\
										GPIO_ResetBits(Key8_GPIO,Key8_Pin);

#define Key1_Val (!((Key1_GPIO->IDR)&Key1_Pin))
#define Key2_Val (!((Key2_GPIO->IDR)&Key2_Pin))
#define Key3_Val (!((Key3_GPIO->IDR)&Key3_Pin))
#define Key4_Val (!((Key4_GPIO->IDR)&Key4_Pin))
#define Key5_Val (!((Key5_GPIO->IDR)&Key5_Pin))
#define Key6_Val (!((Key6_GPIO->IDR)&Key6_Pin))
#define Key7_Val (!((Key7_GPIO->IDR)&Key7_Pin))
#define Key8_Val (!((Key8_GPIO->IDR)&Key8_Pin))

#define Key_Mode1() {Group1_IN;Group2_OUT;}
#define Key_Mode2() {Group2_IN;Group1_OUT;}
#endif
