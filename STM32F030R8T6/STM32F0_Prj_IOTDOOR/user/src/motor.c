#include "motor.h"

static uint8  MotorOverCount = 0;
static uint32 MotorForceCount = 0;
/////�����ת
void MotorForward(void)
{
#if (MOTORDIR == 1)
	MOTOR_AHIGH;
#elif (MOTORDIR == 0)
	MOTOR_BLOW;
	MOTOR_BHIGH;
	MOTOR_ALOW;
#endif	
}

//////�����ת
void MotorBackward(void)
{
#if (MOTORDIR == 0)
	MOTOR_AHIGH;
	MOTOR_BLOW;
#elif (MOTORDIR == 1)
	MOTOR_BHIGH;
	MOTOR_ALOW;
#endif	
}

/////���ͣ
void MotorStop(void)
{
	MOTOR_BHIGH;
	MOTOR_AHIGH;
}

/////////����
void DoorOpen(SystemDatatypedef * q)
{
	MotorOverCount = 0;
	MotorForceCount = 0;
	MotorForward();
	delay_ms(MOTOR_ELECURDELAY);
	while((MotorOverCount < MOTOR_ELECUROVERCOUNT)
    		&& (MotorForceCount < MOTOR_MOTORFORCELIMIT))
	{
		if(q->ADBuff[0] > MOTOR_ELECUROVER)
		{
			MotorOverCount++;
		}
		MotorForceCount++;
	}
        MotorStop();
        delay_ms(300);
	MotorBackward();
	delay_ms(MOTOR_RETURNHALFCOUNT);
	MotorStop();
}

//////����
void DoorClose(SystemDatatypedef * q)
{
	MotorOverCount = 0;
	MotorForceCount = 0;
	MotorBackward();
	delay_ms(MOTOR_ELECURDELAY);
	while((MotorOverCount < MOTOR_ELECUROVERCOUNT)
    		&& (MotorForceCount < MOTOR_MOTORFORCELIMIT))
	{
		if(q->ADBuff[0] > MOTOR_ELECUROVER)
		{
			MotorOverCount++;
		}
		MotorForceCount++;
	}
	MotorStop();
}

