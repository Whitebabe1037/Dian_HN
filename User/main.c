#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Delay.h"
#include "Serial.h"
#include "ADC.h"
//using MicroLIB

void Init(void)
{
	Serial_Init();
	ADC_Init_packaged();
}
 
int main(void)
{
	Init();
	printf("system successfully inited");
	ADC_Start(ENABLE);
	extern uint16_t ADC_Value[2];
	while(1)
	{
		printf("sensor1:%d ", ADC_Value[0]);
		printf("sensor2:%d\r\n", ADC_Value[1]);
		Delay_ms(1000);
	}
}
