/*==========================================================================
 * ����    ��������������Ŀ��������
						 
 
 * ����ʱ�䣺2019-08-02
 * ����		 ��CAU-ZJR
*/

//Ĭ������
#include "Ano_OPMV_LineTracking_Ctrl.h"
#include "Ano_OPMV_CBTracking_Ctrl.h"
#include "Drv_OpenMV.h"
#include "CAU_task_Ctrl.h"
#include "Ano_Math.h"
#include "Ano_Filter.h"
#include "Ano_ProgramCtrl_User.h"
//
//���ݽӿڶ��壺
//=========mapping===============
//��Ҫ���õ��ļ���
#include "Ano_FlightCtrl.h"
#include "CAU_Cable_check.h" //���¼����̲���
#include "Drv_OpenMv_Ground.h" 

//��Ҫ�������õ��ⲿ������
#define RELATIVE_HEIGHT_CM           (jsdata.valid_of_alt_cm)  //��Ը߶�


//��Ҫ������ֵ���ⲿ������

//===============================
//ȫ�ֱ�����

//�����趨��
u8 go_once = 1;//ִ��һ�εı�־λ

/**********************************************************************************************************
*�� �� ��: go_2sec
*����˵��: ���ǰ��2�뽵��
*��    ��: ����ʱ��(ms)
*�� �� ֵ: ��
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


//�����趨��
u8 task_ctrl = 0;//����ִ�б�־
/**********************************************************************************************************
*�� �� ��: CAU_task_ctrl
*����˵��: ���������������
		task_ctrl��������л����Ʊ�־�����ս��ɿ��������ƣ�Ŀǰֻ����flight_mode2����ʱ����Ч��
*��    ��: ����ʱ��(ms)
*�� �� ֵ: ��
**********************************************************************************************************/
void CAU_Ctrl_Task(u8 dT_ms)//20msִ��һ��
{
	/*OPMVģʽ����*/
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
