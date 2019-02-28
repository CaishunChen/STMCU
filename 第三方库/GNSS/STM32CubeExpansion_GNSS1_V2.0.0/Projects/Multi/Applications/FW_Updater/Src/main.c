/**
  *******************************************************************************
  * @file    Applications/FW_Updater/Src/main.c
  * @author  AST/CL
  * @version V2.0.0
  * @date    Apr-2018
  * @brief   Application to be loaded on an STM32 Nucleo board in order to 
  *          use the tool for updating the GNSS Teseo-LIV3F firmware on the 
  *          X-NUCLEO-GNSS1A1 expansion board.
  *

  * \section FW_Updater_Example_Description Example Description
   FW_Updater is the application to be loaded in order to use the tool for
   updating the GNSS Teseo-LIV3F firmware on the X-NUCLEO-GNSS1A1 expansion boards
   (see readme.txt in folder Utilities/PC_Software/FirmwareUpdaterTool).

   Plug the X-NUCLEO-GNSS1A1 expansion board on top of a STM32 Nucleo-F401RE.
   Connect the STM32 Nucleo board to your PC.
   Drag and drop FW_Updater-*.bin (in Binary folder) on Nucleo drive.
   Run the java tool FWUPG.jar (in Utilities/PC_Software/FirmwareUpdaterTool folder).
   From the java GUI, after selecting the right serial port, click Open to start a 
   connection with your STM32 Nucleo and X-NUCLEO-GNSS1A1 boards.
   If the FW version on the Teseo-LIV3F module is not the latest one, click the 
   Update FW >>> button to start the firmware upgrading process.

  * \section FW_Updater_HW_SW_Env Hardware and Software environment

  - This example runs on STM32 Nucleo devices with GNSS STM32 expansion board
    (X-NUCLEO-GNSS1A1)
  - This example has been tested with STMicroelectronics:
    - NUCLEO-F401RE RevC board
    and can be easily tailored to any other supported device and development board.
    This example runs also on the NUCLEO-F411RE RevC board, even if the chip could
    be not exploited at its best since the projects are configured for the
    NUCLEO-F401RE target board.

  * \section FW_Updater_Usage Usage 

 In order to make the program work, you must do the following:
 - WARNING: before opening the project with any toolchain be sure your folder
   installation path is not too in-depth since the toolchain may report errors
   after building.
 - Open IAR toolchain (this firmware has been successfully tested with
   Embedded Workbench V7.80.4).
   Alternatively you can use the Keil uVision toolchain (this firmware
   has been successfully tested with V5.24) or the System Workbench for
   STM32 (this firmware has been successfully tested with Version 2.3.1).
 - Rebuild all files and load your image into target memory.
 - Run the example.
 - Alternatively, you can download the pre-built binaries in "Binary" 
   folder included in the distributed package.
  * 
  *******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        www.st.com/software_license_agreement_liberty_v2
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ********************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f4xx_nucleo.h"

#include "gnss_app_cfg.h"

#include "config_bus.h"
#include "x_nucleo_gnss1a1.h"

/* Private defines -----------------------------------------------------------*/
//#define PRINT_BYTES_ON_TERMINAL_IO  // if defined enable the print of the data bytes on Terminal I/O
//#define PRINT_MSG_ON_TERMINAL_IO    // if defined enable the print of the message logs on Terminal I/O
//#define PRINT_TIME_ON_TERMINAL_IO   // if defined enable the print of the time logs on Terminal I/O
    
#ifdef PRINT_BYTES_ON_TERMINAL_IO
  #define PRINTBYTES PrintBytes
#else
  #define PRINTBYTES(format, args...) ((void)0)
#endif   
   
#ifdef PRINT_MSG_ON_TERMINAL_IO
  #define PRINTMSG printf
#else
  #define PRINTMSG(format, args...)   ((void)0)
#endif

#ifdef PRINT_TIME_ON_TERMINAL_IO
  #define SETTIME SetTime
  #define PRINTTIME PrintTime 
#else
  #define SETTIME(format, args...) ((void)0)
  #define PRINTTIME(format, args...) ((void)0) 
#endif    

#ifdef PRINT_BYTES_ON_TERMINAL_IO
void PrintBytes(char* direction, uint8_t* buf, uint32_t len);
#endif

