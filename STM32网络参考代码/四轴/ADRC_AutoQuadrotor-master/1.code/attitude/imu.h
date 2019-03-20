#ifndef IMU_H
#define IMU_H
//#include "includes.h"
#include "uav_filter.h"
class Filter;
class IMU : public Filter
{
	public:
			//ŷ���Ǳ�ʾ�ķ�������̬
	  Vector3f ea;      //��������ϵ���¸�������ļ��ٶ�
     u8 eaup[3];


	  Vector3f angle;
    Vector3f Acc_lpf;
	  
	  void Init(void);
    void Attitude( float dt);
	  void SensorSample(void);
	  IMU(){}
	private:
		int32_t accRatio;
	  float  factor;                    //�����˲�ϵ��
		float  AccSpeedfactor;            //�����˲�ϵ��
	  LPF2ndData_t Acc_lpf_2nd; //���ٶȶ����˲�
	
	
};

extern IMU imu;

#endif