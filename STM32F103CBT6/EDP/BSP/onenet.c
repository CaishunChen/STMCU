/**

	*	˵���� 		��onenetƽ̨�����ݽ����ӿڲ�
	*
	*	�޸ļ�¼��	V1.0��Э���װ�������ж϶���ͬһ���ļ������Ҳ�ͬЭ��ӿڲ�ͬ��
	*				V1.1���ṩͳһ�ӿڹ�Ӧ�ò�ʹ�ã����ݲ�ͬЭ���ļ�����װЭ����ص����ݡ�
	************************************************************
**/


#include "esp8266.h"

#include "gpio.h"

#include "onenet.h"
#include "edpkit.h"

//C��
#include <string.h>
#include <stdio.h>

#include "bsp_led.h"


#define DEVID	"518514833"
#define APIKEY	"865ZyHxWrl0YL2cxFQ6rI5=AV90="

extern WIFI_Frame_Typedef wifi_frame;

//==========================================================
//	�������ƣ�	OneNet_DevLink
//	�������ܣ�	��onenet��������
//	���ز�����	1-�ɹ�	0-ʧ��
//	˵����		��onenetƽ̨��������
//==========================================================
uint8_t OneNet_DevLink(void)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};				//Э���
	unsigned char *dataPtr;
	uint8_t OneNet_Flag=0;
	
		if( 	EDP_PacketConnect1(DEVID, APIKEY, 256, &edpPacket) ==0  )
		{
					while( WIFI_SendData(edpPacket._data, edpPacket._len) )
					{
						memset(wifi_frame.RX_Buffer,0,WIFI_BUF_LEN);;
					}	
					memset(wifi_frame.RX_Buffer,0,WIFI_BUF_LEN);
					dataPtr = WIFI_GetIPD(500);
					if(dataPtr != NULL)
					{
							if(EDP_UnPacketRecv(dataPtr) == CONNRESP)
							{
									EDP_DeleteBuffer(&edpPacket);								//ɾ��
									OneNet_Flag = EDP_UnPacketConnectRsp(dataPtr) ;
							}
					}
		}
		
		return OneNet_Flag;
}

unsigned char OneNet_FillBuf(char *buf)
{
	char text[16];
	uint8_t LED2_State=0,LED3_State=0,LED4_State=0;
	
	LED2_State  = LL_GPIO_IsOutputPinSet(LED2_GPIO_Port,LED2_Pin);
	LED2_State  = !LED2_State;
	
	LED3_State  = LL_GPIO_IsOutputPinSet(LED3_GPIO_Port,LED3_Pin);
	LED3_State  = !LED3_State;
	
	LED4_State  = LL_GPIO_IsOutputPinSet(LED4_GPIO_Port,LED4_Pin);
	LED4_State  = !LED4_State;	
	
	memset(text, 0, sizeof(text));
	
	strcpy(buf, "{");
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"LED1\":%d,", LED2_State );
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"LED2\":%d,", LED3_State );
	strcat(buf, text);
	
	memset(text, 0, sizeof(text));
	sprintf(text, "\"LED3\":%d",  LED4_State );
	strcat(buf, text);
	
	
	strcat(buf, "}");
	
	return strlen(buf);
}

//==========================================================
//	�������ƣ�	OneNet_SendData
//	�������ܣ�	�ϴ����ݵ�ƽ̨
//	��ڲ�����	type���������ݵĸ�ʽ		
//==========================================================
void OneNet_SendData(void)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};												//Э���
	char buf[128];
	short body_len = 0, i = 0;
	
	memset(buf, 0, sizeof(buf));
	
	body_len = OneNet_FillBuf(buf);																	//��ȡ��ǰ��Ҫ���͵����������ܳ���
	
	if(body_len)
	{
			if(EDP_PacketSaveData(DEVID, body_len, NULL, kTypeSimpleJsonWithoutTime, &edpPacket) == 0)	//���
			{
				for(; i < body_len; i++)
					{
						edpPacket._data[edpPacket._len++] = buf[i];
					}
					WIFI_SendData(edpPacket._data, edpPacket._len);										//�ϴ����ݵ�ƽ̨
					EDP_DeleteBuffer(&edpPacket);															//ɾ��
			}
	}
}



//==========================================================
//	�������ƣ�	OneNet_RevPro
//	�������ܣ�	ƽ̨�������ݼ��
//	��ڲ�����	dataPtr��ƽ̨���ص�����
//==========================================================
void OneNet_RevPro(unsigned char *cmd)
{
	
	EDP_PACKET_STRUCTURE edpPacket = {NULL, 0, 0, 0};	//Э���
	
	char *cmdid_devid = NULL;
	unsigned short cmdid_len = 0;
	char *req = NULL;
	unsigned int req_len = 0;
	unsigned char type = 0;
	
	short result = 0;

	char *dataPtr = NULL;
	char numBuf[10];
	int num = 0;
	
	type = EDP_UnPacketRecv(cmd);
	switch(type)										//�ж���pushdata���������·�
	{
		case CMDREQ:									//�������
			result = EDP_UnPacketCmd(cmd, &cmdid_devid, &cmdid_len, &req, &req_len);
			if(result == 0)								//����ɹ������������ظ������
			{
				EDP_PacketCmdResp(cmdid_devid, cmdid_len, req, req_len, &edpPacket);
			}
		break;
			
		case SAVEACK:
		break;
			
		default:
			result = -1;
		break;
	}
	
//	memset(wifi_frame.RX_Buffer,0,WIFI_BUF_LEN);
	
	if(result == -1)
		return;
	
	dataPtr = strchr(req, '{');							//����'{'
	if(dataPtr != NULL)									//����ҵ���
	{
		dataPtr = strstr((char *)req, "LED1");	
		dataPtr+=4;
  while(*dataPtr>'0'&&*dataPtr<'9') 
		{
		 numBuf[num++] = *dataPtr++;
		}
		if( dataPtr )				//����"LED"
		{
			num = atoi((const char *)numBuf);				//תΪ��ֵ��ʽ
			if(num == 1)								//�����������Ϊ1������
			{
				LED2_ON;
			}
			else if(num == 0)							//�����������Ϊ0�������
			{
				LED2_OFF;
			}
		}
		else if(strstr((char *)req, "LED2"))
		{
			if(num == 1)
			{
				LED3_ON;
			}
			else if(num == 0)
			{
				LED3_OFF;
			}
		}
		else if(strstr((char *)req, "LED3"))
		{
			if(num == 1)
			{
				LED4_ON;
			}
			else if(num == 0)
			{
				LED4_OFF;
			}
		}
	
	}
	
	if(type == CMDREQ && result == 0)						//���������� �� ����ɹ�
	{
		EDP_FreeBuffer(cmdid_devid);						//�ͷ��ڴ�
		EDP_FreeBuffer(req);
															//�ظ�����
		WIFI_SendData(edpPacket._data, edpPacket._len);	//�ϴ�ƽ̨
		EDP_DeleteBuffer(&edpPacket);						//ɾ��
	}
	 memset(wifi_frame.RX_Buffer,0,WIFI_BUF_LEN);
	 wifi_frame.Pointer=0;
}

