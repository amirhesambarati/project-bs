#include "usart-get-position.h"
#include "usbd_cdc_if.h"
#include "string.h"
#include "uart_usb_bridge.h"

#define UART_BUFFER_SIZE 1024

extern UART_HandleTypeDef huart1;

uint8_t	uart_GET_POS_active = 1 ;

uint8_t uart_bridg_active = 1;

uint8_t NewDataInt;

static uint8_t uart_buffer[UART_BUFFER_SIZE];

static volatile uint16_t head = 0;
static volatile uint16_t tail = 0;



		void RingBuffer_Write(uint8_t data){
    uint16_t next = (head + 1) % UART_BUFFER_SIZE;
    if(next != tail) // buffer not full
		{
        uart_buffer[head] = data;
        head = next;
    }
	}


	int RingBuffer_Read(uint8_t *data)
{
    if(head == tail)
        return 0; // buffer empty

    *data = uart_buffer[tail];
    tail = (tail + 1) % UART_BUFFER_SIZE;

    return 1;
}

void Log(char* log)
{
    uint16_t len = strlen(log);
    
    for(uint16_t i = 0; i < len; i++)
    {
        RingBuffer_Write((uint8_t)log[i]);
    }
}

int _write(int file, char *ptr, int len)
{
    for (int i = 0; i < len; i++)
    {
        RingBuffer_Write(ptr[i]);
    }
    return len;
}

int fputc(int ch, FILE *f)
{
    RingBuffer_Write((uint8_t)ch);
    return ch;
}

void UART_USB_Bridge_Task_Init(void){

		HAL_UART_Receive_IT(&huart1,&NewDataInt,1);
}
extern USBD_HandleTypeDef hUsbDeviceFS;

void UART_USB_Bridge_Task(void)
{
    if (hUsbDeviceFS.dev_state != USBD_STATE_CONFIGURED)
        return;

    static uint8_t usb_buffer[64];
    uint8_t count = 0;

    uint16_t temp_tail = tail;
    uint16_t local_head = head;

    while(count < 64 && temp_tail != local_head)
    {
        usb_buffer[count++] = uart_buffer[temp_tail];
        temp_tail = (temp_tail + 1) % UART_BUFFER_SIZE;
    }

		if(count > 0 )
		{
				if(CDC_Transmit_FS(usb_buffer, count) == USBD_OK)
				{
						tail = temp_tail;
				}
		}
}


 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{

    if(huart->Instance == USART1)
    {
				if(uart_bridg_active){
        RingBuffer_Write(NewDataInt);
				}
				if(uart_GET_POS_active){
					
					ring_write(NewDataInt);
				
				}
        HAL_UART_Receive_IT(&huart1, &NewDataInt, 1);
    }
	
}