#ifdef PRINT_TIME_ON_TERMINAL_IO
void SetTime(uint32_t* time);
void PrintTime(char* direction, uint32_t time0, uint32_t time1);
#endif

/* Short timeout for an UART write (in ms) */
#define UART_WRITE_SHORT_TIMEOUT     (10)
/* Timeout for an UART write (in ms) */
#define UART_WRITE_TIMEOUT           (100)
/* Long timeout for an UART write (in ms) */
#define UART_WRITE_LONG_TIMEOUT      (1000)
/* Very short timeout for an UART read (in ms) */
#define UART_READ_VERY_SHORT_TIMEOUT (1)
/* Timeout for an UART read (in ms) */
#define UART_READ_TIMEOUT            (100)
/* Long timeout for an UART read (in ms) */
#define UART_READ_LONG_TIMEOUT       (3000)

/* Timeout used for some particular operations */
#define UART_READ_STRING_TIMEOUT     (2000)
#define UART_READ_READY_ACK_TIMEOUT  (8000)
#define UART_READ_CHUNK_ACK_TIMEOUT  (3500)
#define UART_READ_CRC_ACK_TIMEOUT    (10000)

#define ACK  0xCC
#define NACK 0xDD
#define IDENTIFIER_CMD_LEN      4
#define START_COMMUNICATION_LEN 1
#define IMAGE_INFO_LEN          20
#define FLASHER_READY_CMD_LEN   1

/* Size of Reception buffer */
#define UART_HEADER_SZ        2
#define UART_FWUPG_CTRL_SZ    5
#define UART_FWUPG_SYNC_SZ    26
#define RXBUFFER_SZ           16*1024
#define VER_BUF_SZ            32

/* Global variables ---------------------------------------------------------*/

/* Instance of GNSS Handler */
GNSS_HandleTypeDef pGNSS;

extern UART_HandleTypeDef huart;
extern UART_HandleTypeDef io_huart;

/* Private variables ---------------------------------------------------------*/

/* Buffers used for reception and transmission */
static uint8_t syncBuffer[UART_FWUPG_SYNC_SZ];
static uint8_t rxBuffer[RXBUFFER_SZ];
static char    ver_buf[VER_BUF_SZ];
static int already_flashed = 0;

/* Private function prototypes -----------------------------------------------*/
static void GPIO_Config(void);
static void SystemClock_Config(void);

static void Handle_Upgrading_Process(void);
static char* Get_GNSS_FW_Version(void);
static char* TESEO_Detect_Sentence(const char *rsp, uint8_t *buf, uint32_t len);

/* Global function prototypes -----------------------------------------------*/
void Error_Handler(void);

