

#include "command_parser.h"
#include "main.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "STM32PrintfUART.h"
#include "ReadENC.h"

#define CMD_BUFFER_SIZE 128

extern UART_HandleTypeDef huart1;

static char cmd_buffer[CMD_BUFFER_SIZE];
static uint8_t cmd_index = 0;
volatile char rx_char;
volatile uint8_t rx_flag = 0;
uint8_t NewDataInt;

void CommandParser_Process(char *cmd)
{
	
//	if(strcmp(cmd,"GET_POS")==0)
//...
//else if(strcmp(cmd,"GET_SPEED")==0)
//...
//else if(strcmp(cmd,"RESET")==0)
					Encoder_Update();	
					int32_t Count = Encoder_GetCount();
					int32_t Speed = Encoder_GetSpeed();
					int32_t rpm = Encoder_GetRPM();

if(strstr(cmd,"GET_POS") != NULL)
    {

					printf("POS=%d\r\n",Count);
    }
    else
    {
        Log("Bad Command\r\n");
    }
}


void Command_Input(char c)
{
    if(c == '\n' || c == '\r')
    {
			  if(cmd_index == 0) return;
        
			cmd_buffer[cmd_index] = 0;

        CommandParser_Process(cmd_buffer);

        cmd_index = 0;
    }
    else
    {
        if(cmd_index < CMD_BUFFER_SIZE-1)
        {
            cmd_buffer[cmd_index++] = c;
        }else
				{
						cmd_index = 0; // reset buffer
				}
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        rx_char = NewDataInt;
        rx_flag = 1;

        HAL_UART_Receive_IT(&huart1,&NewDataInt,1);
    }
}

void UART_RX_IT_Init(void){

		HAL_UART_Receive_IT(&huart1,&NewDataInt,1);
}

void test(){
	
if(rx_flag)
{
    rx_flag = 0;
    Command_Input(rx_char);
}

}