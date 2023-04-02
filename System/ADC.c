#include "ADC.h"
/*
	考虑到夹爪是否夹紧需要进行判断，
	platform:stm32f103c8t6
	
	*/

uint16_t ADC_Value[2] = {0};	//数据存在该数组中

/**
  * @brief  初始化ADC1，两路输入通道，规则模式，连续转换扫描模式
  * @param  None
  * @retval None
  */
void ADC_Init_packaged(void)
{
	/******************init peripheral clock**************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA ,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);		//ADCCLK=72MHz/6=12MHz
	/*********************Init GPIO**********************/
	//Init PA0,PA1
	GPIO_InitTypeDef PA0_InitStructure;
	PA0_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	PA0_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	PA0_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA ,&PA0_InitStructure);
	/*********************Init ADC*********************/	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);	//扫描模式通道顺序
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;	//连续转换
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;		//数据右对齐
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//关闭外部触发
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;	//ADC1独立工作
	ADC_InitStruct.ADC_NbrOfChannel = 2;	//扫描2个通道
	ADC_InitStruct.ADC_ScanConvMode = ENABLE;	//扫描模式
	ADC_Init(ADC1, &ADC_InitStruct);
	//DMA Init
	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;	//起点的地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//起点数据宽度16bit
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//起点处地址指针不自增
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_Value;	//终点的地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;	//终点数据宽度16bit
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//终点地址指针自增
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;	//以外设做起点（默认方向）
	DMA_InitStructure.DMA_BufferSize = 2;	//DMA转运一轮的次数
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	//自动重装，连续转运
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;	//硬件触发
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//优先级
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);	//DMA通道1
	ADC_DMACmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
	
	/* Enable ADC1 reset calibration register */   
  ADC_ResetCalibration(ADC1);
  /* Check the end of ADC1 reset calibration register */
  while(ADC_GetResetCalibrationStatus(ADC1));

  /* Start ADC1 calibration */
  ADC_StartCalibration(ADC1);
  /* Check the end of ADC1 calibration */
  while(ADC_GetCalibrationStatus(ADC1));
}

/**
  * @brief  软件启动/停止AD转换
  * @param  NewState: new state of the selected ADC software start injected conversion.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void ADC_Start(FunctionalState NewState)
{
	ADC_SoftwareStartConvCmd(ADC1, NewState);
}
