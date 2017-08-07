#include "main.h"
#include "string.h"
_PID_arg_st PID1_arg;
_PID_arg_st PID2_arg;
_PID_arg_st PID3_arg;
_PID_arg_st PID4_arg;
_PID_arg_st PID5_arg;


_PID_val_st PID1_val;
_PID_val_st PID2_val;
_PID_val_st PID3_val;
_PID_val_st PID4_val;
_PID_val_st PID5_val;

#define PID1_P 0.8f
#define PID1_I 3.0f
#define PID1_D 0.1f

#define PID2_P 60.0f
#define PID2_I 0.0f
#define PID2_D 3.0f

#define PID3_P 0.0f
#define PID3_I 0.0f
#define PID3_D 0.0f

#define PID4_P 0.0f
#define PID4_I 0.0f
#define PID4_D 0.0f

#define PID5_P 0.0f
#define PID5_I 0.0f
#define PID5_D 0.0f
union _Pid_un_ pid_un;


void Param_SaveAccelOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Acc_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Accel, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Acc_Temperature = mpu6050.Acc_Temprea_Offset ;
}

void Param_SaveGyroOffset(xyz_f_t *offset)
{
 memcpy(&mpu6050.Gyro_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Gyro, offset,sizeof(xyz_f_t));
	
 sensor_setup.Offset.Gyro_Temperature = mpu6050.Gyro_Temprea_Offset ;
}

void Param_SaveMagOffset(xyz_f_t *offset)
{
 memcpy(&ak8975.Mag_Offset,offset,sizeof(xyz_f_t));
 memcpy(&sensor_setup.Offset.Mag, offset,sizeof(xyz_f_t));
}

void Para_ResetToFactorySetup(void)
{
	PID1_arg.kp=PID1_P;
	PID1_arg.ki=PID1_I;
	PID1_arg.kd=PID1_D;
	PID1_arg.inc_hz=20.0f;
	PID1_arg.k_inc_d_norm=0.0;
	PID1_arg.k_pre_d=0.0f;
	PID1_arg.k_ff=0.0f;
	
	PID2_arg.kp=PID2_P;
	PID2_arg.ki=PID2_I;
	PID2_arg.kd=PID2_D;
	PID2_arg.inc_hz=20.0f;
	PID2_arg.k_inc_d_norm=0.0f;
	PID2_arg.k_pre_d=0.0f;
	PID2_arg.k_ff=0.0f;
	
	PID3_arg.kp=PID3_P;
	PID3_arg.ki=PID3_I;
	PID3_arg.kd=PID3_D;
	PID3_arg.inc_hz=20.0f;
	PID3_arg.k_inc_d_norm=0.0f;
	PID3_arg.k_pre_d=0.0f;
	PID3_arg.k_ff=0.0f;
	
	PID4_arg.kp=PID4_P;
	PID4_arg.ki=PID4_I;
	PID4_arg.kd=PID4_D;
	PID4_arg.inc_hz=20.0f;
	PID4_arg.k_inc_d_norm=0.0f;
	PID4_arg.k_pre_d=0.0f;
	PID4_arg.k_ff=0.0f;
	
	PID5_arg.kp=PID5_P;
	PID5_arg.ki=PID5_I;
	PID5_arg.kd=PID5_D;
	PID5_arg.inc_hz=20.0f;
	PID5_arg.k_inc_d_norm=0.0f;
	PID5_arg.k_pre_d=0.0f;
	PID5_arg.k_ff=0.0f;
	
}

u8 pid_saved_flag=0;
uint8_t AppParamSave(void)
{
	uint8_t retval=1;
	
	pid_un.save_to_flash.pid_st[0]=PID1_arg;
	pid_un.save_to_flash.pid_st[1]=PID2_arg;
	pid_un.save_to_flash.pid_st[2]=PID3_arg;
	pid_un.save_to_flash.pid_st[3]=PID4_arg;
	pid_un.save_to_flash.pid_st[4]=PID5_arg;
	pid_un.save_to_flash.pid_saved_flag=1;
	pid_un.save_to_flash.sensor_setup=sensor_setup;
	BSP_FLASH_Write(PARAM_SAVED_START_ADDRESS, pid_un.pid_array, sizeof(saveToFlash_st));
	PID_Para_Init();
//	pid_saved_flag=1;
//	BSP_FLASH_Write(PARAM_SAVED_FLAG_ADDRESS, &pid_saved_flag, 1);
   return retval;
}

void PID_Para_Init(void)
{
	//BSP_FLASH_Read(PARAM_SAVED_FLAG_ADDRESS,&pid_saved_flag,1);
	BSP_FLASH_Read(PARAM_SAVED_START_ADDRESS, pid_un.pid_array, sizeof(saveToFlash_st));
	if (pid_un.save_to_flash.pid_saved_flag==1){

	
	PID1_arg=pid_un.save_to_flash.pid_st[0];
	PID2_arg=pid_un.save_to_flash.pid_st[1];
	PID3_arg=pid_un.save_to_flash.pid_st[2];
	PID4_arg=pid_un.save_to_flash.pid_st[3];
	PID5_arg=pid_un.save_to_flash.pid_st[4];
		
	sensor_setup=pid_un.save_to_flash.sensor_setup;
		
	memcpy(&mpu6050.Acc_Offset,&sensor_setup.Offset.Accel,sizeof(xyz_f_t));
	memcpy(&mpu6050.Gyro_Offset,&sensor_setup.Offset.Gyro,sizeof(xyz_f_t));
	memcpy(&ak8975.Mag_Offset,&sensor_setup.Offset.Mag,sizeof(xyz_f_t));
	//memcpy(&mpu6050.vec_3d_cali,&sensor_setup.Offset.vec_3d_cali,sizeof(xyz_f_t));
	
	mpu6050.Acc_Temprea_Offset = sensor_setup.Offset.Acc_Temperature;
	mpu6050.Gyro_Temprea_Offset = sensor_setup.Offset.Gyro_Temperature;
  

	}
}
