#ifndef DEV_GPS_H
#define DEV_GPS_H
#include "stm32f4xx.h"
#include "string.h"

	//������Ϣ
	__packed typedef struct  
	{										    
		u8 num;		//���Ǳ��
		u8 eledeg;	//��������
		u16 azideg;	//���Ƿ�λ��
		u8 sn;		//�����		   
	}nmea_slmsg;  
	//UTCʱ����Ϣ
	__packed typedef struct  
	{										    
		u16 year;	//���
		u8 month;	//�·�
		u8 date;	//����
		u8 hour; 	//Сʱ
		u8 min; 	//����
		u8 sec; 	//����
	}nmea_utc_time;   	   
	//NMEA 0183 Э����������ݴ�Žṹ��
	__packed typedef struct  
	{										    
		u8 svnum;					//�ɼ�������
		nmea_slmsg slmsg[12];		//���12������
		nmea_utc_time utc;			//UTCʱ��
		u32 latitude;				//γ�� ������100000��,ʵ��Ҫ����100000 
		u8 nshemi;					//��γ/��γ,N:��γ;S:��γ				  
		u32 longitude;			    //���� ������100000��,ʵ��Ҫ����100000
		u8 ewhemi;					//����/����,E:����;W:����
		u8 gpssta;					//GPS״̬:0,δ��λ;1,�ǲ�ֶ�λ;2,��ֶ�λ;6,���ڹ���.				  
		u8 posslnum;				//���ڶ�λ��������,0~12.
		u8 possl[12];				//���ڶ�λ�����Ǳ��
		u8 fixmode;					//��λ����:1,û�ж�λ;2,2D��λ;3,3D��λ
		u16 pdop;					//λ�þ������� 0~500,��Ӧʵ��ֵ0~50.0
		u16 hdop;					//ˮƽ�������� 0~500,��Ӧʵ��ֵ0~50.0
		u16 vdop;					//��ֱ�������� 0~500,��Ӧʵ��ֵ0~50.0 

		int altitude;			 	//���θ߶�,�Ŵ���10��,ʵ�ʳ���10.��λ:0.1m	 
		u16 speed;					//��������,�Ŵ���1000��,ʵ�ʳ���10.��λ:0.001����/Сʱ	 
	}nmea_msg;

	struct vehicle_gps_position_s {

	uint64_t timestamp; // required for logger
	uint64_t time_utc_usec;
	int32_t lat;
	int32_t lon;
	int32_t alt;
	int32_t alt_ellipsoid;
	float s_variance_m_s;
	float c_variance_rad;
	float eph;
	float epv;
	float hdop;
	float vdop;
	int32_t noise_per_ms;
	int32_t jamming_indicator;
	float vel_m_s;
	float vel_n_m_s;
	float vel_e_m_s;
	float vel_d_m_s;
	float cog_rad;
	int32_t timestamp_time_relative;
	uint8_t fix_type;
	bool vel_ned_valid;
	uint8_t satellites_used;
	uint8_t _padding0[5]; // required for logger
};

	
	
extern 	nmea_msg gpsx; 

//��������ӿ�
void GPS_DataCacheCall(u8 data);
void GPS_Call(void);	
#endif