int main(void)
{  
  HAL_StatusTypeDef status;  
  uint8_t uart_firstcmd[UART_FWUPG_SYNC_SZ];
  uint16_t len;
  const char* fwupg = "START";
  char* ver = NULL;

  /* MCU Configuration----------------------------------------------------------*/
  
  /* Reset all peripherals and initialize the Flash interface and the Systick. */
  HAL_Init();
  
  /* Configure the system clock */
  SystemClock_Config();

  GPIO_Config();
  IO_UART_Init();

  /* Single instance for the GPS driver */
  GNSS_Init(&pGNSS, GNSS_BUS_UART);

  GNSS_Reset();
  GNSS_UART_Init(DEFAULT_UART_BAUD_RATE);

  /* Get current FW version */
  (void)memset(ver_buf, 0, VER_BUF_SZ);
  ver = Get_GNSS_FW_Version();
  (uint16_t)strlen(ver) > (uint16_t)VER_BUF_SZ ? (len = (uint16_t)VER_BUF_SZ) : (len = (uint16_t)strlen(ver));
  (void)memcpy(ver_buf, ver, (uint32_t)len);
  PRINTMSG("fw version [%d] = %s\n", strlen(ver), ver);
  
  /* Clear the buffer for the first command */
  (void)memset(uart_firstcmd, 0, UART_FWUPG_SYNC_SZ);
  
  /* Infinite loop */
  while (1)
  {
    /* Read uart header (it contains the len of the payload) */
    status = HAL_UART_Receive(&io_huart, (uint8_t *)uart_firstcmd, UART_HEADER_SZ, UART_READ_TIMEOUT);
    if (status != HAL_OK && status != HAL_TIMEOUT)
    {
      Error_Handler();
    }
    len = ((uint16_t)uart_firstcmd[1]<<8 | uart_firstcmd[0]);

    if (len == (uint16_t)UART_FWUPG_CTRL_SZ)
    {
      /* Read here the uart payload */
      status = HAL_UART_Receive(&io_huart, (uint8_t *)uart_firstcmd, len, UART_READ_TIMEOUT);
      if (status != HAL_OK)
      {
        Error_Handler();
      }
      /**
       * The current command is received when the COM port with the Java tool either is opened 
       * for the first time or is closed and then re-opened.
       * In both cases the already_flashed flag must be reset.
       */      
      already_flashed = 0;

      if (strncmp((char *)(uart_firstcmd), fwupg, UART_FWUPG_CTRL_SZ) == 0)
      {
        /** 
        * Send the current fw version to the java Flash Updater Tool (it is also considered 
        * as a confirmation that the serial communication has been correctly established). 
        */
        status = HAL_UART_Transmit(&io_huart, (uint8_t*)ver_buf, (uint16_t)strlen(ver_buf), UART_WRITE_TIMEOUT);
        if (status != HAL_OK)
        {
          Error_Handler();
        }

        Handle_Upgrading_Process();

        /* Clear the buffer for the first command */
        (void)memset(uart_firstcmd, 0, UART_FWUPG_SYNC_SZ);
      }
    }
    else if (len == (uint16_t)UART_FWUPG_SYNC_SZ)
    {
      
      Handle_Upgrading_Process();

      /* Clear the buffer for the first command */
      (void)memset(uart_firstcmd, 0, UART_FWUPG_SYNC_SZ);
    }   
  }
  
}

/* This function manages the FW upgrading process (from synchronization
 * to FW chunk writing to CRC check)
 */
