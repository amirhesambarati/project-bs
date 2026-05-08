
#include "MyApp.h"
#include "main.h"
#include "stm32f1xx_hal.h"
#include "string.h"
#include <stdio.h>

#define ENCODER_PPR 200

extern UART_HandleTypeDef huart1;

volatile int32_t pulse_counter = 0;
static int32_t last_pulse_count = 0;
static int32_t pulse_speed_pps = 0;     // pulses per second
static uint32_t last_speed_time = 0;
static int32_t motor_rpm = 0;

static inline uint32_t millis(void)
{
    return HAL_GetTick();   
}

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
	HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
	
  return ch;
} 

int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(&huart1, (uint8_t*)ptr, len, 100);
    return len;
}


int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 100);
    return ch;
}


void Log(char* log){

		HAL_UART_Transmit(&huart1,(uint8_t*)log,strlen(log),100);
	
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ENC_A1_Pin) // Channel A
    {
        if (HAL_GPIO_ReadPin(ENC_B1_GPIO_Port, ENC_B1_Pin) == GPIO_PIN_RESET)
        {
            pulse_counter++;   // CW
        }
        else
        {
            pulse_counter--;   // CCW
        }
    }
}

static void UpdateSpeed(void)
{
    const uint32_t now = millis();
    const uint32_t dt = now - last_speed_time;

    if (dt >= 10) 
    {
        int32_t count_now = pulse_counter;          
        int32_t dp = count_now - last_pulse_count;  

        pulse_speed_pps = (int32_t)((dp * 1000) / (int32_t)dt);

        last_pulse_count = count_now;
        last_speed_time = now;
    }
}

static void UpdateSpeedAndRPM(void)
{
    uint32_t now = HAL_GetTick();
    uint32_t dt = now - last_speed_time;

    if (dt >= 100) 
    {
        int32_t pulse_now = pulse_counter;
        int32_t dp = pulse_now - last_pulse_count;

        pulse_speed_pps = (dp * 1000) / (int32_t)dt;

        motor_rpm = (pulse_speed_pps * 60) / ENCODER_PPR;

        last_pulse_count = pulse_now;
        last_speed_time = now;
    }
}


void My_App(){

	last_speed_time = millis();
	while(1){
		
		    UpdateSpeedAndRPM();

    static uint32_t last_print = 0;
    if (HAL_GetTick() - last_print >= 100)
    {
        last_print = millis();

        printf("POS=1234 | speed=%ld pps | RPM=%ld | time = %d\r\n",
//               (long)pulse_counter,
               (long)pulse_speed_pps,
               (long)motor_rpm,
								last_print
							 );
    }

	}
		

}

