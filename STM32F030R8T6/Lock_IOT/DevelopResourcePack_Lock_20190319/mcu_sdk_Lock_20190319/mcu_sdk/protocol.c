/****************************************Copyright (c)*************************
**                               版权�?�? (C), 2015-2020, 涂鸦科技
**
**                                 http://www.tuya.com
**
**--------------文件信息-------------------------------------------------------
**�?   �?   �?: protocol.c
**�?        �?: 下发/上报数据处理函数
**�? �? �? �? :

                  *******非常重要，一定要看哦！！�?********

** 1、用户在此文件中实现数据下发/上报功能
** 2、DP的ID/TYPE及数据处理函数都�?要用户按照实际定义实�?
** 3、当�?始某些宏定义后需要用户实现代码的函数内部�?#err提示,完成函数后请删除�?#err
**
**--------------当前版本修订---------------------------------------------------
** �?  �?: v1.0.2
** 日�??�?: 2018�?12�?6�?
** 描�??�?: 1:增加低功耗门锁api接口文件
           2:修改增加部分新功能兼�?

** �?  �?: v1.0.1
** 日�??�?: 2018�?11�?1�?
** 描�??�?: 1:低功耗SDK上线正式�?

** �?  �?: v1.0
** 日�??�?: 2018�?4�?24�?
** 描�??�?: 1:低功耗SDK首版，�?�配1.0.2协议
**
**-----------------------------------------------------------------------------
******************************************************************************/

#include "wifi.h"

/******************************************************************************
                                移植须知:
1:MCU必须在while中直接调用mcu_api.c内的wifi_uart_service()函数
2:程序正常初始化完成后,建议不进行关串口中断,如必须关中断,关中断时间必须短,关中断会引起串口数据包丢�?
3:请勿在中�?/定时器中断内调用上报函数
******************************************************************************/

         
/******************************************************************************
                              第一�?:初始�?
1:在需要使用到wifi相关文件的文件中include "wifi.h"
2:在MCU初始化中调用mcu_api.c文件中的wifi_protocol_init()函数
3:将MCU串口单字节发送函数填入protocol.c文件中uart_transmit_output函数�?,并删�?#error
4:在MCU串口接收函数中调用mcu_api.c文件内的uart_receive_input函数,并将接收到的字节作为参数传入
5:单片机进入while循环后调用mcu_api.c文件内的wifi_uart_service()函数
******************************************************************************/

/******************************************************************************
                        1:dp数据点序列类型对照表
          **此为自动生成代码,如在�?发平台有相关修改请重新下载MCU_SDK**         
******************************************************************************/
const DOWNLOAD_CMD_S download_cmd[] =
{
  {DPID_UNLOCK_TYPE, DP_TYPE_ENUM},
  {DPID_UNLOCK_ID, DP_TYPE_VALUE},
  {DPID_ALART, DP_TYPE_ENUM},
  {DPID_OPEN_DOOR_TIME, DP_TYPE_VALUE},
  {DPID_BATTERY, DP_TYPE_ENUM},
};



/******************************************************************************
                           2:串口单字节发送函�?
请将MCU串口发�?�函数填入该函数�?,并将接收到的数据作为参数传入串口发�?�函�?
******************************************************************************/

/*****************************************************************************
函数名称 : uart_transmit_output
功能描述 : 发数据处�?
输入参数 : value:串口收到字节数据
返回参数 : �?
使用说明 : 请将MCU串口发�?�函数填入该函数�?,并将接收到的数据作为参数传入串口发�?�函�?
*****************************************************************************/
void uart_transmit_output(unsigned char value)
{
  #error "请将MCU串口发�?�函数填入该函数,并删除该�?"
/*
  //示例:
  extern void Uart_PutChar(unsigned char value);
  Uart_PutChar(value);	                                //串口发�?�函�?
*/
}
/******************************************************************************
                           第二�?:实现具体用户函数
1:APP下发数据处理
2:数据上报处理
******************************************************************************/

/******************************************************************************
                            1:�?有数据上报处�?
当前函数处理全部数据上报(包括可下�?/可上报和只上�?)
  �?要用户按照实际情况实�?:
  1:�?要实现可下发/可上报数据点上报
  2:�?要实现只上报数据点上�?
此函数为MCU内部必须调用
用户也可调用此函数实现全部数据上�?
******************************************************************************/

//自动化生成数据上报函�?