static void Handle_Upgrading_Process(void)
{
  int ready;
  int synced = 0; /* became 1 after getting the sync answer from the Teseo */ 
  uint32_t timeout;
  uint8_t ack;
  uint8_t* buffer;
  uint8_t rsp[] = {0x00, 0x00, 0x00, 0x00}; /* array for the FWUPG_SYNC answer (0x83984073U) */
  uint8_t rsp_len = (uint8_t)sizeof(rsp);
  uint8_t offset;
  int trials;
  const char* fwupg_close = "CLOSE";
  char* ver;
  uint16_t len = UART_FWUPG_SYNC_SZ;
  HAL_StatusTypeDef status = HAL_OK;
  int max_trials = 10;
  
  /**
   * Wait for the NMEA commands coming from the java Flash Updater Tool and 
   * forward them to the Teseo 
   */
  for ( ; ; )
  {
    
    if (synced == 1)
    {
      buffer = rxBuffer;
      timeout = UART_READ_LONG_TIMEOUT;
    }
    else
    {
      buffer = syncBuffer;
      timeout = UART_READ_TIMEOUT;
    }
    
    if (already_flashed == 0)
    {
      status = HAL_UART_Receive(&io_huart, buffer, UART_HEADER_SZ, timeout);
      len = ((uint16_t)buffer[1]<<8) | buffer[0];    
      if (status != HAL_OK && status != HAL_TIMEOUT) {
        Error_Handler();
      }
    }
    if (status == HAL_OK)
    {
      already_flashed = 0;
      status = HAL_UART_Receive(&io_huart, buffer, len, timeout);              
      if (status != HAL_OK && status != HAL_TIMEOUT)
      {
        Error_Handler();
      }
      /* COM port closed */
      if (len == (uint16_t)UART_FWUPG_CTRL_SZ)
      {
        if (strncmp((char *)(buffer), fwupg_close, UART_FWUPG_CTRL_SZ) == 0)
        {
          break;
        }
      }
      /* Start Firmware Update*/
      else if (len == (uint16_t)UART_FWUPG_SYNC_SZ)
      {                
        if (status == HAL_OK)
        {
          PRINTBYTES("rx_2", buffer, len);
          
          /* Reset the Teseo and re-initialize the UART */
          GNSS_UART_DeInit();
          GNSS_UART_Init(FAST_UART_BAUD_RATE);
          
          GNSS_Reset();
          ready = 1;
          
          /* Send the IDENTIFIER command */
          trials = 0;
          while ((ready == 1) && (synced == 0) && (trials < max_trials))
          {
            (void)memset(rsp, 0, rsp_len);                                        
            status = HAL_UART_Transmit(&huart, buffer, IDENTIFIER_CMD_LEN, UART_WRITE_SHORT_TIMEOUT);           
            if (status != HAL_OK)
            {
              PRINTMSG("HAL_UART_1_Transmit ERROR!!!");
              Error_Handler();
            }                    
            PRINTBYTES("tx_1", buffer, IDENTIFIER_CMD_LEN);
            /* Receive char by char */
            for (uint8_t i=0; i<rsp_len; i++)
            {
              status = HAL_UART_Receive(&huart, &(rsp[i]), 1, UART_READ_VERY_SHORT_TIMEOUT);
              if ((status != HAL_OK) && (status != HAL_TIMEOUT))
              {
                PRINTMSG("HAL_UART_1_Receive ERROR!!!");
                Error_Handler();
              }
            }            

            PRINTBYTES("rx_1", rsp, rsp_len);                      
            /* Check the answer is the FWUPG_SYNC */
            /**
             * Unfortunately we are able to read only the first byte (0x73) out of the 
             * four bytes (0x73, 0x40, 0x98, 0x83) indicating that the synchronization 
             * went ok. To be investigated.
             */
            if (rsp[0] == (uint8_t)0x73)
            {
              synced = 1;                      
            }
            else
            {
              trials++;
              if (trials == max_trials)
              {
                PRINTMSG("Max number of trials reached!!! Not synced!!!\n");
                ack = NACK;
                status = HAL_UART_Transmit(&io_huart, &ack, 1, UART_WRITE_SHORT_TIMEOUT);
                if (status != HAL_OK)
                {
                  PRINTMSG("HAL_UART_2_Transmit ERROR!!!");
                  Error_Handler();
                }
              }
            }
          }  
          /* Send Device Start Communication */
          (void)memset(rsp, 0, rsp_len);
          offset = IDENTIFIER_CMD_LEN;
          status = HAL_UART_Transmit(&huart, &buffer[offset], START_COMMUNICATION_LEN, UART_WRITE_SHORT_TIMEOUT);
          if (status != HAL_OK)
          {
            PRINTMSG("HAL_UART_1_Transmit ERROR!!!");
            Error_Handler();
          }
          PRINTBYTES("tx_1", &buffer[offset], START_COMMUNICATION_LEN);
          len = 0;
          while (len == (uint16_t)0)
          {
            status = HAL_UART_Receive(&huart, rsp, 1, UART_READ_VERY_SHORT_TIMEOUT);
            if ((status != HAL_OK) && (status != HAL_TIMEOUT))
            {
              PRINTMSG("HAL_UART_1_Receive ERROR!!!");
              Error_Handler();
            }
            len = (uint16_t)strlen((char*)rsp);
          }
          PRINTBYTES("rx_1", rsp, len);
          if (rsp[0] != (uint8_t)ACK)
          {
            Error_Handler();
          }
          /* Send Image Info */ 
          offset = IDENTIFIER_CMD_LEN+START_COMMUNICATION_LEN;
          status = HAL_UART_Transmit(&huart, &buffer[offset], IMAGE_INFO_LEN, UART_WRITE_TIMEOUT);
          if (status != HAL_OK)
          {
            PRINTMSG("HAL_UART_1_Transmit ERROR!!!");
            Error_Handler();
          }           
          PRINTBYTES("tx_1", buffer+offset, IMAGE_INFO_LEN);
          
          /* Send Flasher Ready after Image Info */
          HAL_Delay(100);
          len = 0;
          trials = 0;
          (void)memset(rsp, 0, rsp_len);
          offset = IDENTIFIER_CMD_LEN+START_COMMUNICATION_LEN+IMAGE_INFO_LEN;
          while ((len == (uint16_t)0) && (trials < 50))
          {
            status = HAL_UART_Transmit(&huart, &buffer[offset], FLASHER_READY_CMD_LEN, UART_WRITE_SHORT_TIMEOUT);
            if (status != HAL_OK) {
              Error_Handler();
            }
            PRINTBYTES("tx_1", &buffer[offset], FLASHER_READY_CMD_LEN);
            
            /* Wait for the 1st ACK */
            status = HAL_UART_Receive(&huart, rsp, 1, UART_READ_VERY_SHORT_TIMEOUT);
            if ((status != HAL_OK) && (status != HAL_TIMEOUT))
            {
              PRINTMSG("HAL_UART_1_Receive ERROR!!!");
              Error_Handler();
            }
            len = (uint16_t)strlen((char*)rsp);
            trials++;
          }                    
          /* Wait for the 2nd and the 3rd (the Erase Memory) ACK */
          status = HAL_UART_Receive(&huart, &(rsp[1]), 2, UART_READ_READY_ACK_TIMEOUT);
          if ((status != HAL_OK) && (status != HAL_TIMEOUT))
          {
            PRINTMSG("HAL_UART_1_Receive ERROR!!!");
            Error_Handler();
          }
          len = (uint16_t)strlen((char*)rsp); 
          PRINTBYTES("rx_1", rsp, len);
          
          /* Send an ACK to the java tool indicating the Teseo is ready to be written */
          ack = rsp[0];
          status = HAL_UART_Transmit(&io_huart, &ack, 1, UART_WRITE_TIMEOUT);
          if (status != HAL_OK)
          {
            Error_Handler();
          }
          PRINTBYTES("tx_2", &ack, 1);
        }
        ack = 0x00;
      }
      else
      {
        /* Write the new FW chunk */
        if ((len != (uint16_t)4) && 
            ((buffer[0] != (uint8_t)0xff) || (buffer[1] != (uint8_t)0xff) || (buffer[2] != (uint8_t)0xff) || (buffer[3] != (uint8_t)0xff)))
        {
                            
              status = HAL_UART_Transmit(&huart, buffer, len, timeout);
              if (status != HAL_OK)
              {
                PRINTMSG("HAL_UART_1_Transmit ERROR!!!");
                Error_Handler();
              }
              PRINTMSG("chunk len %d\n", len);
              PRINTBYTES("tx_1", buffer, 5); /* Print first 5 bytes */
              PRINTBYTES("tx_1", buffer+len-5, 5); /* Print last 5 bytes */
              
              /* Receive the ACK */
              status = HAL_UART_Receive(&huart, &ack, 1, UART_READ_CHUNK_ACK_TIMEOUT);
              if ((status != HAL_OK) && (status != HAL_TIMEOUT))
              {
                PRINTMSG("HAL_UART_1_Receive ERROR!!!");
                Error_Handler();
              }              
              PRINTBYTES("rx_1", &ack, 1);                 
              status = HAL_UART_Transmit(&io_huart, &ack, 1, UART_WRITE_TIMEOUT);
              if (status != HAL_OK)
              {
                PRINTMSG("HAL_UART_2_Transmit ERROR!!!");
                Error_Handler();
              }
              PRINTBYTES("tx_2", &ack, 1);
              ack = 0x00;                
            }
        else
        {
          /* EOF reached */
          /* Receive the CRC ACK */
          status = HAL_UART_Receive(&huart, &ack, 1, UART_READ_CRC_ACK_TIMEOUT);
          if ((status != HAL_OK) && (status != HAL_TIMEOUT))
          {
            PRINTMSG("HAL_UART_1_Receive ERROR!!!");
            Error_Handler();
          }          
          PRINTBYTES("rx_1", &ack, 1);
          HAL_Delay(500);
          status = HAL_UART_Transmit(&io_huart, &ack, 1, UART_WRITE_LONG_TIMEOUT);
          if (status != HAL_OK)
          {
            PRINTMSG("HAL_UART_2_Transmit ERROR!!!");
            Error_Handler();
          }
          PRINTBYTES("tx_2", &ack, 1);
          
          /* Reset the Teseo and transmit to the java upgrader the new fw version */
          HAL_Delay(500);
          
          GNSS_UART_DeInit();           
          GNSS_Reset();
          GNSS_UART_Init(DEFAULT_UART_BAUD_RATE);
          
          HAL_Delay(500);

          ver = Get_GNSS_FW_Version();
          (void)memcpy(ver_buf, ver, (uint32_t)strlen(ver));
          PRINTMSG("version fw = %s\n", ver);
          status = HAL_UART_Transmit(&io_huart, (uint8_t*)ver_buf, (uint16_t)strlen(ver_buf), UART_WRITE_TIMEOUT);            
          if (status != HAL_OK)
          {
            Error_Handler();
          }
          already_flashed = 1;
          break;
        }
      }
    }
  }
}

