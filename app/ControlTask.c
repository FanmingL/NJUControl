#include "main.h"
#include "math.h"

 u32 SystemTimeMs=0;
 float Exp_Pitch=0.0f,
	 Exp_Roll=0.0f,
  Exp_X=160.0f,
 Exp_Y=120.0f;
 float TargetPosition[9][2]={
	{12.5f,12.3f} ,
	{33.4f,12.3f},
	{53.0f,12.3f},
	{60.0f,40.0f},
	{42.9f,41.0f},
	{53.5f,32.6f},
	{12.3f,53.5f},
	{32.8f,53.0f},					
	{53.8f,52.8f}
 };
 

 
 
 float AWGAngleNow=0.0f,RAngleNow=0.0f;
 float position_x_out,position_y_out,speed_x_out,speed_y_out,motor_x_out,motor_y_out;
static void exp_angle_update(float RotateAngle, float Exp_AngleWithGradiant)
{
	float q0,q1,q2,q3,norm;
				q0 = my_cos(Exp_AngleWithGradiant/2.0f),
				q1 = my_sin(Exp_AngleWithGradiant/2.0f)*cos(RotateAngle),
				q2 = my_sin(Exp_AngleWithGradiant/2.0f)*sin(RotateAngle),
				q3 = 0;
	norm = my_sqrt(q0*q0+q1*q1+q2*q2+q3*q3);
	q0=q0/norm;
	q1=q1/norm;
	q2=q2/norm;
	q3=q3/norm;
	Exp_Roll = fast_atan2(2*(q0*q1 + q2*q3),1 - 2*(q1*q1 + q2*q2)) *57.3f;
	Exp_Pitch = -asin(2*(q1*q3 - q0*q2)) *57.3f;
}

static void AWGUpdate(void)
{
	static float Temp=0.0f,RAngleLast=0.0f;
	AWGAngleNow	=	asin(my_sqrt(ref_q[1]*ref_q[1]+ref_q[2]*ref_q[2]))*2;
	RAngleNow			=	(fast_atan2(ref_q[2],ref_q[1]))+Temp;
	if ((RAngleLast-RAngleNow)>330.0f*RAD_PER_DEG){
		RAngleNow+=360.0f*RAD_PER_DEG;
		Temp+=360.0f*RAD_PER_DEG;
	}else if ((RAngleLast-RAngleNow)<-330.0f*RAD_PER_DEG)
	{
		RAngleNow-=360.0f*RAD_PER_DEG;
		Temp-=360.0f*RAD_PER_DEG;
	} 
}

static void Stop_Task(float T)
{
	motor_x_out=0;
	motor_y_out=0;
	Set_PitchRoll(0,0);
}
#define TARGET (5-1)
static void Task1_Task(float T)
{ 
	PID1_arg.kp=0.076f;
	PID1_arg.kd=0.134f;
	if ((my_pow(My_x-TargetPosition[TARGET][0])+my_pow(My_y-TargetPosition[TARGET][1]))>2*8*8||(my_abs(My_Vx)>240)||(my_abs(My_Vy)>240)){
	///if ((my_abs(My_Vx)>320||my_abs(My_Vy)>320)){
				PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;
		
						
	}	else {
			PID1_arg.ki=1.5f;
	}
	if ((my_pow(My_x-TargetPosition[TARGET][0])+my_pow(My_y-TargetPosition[TARGET][1]))<2*0.8*0.8){
		PID1_arg.ki=0.0f;
	}
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										TargetPosition[TARGET][0],				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.1			//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										TargetPosition[TARGET][1],				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.1			//integration limit，积分限幅
										 );
	speed_x_out=-PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										position_x_out,				//期望值（设定值）
							//		0,
									My_Vx,			//反馈值（）
										&PID2_arg, //PID参数结构体
										&PID3_val,	//PID数据结构体
										50			//integration limit，积分限幅
										 );
	
	speed_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										position_y_out,				//期望值（设定值）
						//				0,
									My_Vy,			//反馈值（）
										&PID2_arg, //PID参数结构体
										&PID4_val,	//PID数据结构体
										50			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-4.0f,4.0f);
		position_y_out=LIMIT(position_y_out,-4.0f,4.0f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	Set_PitchRoll(motor_x_out,motor_y_out);
}

