#ifndef __DRV_OPENMV_GROUND_H
#define __DRV_OPENMV_GROUND_H

#include "sysconfig.h"
#include "Ano_FcData.h"

//¶¨Òå

typedef struct
{
	u8 offline;
	u8 mode_cmd;
	u8 mode_sta;
	s16 d_angle; //º½Æ«½Ç
}G_openmv_data_st;
//
extern G_openmv_data_st G_opmv;
extern u16 Laser_height_cm;
//

//static
static void G_OpenMV_Data_Analysis(u8 *buf_data,u8 len);
static void G_OpenMV_Check_Reset(void);

//public
void G_OpenMV_Offline_Check(u8 dT_ms);
void G_OpenMV_Byte_Get(u8 bytedata);

#endif


