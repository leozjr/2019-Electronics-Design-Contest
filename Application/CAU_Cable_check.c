#include "Ano_FlightCtrl.h"
#include "Ano_FlyCtrl.h"
#include "Ano_ProgramCtrl_User.h"
#include "Drv_OpenMV.h"

//获取当前OPMV数据： opmv.cb.……
u16 spd,val;
u8 fly_state = 0;
/**********************************************************************************************************
*函 数 名: Cable_check()
*功能说明: 电缆检查
*参    数: 周期时间(ms)
*返 回 值: 无
**********************************************************************************************************/
void Cable_check(u8 dT_ms)
{
	//起飞
		//发送OPMV给 2 来启动计数拍照及其他命令：send_openmv_mode(2)
	switch(fly_state)
	{
	//起飞
		case 0:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x01; 	//起飞
				fly_state = 1;
				OP_ok = 0; //等待上一次操作完成
			}
			break;
		}
		case 1:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x07;   //接近电缆
				spd = 10;
				val = 20;
				fly_state = 2;
				OP_ok = 0;
			}
			break;
		}
		case 2:
		{
			program_ctrl.cmd_state[0] = 0x05;   //直行
																					//左右偏差控制
			fly_state = 2;
			
			break;
		}
		case 3:
		{   
			Program_Ctrl_User_Set_HXYcmps(0,0); //悬停
			//Todo: 判断是否悬停，悬停之后则ok
			fly_state = 2;
			OP_ok = 0;
			break;
		}
		case 4:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x07;   //向左绕杆
				spd = 20;
				val = 80;
				fly_state = 2;
				OP_ok = 0;
			}
			break;
		}
		case 9:
		{
			if(OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x02;   //降落
				fly_state = 3;
				OP_ok = 0;
			}
			break;
		}
			
	}
	
	//
	
	//
}

/**********************************************************************************************************
*函 数 名: Cable_check()
*功能说明: 电缆检查定时版
*参    数: 周期时间(ms)
*返 回 值: 无
**********************************************************************************************************/


void Cable_check_by_time(u8 dT_ms) 
{

	//发送OPMV给 2 来启动计数拍照及其他命令：send_openmv_mode(2)
	switch(fly_state)
	{
	//起飞
		case 0:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x01; 	//起飞
				fly_state = 1;
				OP_ok = 0;
			}
		}
		case 1:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x05;   //直行1
				spd = 20;
				val = 100;
				fly_state = 2;
				OP_ok = 0;
			}
		}
		case 2:
		{
			if(OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x02;   //降落
				fly_state = 3;
				OP_ok = 0;
			}
		}
			
	//左偏-》准备工作完成
	
	//发送模式2
	
	//进入状态机
	}
}