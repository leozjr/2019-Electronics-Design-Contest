#ifndef __CAU_TASK_CTRL_H
#define __CAU_TASK_CTRL_H

//==����
#include "sysconfig.h"
#include "Ano_FcData.h"

//==����

//==��������

//static

//public
void CAU_Ctrl_Task(u8 dT_ms);
void OPMV_task_ctrl(u8 dT_ms);
void Cau_reset(void);

//���Ժ���
void takeoff_and_land(u8 dT_ms);//�����ͣ5�룬����


#endif