/*	
 * This function sends the NMEA $PSTMGETSWVER command and returns
 * the firmware image version
 */
static char* Get_GNSS_FW_Version(void) 
{
  char* ver = NULL;
  HAL_StatusTypeDef status;  
  uint16_t buffer_sz = 512;

  while (ver == NULL)
  {
    status = HAL_UART_Transmit(&huart, (uint8_t *)"$PSTMGETSWVER,6\n\r", (uint16_t)sizeof("$PSTMGETSWVER,6\n\r"), UART_WRITE_TIMEOUT);
    if (status == HAL_OK)
    {
      status = HAL_UART_Receive(&huart, rxBuffer, buffer_sz, UART_READ_STRING_TIMEOUT);
      if ((status != HAL_OK) && (status != HAL_TIMEOUT))
      {
        Error_Handler();
      }
      ver = TESEO_Detect_Sentence("PSTMVER,BINIMG", rxBuffer, buffer_sz);
      HAL_Delay(500);
    }
  }
  return ver;
}

/*
 * This function detects a string from a received response
 */
static char* TESEO_Detect_Sentence(const char *rsp, uint8_t *buf, uint32_t len)
{
  char *result = NULL;
  uint32_t i = 0;
  const unsigned long rsp_len = strlen(rsp);
  len -= strlen(rsp);
  
  PRINTBYTES("rx_1", buf, len); 
  
  while (!result && i < len)
  {
    for (; buf[i] != '$' && i < len; ++i); /* 1. check '$' char */
    if (i == len)
      break; /* no more char.... */
    
    ++i; /* to point to the char after '$' */
    
    if (strncmp(&((char*)buf)[i], rsp, rsp_len) == 0)
    {
      result = &((char*)buf)[i];
    }
  }
  
  if (result)
  {
    for (i = 0; result[i] != '*'; ++i);
    result[i] = 0;
  }
    
  return result;
  
}

