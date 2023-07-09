//Ĭ�����ã�
#include "Drv_OpenMV.h"
//��������
#include "Drv_Uart.h"
#include "Ano_RC.h"
//�趨
#define OPMV_OFFLINE_TIME_MS  1000  //����
//����openmvʹ�ô���3
//ȫ�ֱ���
u16 offline_check_time;
u8 OPMV_switch; //����״̬
u8 openmv_buf[20];
_openmv_data_st opmv;
/**************************************************f********************************************************
*�� �� ��: OpenMV_Byte_Get
*����˵��: OpenMV�ֽ����ݻ�ȡ
*��    ��: �ֽ�����
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Byte_Get(u8 bytedata)
{	
	static u8 len = 0,rec_sta;
	u8 check_val=0;
	
	//
	openmv_buf[rec_sta] = bytedata;
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
		if(1)//(bytedata==0x29)δȷ��
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
		if(bytedata ==0x42)
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
			check_val += openmv_buf[i];
		}
		//
		if(check_val == bytedata)
		{
			//�����ɹ�
			OpenMV_Data_Analysis(openmv_buf,len+6);
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
*�� �� ��: OpenMV_Data_Analysis
*����˵��: OpenMV���ݽ���
*��    ��: �������ݣ��βΣ�������
*�� �� ֵ: ��
**********************************************************************************************************/
static void OpenMV_Data_Analysis(u8 *buf_data,u8 len)
{
	OPMV_switch = *(buf_data+5); //��ȡ���ر���
	
	if(*(buf_data+3)==0x42)
	{
		opmv.cb.sta = *(buf_data+6);
		opmv.cb.angle = *(buf_data+7);
		opmv.cb.go = *(buf_data+8);
		opmv.cb.turn = *(buf_data+9);
		opmv.cb.pos_x = (s16)((*(buf_data+12)<<8)|*(buf_data+13));
		opmv.cb.pos_y = (s16)((*(buf_data+14)<<8)|*(buf_data+15));
		//
		opmv.mode_sta = 1;
	}
	//
	OpenMV_Check_Reset();
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Offline_Check
*����˵��: OpenMV���߼�⣬�������Ӳ���Ƿ�����
*��    ��: ʱ�䣨���룩
*�� �� ֵ: ��
**********************************************************************************************************/
void OpenMV_Offline_Check(u8 dT_ms)
{
	if(offline_check_time<OPMV_OFFLINE_TIME_MS)
	{
		offline_check_time += dT_ms;
	}
	else
	{
		opmv.offline = 1;
		opmv.mode_sta = 0;
	}
	
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Check_Reset
*����˵��: OpenMV���߼�⸴λ��֤��û�е���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void OpenMV_Check_Reset()
{
	offline_check_time = 0;
	opmv.offline = 0;
}
u8 openmv_data[7];
void send_openmv_mode(unsigned  char data)
{
	openmv_data[0]=0xAA;
	openmv_data[1]=0xAF;
	openmv_data[2]=0x05;
	openmv_data[3]=0x01;
	openmv_data[4]=0x06;
	openmv_data[5]=data;
	u8 sum=0;
	for(int i=0;i<6;i++)
	{
		sum+=openmv_data[i];
	}
	openmv_data[6]=sum;
	Drv_Uart3SendBuf(openmv_data,7);
}
void send_openmv_mode_task()
{
	if(CH_N[AUX4]<-100 && CH_N[AUX3]<-100)
	{
		send_openmv_mode(1);
	}
	else if(CH_N[AUX4]>100 && CH_N[AUX3]>100)
	{
		send_openmv_mode(2);
	}
	else
	{
		send_openmv_mode(3);
	}
}


