#ifndef _DRV_OPMV_H
#define _DRV_OPMV_H

//#include "stm32f4xx.h"
//#include "ms5611.h"
#include "sysconfig.h"
#include "TM4C123G.h"

typedef struct 
{
	int8_t rdata[32];
	uint8_t recive_num;
	uint8_t length;
	uint8_t receive_complete_frag;
}CamDataStruct;

typedef struct
{
	s32 height;//cm
	
	float relative_height;	
	float h_delta;
	float h_dt;
	
	u8 measure_ok;
	u8 measure_ot_cnt;
}_height_st;

//void Ultrasonic_Init(void);
void Get_opmv_data(void);
void OPMV_Get(u8);

//extern s8 ultra_start_f;
//extern _height_st ultra;
//extern float cam_position_x_e,cam_position_y_e;//Æ«²î
//extern float cam_angle_z;

extern CamDataStruct CamData;
extern u16 Laser_height_cm;

#endif


