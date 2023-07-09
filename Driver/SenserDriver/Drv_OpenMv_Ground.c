//Ĭ�����ã�
#include "Drv_OpenMv_Ground.h"

//��������
#include "Drv_Uart.h"
#include "Ano_RC.h"
//�趨
#define G_OPMV_OFFLINE_TIME_MS  1000  //����
//����OPMVʹ�ô���5
//ȫ�ֱ���
u16 G_offline_check_time;
u16 Laser_height_cm;
u8 G_openmv_buf[20];
G_openmv_data_st G_opmv;
/**************************************************f********************************************************
*�� �� ��: OpenMV_Byte_Get
*����˵��: �Ե�OpenMV�ֽ����ݻ�ȡ
*��    ��: �ֽ�����
*�� �� ֵ: ��
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
		if(bytedata == 0x40) //������40������ƫ�ƽǶȼ��߶�
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
			//�����ɹ�
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
*�� �� ��: OpenMV_Data_Analysis
*����˵��: �Ե�OpenMV���ݽ���
*��    ��: �������ݣ��βΣ�������
*�� �� ֵ: ��
**********************************************************************************************************/
static void G_OpenMV_Data_Analysis(u8 *buf_data,u8 len)
{
	Laser_height_cm = *(buf_data+5); //��ȡ�߶�
	G_opmv.d_angle = (s16)((*(buf_data+6)<<8)|*(buf_data+7));//����ƫ��
	//
	G_OpenMV_Check_Reset();
}

/**********************************************************************************************************
*�� �� ��: OpenMV_Offline_Check
*����˵��: �Ե�OpenMV���߼�⣬�������Ӳ���Ƿ�����
*��    ��: ʱ�䣨���룩
*�� �� ֵ: ��
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
*�� �� ��: OpenMV_Check_Reset
*����˵��: �Ե�OpenMV���߼�⸴λ��֤��û�е���
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
static void G_OpenMV_Check_Reset()
{
	G_offline_check_time = 0;
	G_opmv.offline = 0;
}






