#ifndef __ADC_H
#define __ADC_H
#include "stm32f10x.h"

void ADC_Init_packaged(void);
void ADC_Start(FunctionalState NewState);
uint16_t ADC_GetData1(void);
uint16_t ADC_GetData2(void);

#endif
