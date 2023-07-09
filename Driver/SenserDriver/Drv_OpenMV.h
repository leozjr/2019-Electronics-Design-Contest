#ifndef __DRV_OPENMV_H
#define __DRV_OPENMV_H

//==����
#include "sysconfig.h"
#include "Ano_FcData.h"

//==����

typedef struct
{
	//
	u8 sta;	
	s8 angle;
	u8 go;
	u8 turn;
	s16 pos_x;
	s16 pos_y;
	u8 dT_ms;

}_openmv_cable_checking_st;

typedef struct
{
	u8 offline;
	u8 mode_cmd;
	u8 mode_sta;
	//
	_openmv_cable_checking_st cb;
}_openmv_data_st;
//==��������

extern _openmv_data_st opmv;
extern u8 OPMV_switch; 

//==��������

//static
static void OpenMV_Data_Analysis(u8 *buf_data,u8 len);
static void OpenMV_Check_Reset(void);

//public
void OpenMV_Offline_Check(u8 dT_ms);
void OpenMV_Byte_Get(u8 bytedata);

//OPMVģʽ�л�
void send_openmv_mode(unsigned  char data);
void send_openmv_mode_task(void);

#endif

