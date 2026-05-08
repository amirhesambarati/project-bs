#include "main.h"
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


void My_App(){
		MotorEnable('z',1);
		printf("motor on !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		MotorDirection('z',1);
		printf("motor direction 1 !\n");		
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		printf("motor start turning !\n");
		UART_USB_Bridge_Task();
		for(int i=1 ; i<6400 ;i++){
		MotorStepPulse('z');
		HAL_Delay(1);	
		}
		printf("motor stop turning !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	

		MotorEnable('z',0);
		printf("motor off !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		MotorEnable('z',1);
		printf("motor on !\n");	
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		MotorDirection('z',0);
		printf("motor direction 0 !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		printf("motor start turning !\n");
		UART_USB_Bridge_Task();
		for(int i=1 ; i<6400 ;i++){
		MotorStepPulse('z');
		HAL_Delay(1);	
		}
		printf("motor stop turning !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	
		
		MotorEnable('z',0);
		printf("motor off !\n");
		UART_USB_Bridge_Task();
		HAL_Delay(5000);	

}