#ifdef PRINT_BYTES_ON_TERMINAL_IO
/*
 * This function print on terminal IO the bytes read/written from/to both the UART
 */
void PrintBytes(char* direction, uint8_t* buf, uint32_t len)
{
  uint32_t i;
  
  printf("[-%s-]%d;", direction, len);
  for (i=0; i<len; i++)
  {
    printf("%02x ", buf[i]);
    //printf("%c", buf[i]);
  }
  printf("\n");
}
#endif

#ifdef PRINT_TIME_ON_TERMINAL_IO
/* Assign to a variable the value of the HAL_GetTick() */
void SetTime (uint32_t* time)
{
  *time = HAL_GetTick();
}

/* Assign to a variable the value of the HAL_GetTick() */
void PrintTime (char* direction, uint32_t time0, uint32_t time1)
{
  uint32_t diff = time1 - time0;
  
  printf("[-%s-]%d (ms);\n", direction, diff);
}
#endif

/* Configures GPIO */
static void GPIO_Config(void)
{
   GPIO_InitTypeDef GPIO_InitStruct;
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  
  /* Configure GPIO pin Output Level */
  GNSS_RST_PIN_SET();
  
  /* Configure Reset */
  GPIO_InitStruct.Pin = GNSS_RST_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GNSS_RST_PORT, &GPIO_InitStruct);
#if 0
  /*Configure GPIO pin : PA5 */
  GPIO_InitStruct.Pin = GNSS_WAKEUP_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GNSS_WAKEUP_PORT, &GPIO_InitStruct);
#endif
}

/* System Clock Configuration */
void SystemClock_Config(void)
{
  
  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  
  __HAL_RCC_PWR_CLK_ENABLE();
  
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
    |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  
  /**Configure the Systick interrupt time 
  */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  
  /**Configure the Systick 
  */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  
  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 15, 0);
}


/*
* This function is executed in case of error occurrence.
*/
void Error_Handler(void)
{
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
}

#ifdef USE_FULL_ASSERT

/* Reports the name of the source file and the source line number */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
  ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

