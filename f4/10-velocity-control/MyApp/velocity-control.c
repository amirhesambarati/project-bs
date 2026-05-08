#include "velocity-control.h"
#include "time.h"
#include "usart-get-position.h"
#include "Queue.h"
#include "uart_usb_bridge.h"

#define PID_PERIOD_US 100
static uint32_t last_pid_time = 0;

typedef struct
{
    float kp;
    float ki;
    float kd;

    float integral;
    float prev_error;

    float out_min;
    float out_max;
} PID_t;

PID_t velo_pid = {
    .kp = 1.0f,
    .ki = 0.0f,
    .kd = 0.00f,
    .out_min = 50,  
    .out_max = 5000000    
};

float target_POS = 100.0f; 

float measured_POS = 0.0f;

float PID_Update(PID_t *pid, float error, float dt)
{
    pid->integral += error * dt;

    if(pid->integral > pid->out_max) pid->integral = pid->out_max;
    if(pid->integral < pid->out_min) pid->integral = pid->out_min;

    float derivative = (error - pid->prev_error) / dt;
    pid->prev_error = error;

    float out =
        pid->kp * error +
        pid->ki * pid->integral +
        pid->kd * derivative;

    if(out > pid->out_max) out = pid->out_max;
    if(out < pid->out_min) out = pid->out_min;

    return out;
}


void Stepper_PID_Task(void)
{

    uint32_t now = micros();
    if(now - last_pid_time < PID_PERIOD_US){
    return;
		}
    last_pid_time = now ;

    int32_t POS = GetPOS();

    float error = target_POS - POS;
	

    float interval = PID_Update(
        &velo_pid,
        error,
        PID_PERIOD_US / 1000000.0f
    );

		    MoveBlock move;
        move.axis = 'z';
				if(error > 0 ){
        move.direction = 0;		
				}else{
        move.direction = 1;		
				}
        move.steps = 1;
        move.step_interval = interval;
				
        if(!Queue_Push(&move)){
					//            Log("Queue Full\r\n");
				}

	
}


