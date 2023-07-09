//默认引用：
#include "Drv_OpenMv_Ground.h"

//新增引用
#include "Drv_Uart.h"
#include "Ano_RC.h"
//设定
#define G_OPMV_OFFLINE_TIME_MS  1000  //毫秒
//地面OPMV使用串口5
//全局变量
u16 G_offline_check_time;
u16 Laser_height_cm;
u8 G_openmv_buf[20];
G_openmv_data_st G_opmv;
/**************************************************f********************************************************
*函 数 名: OpenMV_Byte_Get
*功能说明: 对地OpenMV字节数据获取
*参    数: 字节数据
*返 回 值: 无
**********************************************************************************************************/
void G_OpenMV_Byte_Get(u8 bytedata)
{	
	static u8 len = 0,rec_sta;
	u8 check_val=0;
	
	//
	G_openmv_buf[rec_sta] = bytedata;
	//
	if(rec_sta==0)
	{
		if(bytedata==0xaa)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==1)
	{
		if(1)//(bytedata==0x29)未确定
		{
			rec_sta++;
		}	
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==2)
	{
		if(bytedata==0x05)
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==3)
	{
		if(bytedata == 0x40) //地面用40，发送偏移角度及高度
		{
			rec_sta++;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else if(rec_sta==4)
	{
		//
		len = bytedata;
		if(len<20)
		{
			rec_sta++;
		}		
		else
		{
			rec_sta=0;
		}
	}
	else if(rec_sta==(len+5))
	{
		//
		for(u8 i=0;i<len+5;i++)
		{
			check_val += G_openmv_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//解析成功
			G_OpenMV_Data_Analysis(G_openmv_buf,len+6);
			//
			rec_sta=0;
		}
		else
		{
			rec_sta=0;
		}		
	}
	else
	{
		//	
		rec_sta++;
	}
	
}

/**********************************************************************************************************
*函 数 名: OpenMV_Data_Analysis
*功能说明: 对地OpenMV数据解析
*参    数: 缓存数据（形参），长度
*返 回 值: 无
**********************************************************************************************************/
static void G_OpenMV_Data_Analysis(u8 *buf_data,u8 len)
{
	Laser_height_cm = *(buf_data+5); //获取高度
	G_opmv.d_angle = (s16)((*(buf_data+6)<<8)|*(buf_data+7));//航向偏角
	//
	G_OpenMV_Check_Reset();
}

/**********************************************************************************************************
*函 数 名: OpenMV_Offline_Check
*功能说明: 对地OpenMV掉线检测，用来检测硬件是否在线
*参    数: 时间（毫秒）
*返 回 值: 无
**********************************************************************************************************/
void G_OpenMV_Offline_Check(u8 dT_ms)
{
	if(G_offline_check_time<G_OPMV_OFFLINE_TIME_MS)
	{
		G_offline_check_time += dT_ms;
	}
	else
	{
		G_opmv.offline = 1;
		G_opmv.mode_sta = 0;
	}
	
}

/**********************************************************************************************************
*函 数 名: OpenMV_Check_Reset
*功能说明: 对地OpenMV掉线检测复位，证明没有掉线
*参    数: 无
*返 回 值: 无
**********************************************************************************************************/
static void G_OpenMV_Check_Reset()
{
	G_offline_check_time = 0;
	G_opmv.offline = 0;
}






