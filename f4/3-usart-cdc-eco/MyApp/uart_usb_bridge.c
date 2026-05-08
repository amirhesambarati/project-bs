
#include "usbd_cdc_if.h"
#include "string.h"
#include "uart_usb_bridge.h"

extern UART_HandleTypeDef huart1;

HAL_StatusTypeDef Status ;

uint8_t NewData[2];

#define UART_BUFFER_SIZE 512

uint8_t uart_buffer[UART_BUFFER_SIZE];

volatile uint16_t head = 0;
volatile uint16_t tail = 0;

#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART2 and Loop until the end of transmission */
  //HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
	CDC_Transmit_FS((uint8_t*)&ch, 1);
	
  return ch;
} 

int _write(int file, char *ptr, int len)
{
		CDC_Transmit_FS((uint8_t*)ptr, len);
    return len;
}


int fputc(int ch, FILE *f)
{
		CDC_Transmit_FS((uint8_t*)&ch, 1);
    return ch;
}

void Log(char* log){
		
	CDC_Transmit_FS((uint8_t*)log, strlen(log));
	
}

void USB_SendString(char *str)
{
    CDC_Transmit_FS((uint8_t*)str, strlen(str));
}

void RingBuffer_Write(uint8_t data)
{
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

void USB_Bridge_Task(void)
{
    static uint8_t usb_buffer[64];
    uint8_t count = 0;

    while(count < 64 && head != tail)
    {
        usb_buffer[count++] = uart_buffer[tail];
        tail = (tail + 1) % UART_BUFFER_SIZE;
    }

    if(count > 0)
    {
        if(CDC_Transmit_FS(usb_buffer, count) == USBD_OK)
        {
            // sent
        }
        else
        {
            // USB busy → برگرداندن داده به بافر
            tail = (tail - count + UART_BUFFER_SIZE) % UART_BUFFER_SIZE;
        }
    }
}

void test1(){//pulling
	Status = HAL_UART_Receive(&huart1,NewData,1,100);
	
	if(Status == HAL_OK){
		USB_SendString("this is crc test ! /n");
	}
	HAL_Delay(30);

}

volatile uint8_t uart_rx;
volatile uint8_t uart_flag = 0;
uint8_t NewDataInt;

 void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        RingBuffer_Write(NewDataInt);
        Status = HAL_UART_Receive_IT(&huart1, &NewDataInt, 1);
    }
}

void test2(){ //intterupt

	        if(uart_flag)
        {
            uart_flag = 0;
            printf("%c",NewDataInt);
        }
	
}

void USB_Bridge(){
	Status = HAL_UART_Receive_IT(&huart1,&NewDataInt,1);
	while(1){
	//test1();
	//test2();
	USB_Bridge_Task();
	}
}