static void Task2_Task(float T)
{
	Task1_Task(T);
	
}
#define TASK3_P  (4-1)
static void Task3_Task(float T)
{
	static int step = 0;
	static int count=0;
		PID1_arg.kp=0.056f;
	PID1_arg.kd=0.15f;
	if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])<1.1*1.1&&my_pow(My_y-TargetPosition[TASK3_P+step][1]))<1.1*1.1&&(step==0)){
		count++;
	}else {
	count-=100;
		count=LIMIT(count,0,1001);
	}
	if (count>400){
		step++;
	}
	//if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])+my_pow(My_y-TargetPosition[TASK3_P+step][1]))>2*8*8||(my_abs(My_Vx)>240)||(my_abs(My_Vy)>240)){
	///if ((my_abs(My_Vx)>320||my_abs(My_Vy)>320)){
		if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])+my_pow(My_y-TargetPosition[TASK3_P+step][1]))>2*8*8){
			PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;
		
						
	}	else {
			PID1_arg.ki=0.9f;
	}
	if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])+my_pow(My_y-TargetPosition[TASK3_P+step][1]))<2*0.93*0.93){
		PID1_arg.ki=0.0f;
	}
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										TargetPosition[TASK3_P+step][0],				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										TargetPosition[TASK3_P+step][1],				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-4.0f,4.0f);
		position_y_out=LIMIT(position_y_out,-4.0f,4.0f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	//	if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])+my_pow(My_y-TargetPosition[TASK3_P+step][1]))<2*0.93*0.93)
	//	{
	//		motor_x_out=0;
	//		motor_y_out=0;
	//	}
	Set_PitchRoll(motor_x_out,motor_y_out);
}
//float Task4_x=40.0f;
//float Task4_y=48.2f;
float Task4_x=22.0f;
float Task4_y=39.5f;

static void Task4_Task(float T)
{
	static int step = 0;
	static int count=0;
	float Target_x,Target_y;
	
	PID1_arg.kp=0.060f;
	PID1_arg.kd=0.205f;
	
	if(step==0&&(my_pow(My_x-Task4_x)+my_pow(My_y-Task4_y))<2*5*5) {
		count++;
	}else {
	count-=2;
		count=LIMIT(count,0,1001);
	}
	if (count>100){
		step++;
		count = 0;
	}
	if (!step){
		Target_x=Task4_x;
		Target_y=Task4_y;
	}else{
			Target_x=TargetPosition[8][0];
		Target_y=TargetPosition[8][1];
	}
	
	//if ((my_pow(My_x-TargetPosition[TASK3_P+step][0])+my_pow(My_y-TargetPosition[TASK3_P+step][1]))>2*8*8||(my_abs(My_Vx)>240)||(my_abs(My_Vy)>240)){
	///if ((my_abs(My_Vx)>320||my_abs(My_Vy)>320)){
		if ((my_pow(My_x-Target_x)+my_pow(My_y-Target_y))>2*12*12){
			PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;	
	}	else {
			PID1_arg.ki=0.88f;
	}
		if ((my_pow(My_x-Target_x)+my_pow(My_y-Target_y))<2){
		PID1_arg.ki=0.0f;
	}
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										Target_x,				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										Target_y,				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-1.3f,1.3f);
		position_y_out=LIMIT(position_y_out,-1.3f,1.3f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	//		if ((my_pow(My_x-TargetPosition[8][0])+my_pow(My_y-TargetPosition[8][1]))<2*1.3f*1.3f)
	//	{
	//		motor_x_out=0;
	//		motor_y_out=0;
	//  	}
	Set_PitchRoll(motor_x_out,motor_y_out);
	

}

