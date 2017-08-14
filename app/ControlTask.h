#ifndef _CONTROL_TASK_H_
#define _CONTROL_TASK_H_

void ControlLoop(void);
extern float Exp_Pitch,
	 Exp_Roll,
  Exp_X,
 Exp_Y;
void ResetAngle(void);
extern float TargetPosition[9][2];
extern  float position_x_out,position_y_out,speed_x_out,speed_y_out,motor_x_out,motor_y_out;
extern float AWGAngleNow,RAngleNow;
void VelocityUpdate(float T);
extern u8 Task6_Buffer[4];
#endif

