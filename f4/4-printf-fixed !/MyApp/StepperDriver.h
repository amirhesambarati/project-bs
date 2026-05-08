#include "main.h"

void MotorDirection(char label, uint8_t dir) ;
void MotorEnable(char label, uint8_t en) ;
void MotorStepPulse(char axis) ;
static inline uint32_t millis(void) ;
uint32_t micros();