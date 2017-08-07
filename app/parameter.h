#ifndef _PARAMETER_H_
#define _PARAMETER_H_
#include "stm32f4xx.h"



#define PID_ST_SIZE 28u


typedef struct {

_PID_arg_st pid_st[5];
sensor_setup_t sensor_setup;
u8 pid_saved_flag;
}saveToFlash_st;

union _Pid_un_
{
	saveToFlash_st save_to_flash;
	uint8_t pid_array[sizeof(saveToFlash_st)];
};
extern union _Pid_un_ pid_un;
extern _PID_arg_st PID1_arg;
extern _PID_arg_st PID2_arg;
extern _PID_arg_st PID3_arg;
extern _PID_arg_st PID4_arg;
extern _PID_arg_st PID5_arg;


extern _PID_val_st PID1_val;
extern _PID_val_st PID2_val;
extern _PID_val_st PID3_val;
extern _PID_val_st PID4_val;
extern _PID_val_st PID5_val;
void Para_ResetToFactorySetup(void);
void Param_SaveAccelOffset(xyz_f_t *offset);
void Param_SaveGyroOffset(xyz_f_t *offset);
void Param_SaveMagOffset(xyz_f_t *offset);
uint8_t AppParamSave(void);
extern u8 pid_saved_flag;
void PID_Para_Init(void);
#define PARAM_SAVED_START_ADDRESS ADDR_FLASH_SECTOR_10
#define PARAM_SAVED_FLAG_ADDRESS ADDR_FLASH_SECTOR_11
#endif

