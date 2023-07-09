//#include "include.h"
#include "Drv_OPMV.h"
#include "Drv_Uart.h"
#include "TM4C123G.h"
#include "Ano_OF.h"
#include "Ano_FcData.h"
//#include "led.h"
//#include "position_ctrl.h"
//#include "ctrl.h"
s16 x_e,y_e,angle_z;
u8 camera_data[50];
u16 Laser_height_cm;//
u32 judgement;
float cam_position_x_e,cam_position_y_e;//
float cam_angle_z;
float camera_x,camera_y,tof_z;
CamDataStruct CamData;

//*******************************************
//void Ultrasonic_Init()
//{
//  Drv_Uart5Init(115200);			//串口5初始化，函数参数为波特率
//}



void Get_opmv_data()//在scheduler文件中
{
	if(CamData.receive_complete_frag == 1)
	{
		CamData.receive_complete_frag = 0;
		static float old_height = 0;
		
		u16 x_temp,y_temp,angle_temp;
		angle_temp=(u16)(CamData.rdata[0]&0x00FF);
		x_temp=(u16)(CamData.rdata[2]&0x00FF);
		y_temp=(u16)(CamData.rdata[4]&0x00FF);
		
		angle_z =(s16)(((CamData.rdata[1])<<8)|angle_temp);
		x_e =(s16)(((CamData.rdata[3])<<8)|x_temp);
		y_e =(s16)(((CamData.rdata[5])<<8)|y_temp);
		 
//		angle_z=CamData.rdata[1]*256+CamData.rdata[0];
//		x_e=CamData.rdata[3]*256+CamData.rdata[2];
//		y_e=CamData.rdata[5]*256+CamData.rdata[4];
//		angle_z=CamData.rdata[0];
//		x_e=CamData.rdata[2];
//		y_e=CamData.rdata[4];
		cam_position_x_e=y_e-160;
		cam_position_y_e=x_e-180;
		cam_angle_z=angle_z;
				
	}
	camera_x=((float)CamData.rdata[13]-48)*1000+((float)CamData.rdata[14]-48)*100+((float)CamData.rdata[15]-48)*10+((float)CamData.rdata[16]-48);
	camera_y=((float)CamData.rdata[17]-48)*1000+((float)CamData.rdata[18]-48)*100+((float)CamData.rdata[19]-48)*10+((float)CamData.rdata[20]-48);
	tof_z=((float)CamData.rdata[21]-48)*1000+((float)CamData.rdata[22]-48)*100+((float)CamData.rdata[23]-48)*10+((float)CamData.rdata[24]-48);
	
	sens_hd_check.tof_ok = 1;
	switchs.tof_on = 1;
	Laser_height_cm=tof_z/10;
}

u16 ultra_distance_old;
_height_st ultra;
u8 i=0;
u8 j=0;


void OPMV_Get(u8 Data)
{
		static u8 state_machine = 0;
		static u8 sumchkm = 0;
	if(state_machine ==0)
	{
		if(Data =='#')
		{
			state_machine = 1;
			sumchkm = Data;
		}
		else
			state_machine = 0;
	}//0xAA
	else if(state_machine == 1)
	{
		sumchkm += Data;
		CamData.length=Data;
		CamData.recive_num=0;
		if(CamData.length != 0)
			state_machine = 2;
		else
			state_machine = 3;
	}//LEN
	else if(state_machine == 2)
	{
		sumchkm += Data;
		CamData.rdata[CamData.recive_num++]=Data;
		if(CamData.recive_num >= CamData.length)
			state_machine = 3;
	}//DATA
	else if(state_machine == 3)
	{
		if(sumchkm == Data)
			CamData.receive_complete_frag=1;
		else
			state_machine = 0;
	}//check sum
	
//	
//	if(state==0&&com_data=='#')
//	{
//		i=0;
//		state=1;
//	}
//	else if(state==1&&com_data!='*')
//	{
//		state=1;
//		camera_data[i]=com_data;
//		i++;
//	}
//	else if(state==1&&com_data=='*')
//	{
//		state=0;
//		cam_angle_z=((camera_data[0]-48)*100+(camera_data[1]-48)*10+(camera_data[2]-48))-100;
////		cam_position_y_e=((camera_data[3]-48)*100+(camera_data[4]-48)*10+(camera_data[5]-48))-180;//先给
////		cam_position_x_e=((camera_data[6]-48)*100+(camera_data[7]-48)*10+(camera_data[8]-48))-160;
//		cam_position_y_e=((camera_data[3]-48)*100+(camera_data[4]-48)*10+(camera_data[5]-48))-140;//先给
//		cam_position_x_e=((camera_data[6]-48)*100+(camera_data[7]-48)*10+(camera_data[8]-48))-130;
//		//judgement=(camera_data[12]-48)*100+(camera_data[13]-48)*10+(camera_data[14]-48);//判断是否要拐弯
//		//if(judgement==1)
//		//{
//		//	j++;
//		//}
//		//if(j==1)
//	//{
//		//	except_A.z=135;//遇到拐弯则直接给45度
//		//}
//	}
//	else
//		state = 0;
////	except_A.z=1000*(i+1);
	
}
