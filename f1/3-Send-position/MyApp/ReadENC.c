#include "ReadENC.h"

#define ENCODER_PPR 200

volatile int32_t pulse_counter = 0;

static int32_t last_pulse_count = 0;
static int32_t pulse_speed_pps = 0;
static int32_t motor_rpm = 0;

static uint32_t last_speed_time = 0;


void Encoder_Init(void)
{
    pulse_counter = 0;
    last_pulse_count = 0;
    pulse_speed_pps = 0;
    motor_rpm = 0;
    last_speed_time = HAL_GetTick();
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == ENC_A1_Pin)
    {
        if (ENC_B1_GPIO_Port->IDR & ENC_B1_Pin)
            pulse_counter--;
        else
            pulse_counter++;
    }
}


void Encoder_Update(void)
{
    uint32_t now = HAL_GetTick();
    uint32_t dt = now - last_speed_time;

    if (dt >= 100)
    {
        int32_t pulse_now;

        __disable_irq();
        pulse_now = pulse_counter;
        __enable_irq();

        int32_t dp = pulse_now - last_pulse_count;

        pulse_speed_pps = (int32_t)((int64_t)dp * 1000 / dt);

        motor_rpm = (pulse_speed_pps * 60) / ENCODER_PPR;

        last_pulse_count = pulse_now;
        last_speed_time = now;
    }
}


int32_t Encoder_GetCount(void)
{
    int32_t count;

    __disable_irq();
    count = pulse_counter;
    __enable_irq();

    return count;
}


int32_t Encoder_GetSpeed(void)
{
    return pulse_speed_pps;
}


int32_t Encoder_GetRPM(void)
{
    return motor_rpm;
}
