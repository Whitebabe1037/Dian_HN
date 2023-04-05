#ifndef __KALMAN_H
#define __KALMAN_H
#include "stm32f10x.h"

typedef struct{
	float x_last; //上一时刻最优估计(修正值)
	float prediction_last;	//上一时刻的噪声协方差矩阵
}Kalman;

void kalman_filter(Kalman* p, float R, uint16_t x_measure);

#endif
