/*==========================================================================
 * 描述    ：整个比赛任务的控制与调度
						 
 
 * 更新时间：2019-08-02
 * 作者		 ：CAU-ZJR
*/

//默认引用
#include "Ano_OPMV_LineTracking_Ctrl.h"
#include "Ano_OPMV_CBTracking_Ctrl.h"
#include "Drv_OpenMV.h"
#include "CAU_task_Ctrl.h"
#include "Ano_Math.h"
#include "Ano_Filter.h"
#include "Ano_ProgramCtrl_User.h"
//
//数据接口定义：
//=========mapping===============
//需要引用的文件：
#include "Ano_FlightCtrl.h"
#include "CAU_Cable_check.h" //电缆检查过程部分
#include "Drv_OpenMv_Ground.h" 

//需要调用引用的外部变量：
#define RELATIVE_HEIGHT_CM           (jsdata.valid_of_alt_cm)  //相对高度


//需要操作赋值的外部变量：

//===============================
//全局变量：

//参数设定：
u8 go_once = 1;//执行一次的标志位

/**********************************************************************************************************
*函 数 名: go_2sec
*功能说明: 起飞前进2秒降落
*参    数: 周期时间(ms)
*返 回 值: 无
**********************************************************************************************************/
u16 timer = 0;
void takeoff_and_land(u8 dT_ms)//20ms
{	
	
	one_key_take_off();
	if(timer < 500)
		timer++;
	else
	{
		go_once = 0;
		one_key_land();
	}
}


//参数设定：
u8 task_ctrl = 0;//任务执行标志
/**********************************************************************************************************
*函 数 名: CAU_task_ctrl
*功能说明: 比赛任务控制任务
		task_ctrl是任务的切换控制标志，最终将由开关来控制，目前只是用flight_mode2来暂时试试效果
*参    数: 周期时间(ms)
*返 回 值: 无
**********************************************************************************************************/
void CAU_Ctrl_Task(u8 dT_ms)//20ms执行一次
{
	/*OPMV模式控制*/
	send_openmv_mode_task();
	
	//task_ctrl = msp.switch_sta;
	task_ctrl = flag.flight_mode2;
	switch(task_ctrl)
	{
		case 0:
				{
					//one_key_land();
					break;
				}
		case 2: 
				{
					if(go_once)
						takeoff_and_land(dT_ms);
					break;
				}
		case 1: 
				{
					Cable_check_by_time(dT_ms);
					break;
				}
		case 15:
				{
					Cau_reset();
					break;
				}
		default: break;
	}
}

void Cau_reset()
{
	go_once = 1;
}
