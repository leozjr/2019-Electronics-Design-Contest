#include "Ano_FlightCtrl.h"
#include "Ano_FlyCtrl.h"
#include "Ano_ProgramCtrl_User.h"
#include "Drv_OpenMV.h"

//��ȡ��ǰOPMV���ݣ� opmv.cb.����
u16 spd,val;
u8 fly_state = 0;
/**********************************************************************************************************
*�� �� ��: Cable_check()
*����˵��: ���¼��
*��    ��: ����ʱ��(ms)
*�� �� ֵ: ��
**********************************************************************************************************/
void Cable_check(u8 dT_ms)
{
	//���
		//����OPMV�� 2 �������������ռ��������send_openmv_mode(2)
	switch(fly_state)
	{
	//���
		case 0:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x01; 	//���
				fly_state = 1;
				OP_ok = 0; //�ȴ���һ�β������
			}
			break;
		}
		case 1:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x07;   //�ӽ�����
				spd = 10;
				val = 20;
				fly_state = 2;
				OP_ok = 0;
			}
			break;
		}
		case 2:
		{
			program_ctrl.cmd_state[0] = 0x05;   //ֱ��
																					//����ƫ�����
			fly_state = 2;
			
			break;
		}
		case 3:
		{   
			Program_Ctrl_User_Set_HXYcmps(0,0); //��ͣ
			//Todo: �ж��Ƿ���ͣ����֮ͣ����ok
			fly_state = 2;
			OP_ok = 0;
			break;
		}
		case 4:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x07;   //�����Ƹ�
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
				program_ctrl.cmd_state[0] = 0x02;   //����
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
*�� �� ��: Cable_check()
*����˵��: ���¼�鶨ʱ��
*��    ��: ����ʱ��(ms)
*�� �� ֵ: ��
**********************************************************************************************************/


void Cable_check_by_time(u8 dT_ms) 
{

	//����OPMV�� 2 �������������ռ��������send_openmv_mode(2)
	switch(fly_state)
	{
	//���
		case 0:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x01; 	//���
				fly_state = 1;
				OP_ok = 0;
			}
		}
		case 1:
		{
			if (OP_ok)
			{
				program_ctrl.cmd_state[0] = 0x05;   //ֱ��1
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
				program_ctrl.cmd_state[0] = 0x02;   //����
				fly_state = 3;
				OP_ok = 0;
			}
		}
			
	//��ƫ-��׼���������
	
	//����ģʽ2
	
	//����״̬��
	}
}