static void Task5_Task(float T)
{
	
	static int my_target=0;
	static int circle = 0;
	static int my_step=0;
	static int count=0;
	u8 XU[3]={1,5,8};
	float target_x = TargetPosition[XU[my_step]][0],
				target_y = TargetPosition[XU[my_step]][1];
	PID1_arg.kp=0.056f;
	PID1_arg.kd=0.14f;
	
	
	if((my_pow(My_x-target_x)+my_pow(My_y-target_y))<2*1.2*1.2) {
		count++ ;
if (count>20){		
		my_step++;
		my_step=LIMIT(my_step,0,2);
	count = 0;
}
	}
	if ((my_pow(My_x-target_x)+my_pow(My_y-target_y) )>2*8*8){
			PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;	
	}	else {
			PID1_arg.ki=0.9f;
	}
	
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_x,				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.3		//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_y,				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-1.5f,1.5f);
		position_y_out=LIMIT(position_y_out,-1.5f,1.5f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	// 	if ((my_pow(My_x-TargetPosition[8][0])+my_pow(My_y-TargetPosition[8][1]))<2*1.1f*1.1f)
	//	{
	//		motor_x_out=0;
	//		motor_y_out=0;
	//	}
	Set_PitchRoll(motor_x_out,motor_y_out);
}



u8 Task6_Buffer[4]={0,0,0,0};

u8 Task6_Map[3][3]={
{0,1,2},
{1,1,1},
{2,1,3},
};
static void Task6_Task(float T)
{
	static int my_step=0;
	float target_x,target_y ;
		static int count=0;
	static int little_step=0;
	int Map;
	if (my_step!=3){
	Map	= Task6_Map[(int)my_abs(Task6_Buffer[my_step]/3-Task6_Buffer[my_step+1]/3)][(int)my_abs(Task6_Buffer[my_step]%3-Task6_Buffer[my_step+1]%3)];
	if (Map==1){
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
	}else if (Map==2)
	{
		if ((Task6_Buffer[my_step]-1)/3==(Task6_Buffer[my_step+1]-1)/3){
		
		if ((Task6_Buffer[my_step]-1)/3==0){
			if (little_step==0){ 
			target_x = (TargetPosition[1][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[1][1])/2;
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			} 
		
		
		}else{
				if (little_step==0){
		target_x = (TargetPosition[7][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[7][1])/2;		
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			}
		
		
		}
			
		}else{
					if (Task6_Buffer[my_step]%3==0){
			if (little_step==0){
			target_x = (TargetPosition[5][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[5][1])/2;
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			}
		
		
		}else{
				if (little_step==0){
		target_x = (TargetPosition[3][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[3][1])/2;		
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			}
		
		
		}
		
		
		}
	
	
	}else if (Map==3)
	{
		if (Task6_Buffer[my_step]==3||Task6_Buffer[my_step]==7)
		{		
				if (little_step==0){
		target_x = (TargetPosition[8][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[8][1])/2;		
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			}
		}		
		else
		{
		if (little_step==0){
		target_x = (TargetPosition[6][0]+TargetPosition[4][0])/2,
		target_y = (TargetPosition[4][1]+TargetPosition[6][1])/2;		
			}else {
	target_x = TargetPosition[Task6_Buffer[my_step+1]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step+1]-1][1];
			}
		
		}
	
	
	}
	}else {

	target_x = TargetPosition[Task6_Buffer[my_step]-1][0],
	target_y = TargetPosition[Task6_Buffer[my_step]-1][1];
}

	if((my_pow(My_x-target_x)+my_pow(My_y-target_y))<2*1.2*1.2) {
		count++ ;
if (count>20){	
if (Map==1){	
		my_step++;
}
else if (Map==2||Map==3){
	little_step++;
	if (little_step>1){
		my_step++;
		little_step=0;
	}
}
my_step=LIMIT(my_step,0,3);
	count = 0;
}
	}


	if ((my_pow(My_x-target_x)+my_pow(My_y-target_y))>2*8*8){
			PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;	
	}	else {
			PID1_arg.ki=1.3f;
	}
	if ((my_pow(My_x-TargetPosition[Task6_Buffer[3]-1][0])+my_pow(My_y-TargetPosition[Task6_Buffer[3]-1][1]))<2*4*4){
			PID1_arg.ki=0.78f;
	}
		PID1_arg.kp=0.056f;
	PID1_arg.kd=0.217f;
	
	
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_x,				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.3		//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_y,				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-2.0f,2.0f);
		position_y_out=LIMIT(position_y_out,-2.0f,2.0f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	Set_PitchRoll(motor_x_out,motor_y_out);
	
	
	 
	
	
}
float CirclePoint[5][2]=
{{43.2f,43.2f},
{31.3,43.2},
	{22.5,37.2},
		{27.5,22},
			{42.3,27.9}
};
static void Task7_Task(float T)
{
	static int circle = 0;
	static int my_step=0;
	static int count=0;
	float target_x = CirclePoint[my_step][0],
				target_y = CirclePoint[my_step][1];
	PID1_arg.kp=0.056f;
	PID1_arg.kd=0.15f;
	
	
	if((my_pow(My_x-target_x)+my_pow(My_y-target_y))<2*1.2*1.2) {
		count++ ;
if (count>20){		
		my_step++;
	if (my_step==5)
	{my_step=0;
	circle++;
	}
	count = 0;
}
	}
	if (circle>=3){
	target_x = TargetPosition[8][0],
				target_y = TargetPosition[8][1];
	}
	if ((my_pow(My_x-target_x)+my_pow(My_y-target_y))>2*8*8){
			PID1_arg.ki=0;
				PID1_val.err_i=0;
				PID2_val.err_i=0;	
	}	else {
			PID1_arg.ki=1.4f;
	}
	if ((my_pow(My_x-TargetPosition[8][0])+my_pow(My_y-TargetPosition[8][0]))<2*12*12)
	{
			PID1_arg.ki=0.8f;
	}
	
	position_x_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_x,				//期望值（设定值）
										My_x,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID1_val,	//PID数据结构体
										0.3		//integration limit，积分限幅
										 );
	position_y_out=PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										target_y,				//期望值（设定值）
										My_y,			//反馈值（）
										&PID1_arg, //PID参数结构体
										&PID2_val,	//PID数据结构体
										0.3			//integration limit，积分限幅
										 );
		position_x_out=LIMIT(position_x_out,-1.5f,1.5f);
		position_y_out=LIMIT(position_y_out,-1.5f,1.5f);
	motor_x_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_x_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(-Pitch*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
	
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										sin(position_y_out*RAD_PER_DEG),				//期望值（设定值）
										//sin(2*RAD_PER_DEG),
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID3_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										2			//integration limit，积分限幅
										 );
		if (my_abs(Pitch)>=6.0f){
			motor_x_out=0;
		}
		if (my_abs(Roll)>=6.0f){
			motor_y_out=0;
		}
	Set_PitchRoll(motor_x_out,motor_y_out);
	
		
}
static void Task8_Task(float T)
{

}





static void TaskControl(float T)
{
	if (NS==Stop)
	{
		Stop_Task(T);
	}
	else if (NS==Task1)
	{
		Task1_Task(T);
	}
	else if (NS==Task2)
	{
	Task2_Task(T);
	}	
	else if (NS==Task3)
	{
	Task3_Task(T);
	}
	else if (NS==Task4)
	{
	Task4_Task(T);
	
	}
	else if (NS==Task5)
	{
	Task5_Task(T);
	}
	else if (NS==Task6)
	{
	Task6_Task(T);
	}
	else if (NS==Task7)
	{
	Task7_Task(T);
	}
	else if (NS==Task8)
	{
	Task8_Task(T);
	}


}

static void Task_2ms(void)
{
	float inner_loop_time = GetInnerLoop(Task_2ms_Time)/1000000.0f;
	MPU6050_Read();
	MPU6050_Data_Prepare( inner_loop_time );
 	IMUupdate(0.5f *inner_loop_time,mpu6050.Gyro_deg.x, mpu6050.Gyro_deg.y, mpu6050.Gyro_deg.z, //更新IMU
						mpu6050.Acc.x, mpu6050.Acc.y, mpu6050.Acc.z,&Roll,&Pitch,&Yaw);
	AWGUpdate();
//	exp_angle_update(RAngleNow,AWGAngleNow);
}

	

static void Task_5ms(void)
{	
	
}

static void Task_10ms(void)
{
	float inner_loop_time = GetInnerLoop(Task_10ms_Time)/1000000.0f;
	ANO_AK8975_Read();										//读取磁力计
	//if (location_refresh){
	
//	}
	
}


static void Task_20ms(void)
{
	float inner_loop_time = GetInnerLoop(Task_20ms_Time)/1000000.0f;
	Refresh_Encoder();
			if (location_refresh&&(NS!=Stop)){
			BRUSH_COLOR=RED;
			LCD_DrawPoint(My_x,My_y);
			location_refresh=0;
	}
			TaskControl(inner_loop_time);
}

static void Task_50ms(void)
{	

	
	
}

void ControlLoop(void)																//ControlLoop中将各种任务分为1ms、2ms、5ms、10ms、20ms任务
{
	SystemTimeMs++;
	if (SystemTimeMs%2==0)Task_2ms();
	if (SystemTimeMs%5==0)Task_5ms();
	if (SystemTimeMs%10==0)Task_10ms();
	if (SystemTimeMs%20==0)Task_20ms();
	if (SystemTimeMs%50==0)Task_50ms();
	DataTransferTask(SystemTimeMs);
}

void ResetAngle(void)
{
	char temp;
	float T = GetInnerLoop(Reset_Time)/1000000.0f;
	LCD_Clear(BLACK);
	NS=Test;
	LCD_DisplayString(30,30,16,"ReSeting");
	while (my_abs(Roll)>0.08f||my_abs(Pitch)>0.08f){
		temp = Get_KeyValue();
		if (my_abs(Pitch)>0.08f)
		motor_x_out=-1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										0,				//期望值（设定值）
										sin(Pitch*RAD_PER_DEG),			//反馈值（）
										&PID6_arg, //PID参数结构体
										&PID5_val,	//PID数据结构体
										0.2		//integration limit，积分限幅
										 );
	if(my_abs(Roll)>0.08f)
	motor_y_out=1000*PID_calculate( T,            //周期（单位：秒）
										0,				//前馈值
										0,				//期望值（设定值）
										sin(Roll*RAD_PER_DEG),			//反馈值（）
										&PID6_arg, //PID参数结构体
										&PID6_val,	//PID数据结构体
										0.2			//integration limit，积分限幅
										 );									 
	Set_PitchRoll(motor_x_out,motor_y_out);
	if (temp=='D'){
	NS = Stop;
	break;
	}
	}
	NS=Stop;
	motor_x_out=0;
	motor_y_out=0;
	Set_PitchRoll(0,0);
	LCD_Clear(BLACK);

}
void VelocityUpdate(float T){
	static float My_X_old=0,My_Y_old=0;
	float My_Vx_Raw,My_Vy_Raw;
	
	My_Vx_Raw = (My_x-My_X_old)/T;
	My_Vy_Raw = (My_y-My_Y_old)/T;
	
	My_Y_old = My_y;
	My_X_old = My_x;
	
//	My_Vx = My_Vx_Raw;
//	My_Vy = My_Vy_Raw;
//	if(my_abs(My_Vx)>5000){
//		My_Vx=0;
//	}
//	if (my_abs(My_Vy)>5000)
//	{
//			My_Vy=0;
//	}
	LPF_1_(50,T,My_Vx_Raw,My_Vx);
	LPF_1_(50,T,My_Vy_Raw,My_Vy);
	
}

