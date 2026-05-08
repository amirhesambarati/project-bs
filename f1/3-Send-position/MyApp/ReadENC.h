#ifndef READENC_H
#define READENC_H

#include "stm32f1xx_hal.h"
#include "main.h"
#include <stdint.h>

void Encoder_Init(void);
void Encoder_Update(void);

int32_t Encoder_GetCount(void);
int32_t Encoder_GetSpeed(void);
int32_t Encoder_GetRPM(void);

#endif
