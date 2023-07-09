#ifndef __CAU_Cable_check_H
#define __CAU_Cable_check_H
//==����
#include "sysconfig.h"
#include "Ano_FcData.h"

//==����
typedef struct
{
	float exp_velocity_h_cmps[2];
	float exp_yaw_pal_dps;
	
}_cau_cable_check_ctrl_st;

//==��������
extern _cau_cable_check_ctrl_st cau_cable_check_ctrl;
extern u16 spd,val; //ֱ����������Ʒ��е��ٶȺ;��룬ʹ����flyctrl�еı���

//==��������

//static

//public
void Cable_check(u8 dT_ms);
void Cable_check_by_time(u8 dT_ms); //��ʱ��

#endif


