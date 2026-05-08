#include "main.h"
#include "time.h"
#include "MyApp.h"
#include "Queue.h"
#include "StepperDriver.h"
#include "command_parser.h"
#include "uart_usb_bridge.h"

#define CMD_BUFFER_SIZE 64

static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;

void Command_Input(char c)
{
    if(c == '\n' || c == '\r')
    {
        cmd_buffer[cmd_index] = 0;

        CommandParser_Process(cmd_buffer);

        cmd_index = 0;
    }
    else
    {
        if(cmd_index < CMD_BUFFER_SIZE-1)
        {
            cmd_buffer[cmd_index++] = c;
        }
    }
}
/////////////////////////////////////
//usbcdcif.c
/////////////////////////////////////

//extern void Command_Input(char c);
//static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
//{
//  /* USER CODE BEGIN 6 */
//	  for(uint32_t i=0;i<*Len;i++)
//  {
//      Command_Input((char)Buf[i]);
//  }
//	
//  USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
//  USBD_CDC_ReceivePacket(&hUsbDeviceFS);
//  return (USBD_OK);
//  /* USER CODE END 6 */
//}

/////////////////////////////////////

uint32_t time = 0 ;
void test1(){
	
		time = micros();
		printf("%d !\n",time);
	
		UART_USB_Bridge_Task();
		HAL_Delay(1000);
	
}

void My_App(){
	//test1();

}

