#ifndef __SERIAL_H
#define __SERIAL_H
#include "stm32f10x.h"
#include <stdio.h>

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t* Array, uint16_t Length);
void Serial_SendString(char* String);
void Serial_SendNumber(uint32_t Num, uint8_t Length);
uint32_t Serial_Pow(uint32_t X, uint32_t Y);

#endif
