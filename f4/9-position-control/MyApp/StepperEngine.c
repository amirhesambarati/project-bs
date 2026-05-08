#include "StepperEngine.h"
#include "StepperDriver.h"
#include "Queue.h"
#include "time.h"


uint32_t  STEP_Time = 0 ;

typedef struct
{
    MoveBlock current;
    uint32_t step_count;
    uint8_t  busy;
	

} StepperState;

StepperState stepper;

void Stepper_LoadNextBlock()
{
    MoveBlock block;

    if(Queue_Pop(&block))
    {
        stepper.current = block;
        stepper.step_count = 0;
        stepper.busy = 1;
				MotorEnable(block.axis, 1 );
        MotorDirection(block.axis , block.direction);

    }
    else
    {
				MotorEnable(block.axis, 0 );
        stepper.busy = 0;
    }
}

void Stepper_ISR()
{
    if(!stepper.busy)
    {
        Stepper_LoadNextBlock();
        return;
    }
		
		uint32_t now = micros();

		if (now - STEP_Time > stepper.current.step_interval){
		STEP_Time = now;
    MotorStepPulse(stepper.current.axis);

    stepper.step_count++;
		}
    if(stepper.step_count >= stepper.current.steps)
    {
        stepper.busy = 0;
    }
}
