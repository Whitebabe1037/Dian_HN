#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include "Delay.h"
#include "Serial.h"
#include "ADC.h"
#include "Kalman.h"
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
	Kalman sensor[2];
	sensor[0].prediction_last = 1;
	sensor[1].prediction_last = 1;
	extern uint16_t ADC_Value[2];
	while(1)
	{
		kalman_filter(&sensor[0], 0.1, ADC_Value[0]);
		kalman_filter(&sensor[1], 0.1, ADC_Value[1]);
		printf("1:%d ", ADC_Value[0]);
		printf("corrected:%d\r\n", (int)sensor[0].x_last);
		printf("2:%d ", ADC_Value[1]);
		printf("corrected:%d\r\n", (int)sensor[1].x_last);
		Delay_ms(100);
	}
}
