
#include "string.h"
#include "main.h"
#include "StepperDriver.h"

extern TIM_HandleTypeDef htim2;

uint32_t micros()
{
    return __HAL_TIM_GET_COUNTER(&htim2);
}

static inline uint32_t millis(void)
{
    return HAL_GetTick();   
}

void MotorEnable(char label, uint8_t en) {
  GPIO_PinState s = en ? GPIO_PIN_RESET : GPIO_PIN_SET;

  switch (label) {
    case 'x': case 'X':
      HAL_GPIO_WritePin(XY_EN_GPIO_Port, XY_EN_Pin, s);
      break;
//    case 'y': case 'Y':
//      HAL_GPIO_WritePin(Y_EN_GPIO_Port, Y_EN_Pin, s);
//      break;
    case 'z': case 'Z':
      HAL_GPIO_WritePin(Z_EN_GPIO_Port, Z_EN_Pin, s);
      break;
  }
}

void MotorDirection(char label, uint8_t dir) {
  //GPIO_PinState s = dir ? GPIO_PIN_SET : GPIO_PIN_RESET;

  switch (label) {
    case 'x':
    case 'X':
      HAL_GPIO_WritePin(X_DIR_GPIO_Port, X_DIR_Pin, dir ? GPIO_PIN_SET : GPIO_PIN_RESET);
		case 'z':
    case 'Z':
      HAL_GPIO_WritePin(Z_DIR_GPIO_Port, Z_DIR_Pin, dir ? GPIO_PIN_SET : GPIO_PIN_RESET);
      break;
  }
	for (volatile int i = 0; i < 100; ++i) __NOP();
}

void MotorStepPulse(char axis)
{
    switch(axis)
    {
        case 'x':
        case 'X':

            HAL_GPIO_WritePin(X_STEP_GPIO_Port, X_STEP_Pin, GPIO_PIN_SET);

            for(volatile int i=0;i<50;i++);
				//HAL_Delay(2);

            HAL_GPIO_WritePin(X_STEP_GPIO_Port, X_STEP_Pin, GPIO_PIN_RESET);

        break;
    }
}
