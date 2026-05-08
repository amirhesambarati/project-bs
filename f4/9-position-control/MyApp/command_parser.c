#include "command_parser.h"
#include "Queue.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "uart_usb_bridge.h"


void CommandParser_Process(char *cmd)
{
    char axis;
    uint32_t steps;
    uint32_t interval;
    int dir;

    if(sscanf(cmd,"%c %u %u %d",&axis,&steps,&interval,&dir) == 4)
    {
        MoveBlock move;

        move.axis = axis;
        move.steps = steps;
        move.step_interval = interval;
        move.direction = dir;

        if(!Queue_Push(&move))
            Log("Queue Full\r\n");
        else
            Log("OK\r\n");
    }
    else
    {
        Log("Bad Command\r\n");
    }
}

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