/*****************************************************************************
函数名称 : all_data_update
功能描述 : 系统�?有dp点信息上�?,实现APP和muc数据同步
输入参数 : �?
返回参数 : �?
使用说明 : 此函数SDK内部�?调用;
           MCU必须实现该函数内数据上报功能;包括只上报和可上报可下发型数�?
*****************************************************************************/
void all_data_update(void)
{
  #error "请在此处理可下发可上报数据及只上报数据示�?,处理完成后删除该�?"
  /* 
  //此代码为平台自动生成，请按照实际数据修改每个可下发可上报函数和只上报函数
  mcu_dp_enum_update(DPID_UNLOCK_TYPE,��ǰ������ʽ); //ö���������ϱ�;
  mcu_dp_value_update(DPID_UNLOCK_ID,��ǰ����ID); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_ALART,��ǰ����); //ö���������ϱ�;
  mcu_dp_value_update(DPID_OPEN_DOOR_TIME,��ǰԶ�̿��ŵ���ʱ); //VALUE�������ϱ�;
  mcu_dp_enum_update(DPID_BATTERY,��ǰ����״̬); //ö���������ϱ�;

 */
}


/*****************************************************************************
函数名称 : dp_record_combine_update
功能描述 : 记录型数据组合上�?
输入参数 : time     : 时间数据长度7,首字节表示是否传输标志位，其余依次为年�?�月、日、时、分、秒
           dp_bool  : bool型dpid�?, v_bool:对应�?
           dp_enum  : enum型dpid�?, v_enum:对应�?
           dp_value : value型dpid�?, v_value:对应�?
           dp_string: string型dpid�?, v_string:对应�?,len:string长度
返回参数 : �?
*****************************************************************************/
unsigned char dp_record_combine_update(unsigned char time[],
                                       unsigned char dp_bool,unsigned char v_bool,
                                       unsigned char dp_enum,unsigned char v_enum,
                                       unsigned char dp_value,unsigned char v_value,
                                       unsigned char dp_string,unsigned char v_string[],unsigned char len)
{
  unsigned short length = 0;
  
  if(stop_update_flag == ENABLE)
    return SUCCESS;
  
  //local_time
  length = set_wifi_uart_buffer(length,(unsigned char *)time,7);
  
  //bool
  length = set_wifi_uart_byte(length,dp_bool);
  length = set_wifi_uart_byte(length,DP_TYPE_BOOL);
  length = set_wifi_uart_byte(length,0);
  length = set_wifi_uart_byte(length,1);
  if(v_bool == FALSE)
  {
    length = set_wifi_uart_byte(length,FALSE);
  }
  else
  {
    length = set_wifi_uart_byte(length,1);
  }
  //enum
  length = set_wifi_uart_byte(length,dp_enum);
  length = set_wifi_uart_byte(length,DP_TYPE_ENUM);
  length = set_wifi_uart_byte(length,0);
  length = set_wifi_uart_byte(length,1);
  length = set_wifi_uart_byte(length,v_enum);
  //value
  length = set_wifi_uart_byte(length,dp_value);
  length = set_wifi_uart_byte(length,DP_TYPE_VALUE);
  length = set_wifi_uart_byte(length,0);
  length = set_wifi_uart_byte(length,4);
  length = set_wifi_uart_byte(length,v_value >> 24);
  length = set_wifi_uart_byte(length,v_value >> 16);
  length = set_wifi_uart_byte(length,v_value >> 8);
  length = set_wifi_uart_byte(length,v_value & 0xff);
  //string
  length = set_wifi_uart_byte(length,dp_string);
  length = set_wifi_uart_byte(length,DP_TYPE_STRING);
  length = set_wifi_uart_byte(length,len / 0x100);
  length = set_wifi_uart_byte(length,len % 0x100);
  length = set_wifi_uart_buffer(length,(unsigned char *)v_string,len);
  
  wifi_uart_write_frame(STATE_RC_UPLOAD_CMD,length);
  
  return SUCCESS;
}

/******************************************************************************
                                WARNING!!!    
                            2:�?有数据上报处�?
自动化代码模板函�?,具体请用户自行实现数据处�?
******************************************************************************/



