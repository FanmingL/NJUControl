#ifndef _PWM_H_
#define _PWM_H_

void PWM_Configuration(void);
#define SetPWMOut(CH1,CH2,CH3,CH4) \
											TIM3->CCR1=CH1;\
											TIM3->CCR2=CH2;\
											TIM3->CCR3=CH3;\
											TIM3->CCR4=CH4;
#define Pitch_Motor_Out(x) (TIM3->CCR3=x);
#define Roll_Motor_Out(x) (TIM3->CCR4=x);
void Set_PitchRoll(float Pitch,float Roll);
#endif
