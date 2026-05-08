
#include "usart-get-position.h"
#include "uart_usb_bridge.h"

#define UART_POS_BUFFER_SIZE 256
#define LINE_BUFFER_SIZE 100
#define POS_TIMEOUT 50

extern UART_HandleTypeDef huart1;

static uint8_t rx_byte;

static uint8_t uart_buffer[UART_POS_BUFFER_SIZE];
static volatile uint16_t headPOS = 0;
static volatile uint16_t tailPOS = 0;

static char line_buffer[LINE_BUFFER_SIZE];
static uint16_t line_index = 0;

int position = 0;

typedef struct{
    uint8_t busy;
    uint32_t timestamp;
}Request;

Request request = {0};

void LogUART(char *msg)
{
    HAL_UART_Transmit(&huart1,(uint8_t*)msg,strlen(msg),100);
}

void send_request_position()
{
    if(!request.busy)
    {
        LogUART("GET_POS\r\n");
        request.busy = 1;
        request.timestamp = HAL_GetTick();
    }
}

void ring_write(uint8_t data)
{
    uint16_t next = (headPOS + 1) % UART_POS_BUFFER_SIZE;

    if(next != tailPOS)
    {
        uart_buffer[headPOS] = data;
        headPOS = next;
    }
}

int ring_read(uint8_t *data)
{
    if(headPOS == tailPOS)
        return 0;

    *data = uart_buffer[tailPOS];
    tailPOS = (tailPOS + 1) % UART_POS_BUFFER_SIZE;

    return 1;
}

void parse_position(char *line)
{
    char *ptr = strstr(line,"POS=");

    if(ptr)
    {
        ptr += 4;
        position = atoi(ptr);
        request.busy = 0;
//			printf("position = %d\n",position);
    }
}

void process_uart()
{
    uint8_t data;

    while(ring_read(&data))
    {
        if(data == '\n')
        {
            line_buffer[line_index] = '\0';
            parse_position(line_buffer);
            line_index = 0;
        }
        else
        {
            if(line_index < LINE_BUFFER_SIZE-1)
            {
                line_buffer[line_index++] = data;
            }
            else
            {
                line_index = 0;
//								Log("overflow line buffer\n");
						}
        }
    }
}

void check_timeout()
{
    if(request.busy)
    {
        if(HAL_GetTick() - request.timestamp > POS_TIMEOUT)
        {
					Log("POS time out !\n");
            request.busy = 0;
        }
    }
}

int GetPOS(){
return position;
}


static int32_t last_cnt = 0;

int32_t Encoder_GetDelta(void)
{
    int32_t cnt = position ;
    int32_t delta = cnt - last_cnt;
    last_cnt = cnt;
    return delta;
}