/*
  ******************************************************************************
  * @file   
  * @author  ytf
  * @version 
  * @date   
  * @brief   ����ִ���ļ������е����������ִ��
  *    
  *****************************************************************************
*/
#include "includes.h"
#include "imu.h"
#include "stdio.h"
#include "dev_clock.h"
#include "dev_mavlink.h"
#include  "drv_w25qxx.h"
#include "cal_vector3.h"
#include "uav_parameter.h"
#include "dev_sd.h"
#include "dev_flash.h"	
#include "uav_rc.h"
#include "dev_gps.h"
#include "uav_control.h"
#include "uav_attitude.h"
#include "dev_us100.h"
#include "uav_parameter_sd.h"
#include "uav_adrc.h"
#include "nav_speed.h"
#include "nav_h.h"
#include "control_v.h"
//#include "ff.h"
//#include "stdlib.h"

#include "AttitudePositionEstimatorEKF.h"
/*
1.�����Ǳ���������ϵ
2.6050�Ƕ���������ϵ
3.ak8975�Ǳ���������ϵ
*/

#define scaf   9.086/4096

Vector3f UAV;


//==================================
void task_1_ms(void)
{
	

	
}
extern void AngularRote(float dt);
void task_2_ms(void)
{
		//���ݴ���
   Mavlink.Data_Exchange();

  float dt=Get_Cycle_T(1);
  //���������ݸ���
	 imu.SensorSample();
	 //adrc �ڻ�
   Adrc_.Angular(dt);
		//�����ٶ� λ��
}
void task_5_ms(void)
{
	 float dt=Get_Cycle_T(2);
 //ң�����ݹ���
	Rc.DataMange(dt);
  //��̬����
	// imu.Attitude(dt);
	 att_ekf.loop_up();
	 Adrc_.Angle(dt);
	 nav_v.get_a_v(dt);
	 nav_v.east(dt);
	 ctrl.V();
  
	
}

void task_8_ms()
{
 float dt=Get_Cycle_T(0);
	

};
void task_10_ms(void)
{
	 float dt=Get_Cycle_T(3);
  	  //��ѹ�����ݸ���
	 if( Ms5611.DataUpdate()){
	  nav_v.get_ms_v(dt);
	 }

	 //�������ݸ���
	 Ak8975.DataSample();
  //�Ƕȿ���
		 

	// Control.Angle(dt);
		 
	//λ�ÿ���
   Attitude.DISControl(dt);
}
void task_20_ms(void)
{

	//GPS ���ݸ���
	GPS_Call();
}

void task_50_ms(void)
{
 //���������ݲ�������
	Us100.SampleTriger(1);
	
}


void TaskLoop(void)
{
		//1ms ����
	  task_1_ms();

	 if(loop.cnt_2ms>=2)
	{
	 //2ms ����
	 task_2_ms();
		loop.cnt_2ms=0;
	}
	if(loop.cnt_5ms>=5)
	{
		//5ms ����
	  //task_5_ms();
		loop.cnt_5ms=0  ;
	}
		if(loop.cnt_8ms>=8)
	{
		//5ms ����
	  task_8_ms();
		loop.cnt_8ms=0;
	}
	
	if(loop.cnt_10ms>=10)
	{
		//10ms ����
	  task_10_ms();
		loop.cnt_10ms=0;
	}
  if(loop.cnt_20ms>=20)
	{
		 //20ms ����
	  task_20_ms();
		loop.cnt_20ms=0;
	}
  if(loop.cnt_50ms>=50)
	{
		 //50ms ����
	  task_50_ms();
		loop.cnt_50ms=0;
	}

}


int counT=0;
int up=1;
int main(void )
{ 
 	SysTick_Configuration();
  Uav.Init();
  imu.Init();
  //parameter_sd.SaveDataInit();
	while(1)
	{
	//������ѯ	
	 TaskLoop();
		
		if(counT>=10&&up==1)
		{
			//counT--;
		  up=0;
		}
		if(counT<=-10&&up==0)
		{
		// counT++;
			up=1;
		}
		if(up==1)
			  counT++;
			if(up==0)
			  counT--;
	}
}
	
	
	
	
	
	
	
	
	




