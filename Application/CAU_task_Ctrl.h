#ifndef __CAU_TASK_CTRL_H
#define __CAU_TASK_CTRL_H

//==引用
#include "sysconfig.h"
#include "Ano_FcData.h"

//==定义

//==数据声明

//static

//public
void CAU_Ctrl_Task(u8 dT_ms);
void OPMV_task_ctrl(u8 dT_ms);
void Cau_reset(void);

//测试函数
void takeoff_and_land(u8 dT_ms);//起飞悬停5秒，降落


#endif
