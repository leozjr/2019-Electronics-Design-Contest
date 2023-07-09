#ifndef __CAU_Cable_check_H
#define __CAU_Cable_check_H
//==引用
#include "sysconfig.h"
#include "Ano_FcData.h"

//==定义
typedef struct
{
	float exp_velocity_h_cmps[2];
	float exp_yaw_pal_dps;
	
}_cau_cable_check_ctrl_st;

//==数据声明
extern _cau_cable_check_ctrl_st cau_cable_check_ctrl;
extern u16 spd,val; //直接在这里控制飞行的速度和距离，使用了flyctrl中的变量

//==函数声明

//static

//public
void Cable_check(u8 dT_ms);
void Cable_check_by_time(u8 dT_ms); //定时版

#endif


