#include "Serial.h"
/*
TxPin:PA9  RxPin:PA10
*/

void Serial_Init()
{
	//Init RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//Init PA9 (AF-push-pull TX)
	GPIO_InitTypeDef PA9_InitStructure;
	PA9_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	PA9_InitStructure.GPIO_Pin = GPIO_Pin_9;
	PA9_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//和功耗相关
	GPIO_Init(GPIOA ,&PA9_InitStructure);	
	
	//Init PA10 (input-pull-up RX)
	GPIO_InitTypeDef PA10_InitStructure;
	PA10_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	PA10_InitStructure.GPIO_Pin = GPIO_Pin_10;
	PA10_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//和功耗相关
	GPIO_Init(GPIOA ,&PA10_InitStructure);	
	
	//Init USART1
	USART_InitTypeDef USART1_InitStruct;
	USART1_InitStruct.USART_BaudRate = 9600;	//填入波特率，自动计算分频系数
	USART1_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;		//无硬件流控
	USART1_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;		//收+发
	USART1_InitStruct.USART_Parity = USART_Parity_No;		//无校验
	USART1_InitStruct.USART_StopBits = USART_StopBits_1;	//停止位位宽
	USART1_InitStruct.USART_WordLength = USART_WordLength_8b;	//字长
	
	USART_Init(USART1, &USART1_InitStruct);
	USART_Cmd(USART1, ENABLE);
}

void Serial_SendByte(uint8_t Byte)
{
	while(! USART_GetFlagStatus(USART1, USART_FLAG_TXE));
	USART_SendData(USART1, Byte);		
}

void Serial_SendArray(uint8_t* Array, uint16_t Length)
{
	for(uint16_t i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

void Serial_SendString(char* String)
{
	uint8_t i = 0;
	while(String[i] != '\0')
	{
		Serial_SendByte(String[i]);
		i++;
	}
}

uint32_t Serial_Pow(uint32_t X, uint32_t Y)		//返回X的Y次方(Y>0)
{
	if(Y == 0)
	{
		return 1;
	}
	else if(Y > 0)
	{
		return X * Serial_Pow(X, Y - 1);
	}
	else
	{
		return 0;
	}
}

void Serial_SendNumber(uint32_t Num, uint8_t Length)		//从高位到低位发送十进制数
{
	for(uint8_t i = 1; i <= Length; i ++)
	{
		Serial_SendByte(Num / Serial_Pow(10, Length - i) % 10 + '0');
	}
}

int fputc(int ch, FILE *f)	//printf()函数调用的底层，改为输出到串口
{
	Serial_SendByte(ch);
	return ch;
}

