
#include "string.h"
#include "main.h"
#include "StepperDriver.h"

	void MotorEnable(char label, uint8_t en) {
		
		GPIO_PinState s = en ? GPIO_PIN_RESET : GPIO_PIN_SET;

		switch (label) {
				case 'x': case 'X': case 'y': case 'Y':
					HAL_GPIO_WritePin(XY_EN_GPIO_Port, XY_EN_Pin, s);
					break;
				case 'z': case 'Z':
					HAL_GPIO_WritePin(Z_EN_GPIO_Port, Z_EN_Pin, s);
					break;
				case 'a': case 'A':
					HAL_GPIO_WritePin(E0_EN_GPIO_Port, E0_EN_Pin, s);
					break;
				case 'b': case 'B':
					HAL_GPIO_WritePin(E1_EN_GPIO_Port, E1_EN_Pin, s);
					break;
				case 'c': case 'C':
					HAL_GPIO_WritePin(E2_EN_GPIO_Port, E2_EN_Pin, s);
					break;
			}
	}


	void MotorDirection(char label, uint8_t dir) {
		
  GPIO_PinState s = dir ? GPIO_PIN_SET : GPIO_PIN_RESET;

  switch (label) {
    case 'x': case 'X':
      HAL_GPIO_WritePin(X_DIR_GPIO_Port, X_DIR_Pin,	s );
			break;
		case 'y': case 'Y':
      HAL_GPIO_WritePin(Y_DIR_GPIO_Port, Y_DIR_Pin,	s );
      break;
		case 'z': case 'Z':
      HAL_GPIO_WritePin(Z_DIR_GPIO_Port, Z_DIR_Pin,	s );
      break;
		case 'a': case 'A':
		HAL_GPIO_WritePin(E0_DIR_GPIO_Port, E0_DIR_Pin, s);
			break;
		case 'b': case 'B':
		HAL_GPIO_WritePin(E1_DIR_GPIO_Port, E1_DIR_Pin, s);
			break;
		case 'c': case 'C':
		HAL_GPIO_WritePin(E2_DIR_GPIO_Port, E2_DIR_Pin, s);
			break;
  }
	for (volatile int i = 0; i < 50 ; ++i) __NOP();
}

void MotorStepPulse(char axis)
{
    GPIO_TypeDef *port;
    uint16_t pin;

    switch(axis)
    {
        case 'x':
        case 'X':
            port = X_STEP_GPIO_Port;
            pin  = X_STEP_Pin;
            break;
				case 'y':
        case 'Y':
            port = Y_STEP_GPIO_Port;
            pin  = Y_STEP_Pin;
            break;
				
				case 'z':
				case 'Z':
            port = Z_STEP_GPIO_Port;
            pin  = Z_STEP_Pin;
            break;
				case 'a':
				case 'A':
            port = E0_STEP_GPIO_Port;
            pin  = E0_STEP_Pin;
            break;
				case 'b':
				case 'B':
            port = E1_STEP_GPIO_Port;
            pin  = E1_STEP_Pin;
            break;
				case 'c':
				case 'C':
            port = E2_STEP_GPIO_Port;
            pin  = E2_STEP_Pin;
            break;				
				
        default:
            return;
    }

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);

    for(volatile int i=0;i<50;i++) __NOP();

    HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
}