/******************************************************************************
                                WARNING!!!                     
此代码为SDK内部调用,请按照实际dp数据实现函数内部数据
******************************************************************************/
#ifdef SUPPORT_MCU_RTC_CHECK
/*****************************************************************************
函数名称 : mcu_write_rtctime
功能描述 : MCU校对本地RTC时钟
输入参数 : �?
返回参数 : �?
使用说明 : MCU�?要自行实现该功能
*****************************************************************************/
void mcu_write_rtctime(unsigned char time[])
{
  #error "请自行完成RTC时钟写入代码,并删除该�?"
  /*
  time[0]为是否获取时间成功标志，�? 0 表示失败，为 1表示成功
  time[1] �? �? �? , 0x00 �? �?2000 �?
  time[2]为月份，�? 1 �?始到12 结束
  time[3]为日期，�? 1 �?始到31 结束
  time[4]为时钟，�? 0 �?始到23 结束
  time[5]为分钟，�? 0 �?始到59 结束
  time[6]为秒钟，�? 0 �?始到59 结束
  time[7]为星期，�? 1 �?始到 7 结束�?1代表星期�?
 */
  if(time[0] == 1)
  {
    //正确接收到wifi模块返回的本地时钟数�? 
	 
  }
  else
  {
  	//获取本地时钟数据出错,有可能是当前wifi模块未联�?
  }
}
#endif

#ifdef WIFI_TEST_ENABLE
/*****************************************************************************
函数名称 : wifi_test_result
功能描述 : wifi功能测试反馈
输入参数 : result:wifi功能测试结果;0:失败/1:成功
           rssi:测试成功表示wifi信号强度/测试失败表示错误类型
返回参数 : �?
使用说明 : MCU�?要自行实现该功能
*****************************************************************************/
void wifi_test_result(unsigned char result,unsigned char rssi)
{
  #error "请自行实现wifi功能测试成功/失败代码,完成后请删除该行"
  if(result == 0)
  {
    //测试失败
    if(rssi == 0x00)
    {
      //未扫描到名称为tuya_mdev_test路由�?,请检�?
    }
    else if(rssi == 0x01)
    {
      //模块未授�?
    }
  }
  else
  {
    //测试成功
    //rssi为信号强�?(0-100, 0信号�?差，100信号�?�?)
  }
  
}
#endif

/*****************************************************************************
函数名称 : wifi_update_handle
功能描述 : MCU请求mcu固件升级返回函数
输入参数 : status:校验标志
返回参数 : �?
使用说明 : MCU主动调用 wifi_update_request 函数完成后该函数内可获取升级当前状�??
*****************************************************************************/
void wifi_update_handle(unsigned char status)
{ 
  #error "请自行完成wifi模块升级状�?�返回代�?,并删除该�?"

  switch (status)
  {
    case 0:
    {
      //�?始检查固件更�?
      break;
    }

    case 1:
    {
      //已经是最新固�?
      break;
    }

    case 2:
    {
      //正在更新固件
      break;
    }

    case 3:
    {
      //固件更新成功
      break;
    }

    case 4:
    {
      //固件更新失败
      break;
    }

    default:
      break;
  }
}

#ifdef SUPPORT_MCU_FIRM_UPDATE
/*****************************************************************************
函数名称 : mcu_firm_update_handle
功能描述 : MCU进入固件升级模式
输入参数 : value:固件缓冲�?
           position:当前数据包在于固件位�?
           length:当前固件包长�?(固件包长度为0�?,表示固件包发送完�?)
返回参数 : �?
使用说明 : MCU�?要自行实现该功能
*****************************************************************************/
unsigned char mcu_firm_update_handle(const unsigned char value[],unsigned long position,unsigned short length)
{
  #error "请自行完成MCU固件升级代码,完成后请删除该行"
  if(length == 0)
  {
    //固件数据发�?�完�?
  }
  else
  {
    //固件数据处理
  }
  
  return SUCCESS;
}
#endif

/******************************************************************************
                                WARNING!!!                     
以下函数用户请勿修改!!
******************************************************************************/

/*****************************************************************************
函数名称 : dp_download_handle
功能描述 : dp下发处理函数
输入参数 : dpid:DP序号
           value:dp数据缓冲区地�?
           length:dp数据长度
返回参数 : 成功返回:SUCCESS/失败返回:ERRO
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char dp_download_handle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
  /*********************************
  当前函数处理可下�?/可上报数据调�?                    
  具体函数内需要实现下发数据处�?
  完成用需要将处理结果反馈至APP�?,否则APP会认为下发失�?
  ***********************************/
  unsigned char ret;
  switch(dpid)
  {

  default:
    break;
  }
  return ret;
}
/*****************************************************************************
函数名称 : get_download_cmd_total
功能描述 : 获取�?有dp命令总和
输入参数 : �?
返回参数 : 下发命令总和
使用说明 : 该函数用户不能修�?
*****************************************************************************/
unsigned char get_download_cmd_total(void)
{
  return(sizeof(download_cmd) / sizeof(download_cmd[0]));
}
