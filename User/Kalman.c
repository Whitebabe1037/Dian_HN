#include "Kalman.h"
/*一维卡尔曼滤波*/

void kalman_filter(Kalman* p, float R, uint16_t x_measure)
{
	//预测
	float x_pre = p->x_last;	//根据上一时刻最优估计得出的当前未修正预测值
	float prediction_pre = p->prediction_last;	//根据上一时刻的噪声协方差矩阵估计当前值
	//修正预测
	float kal = prediction_pre / (prediction_pre + R);	//卡尔曼参数
	float prediction_now = (1 - kal) * prediction_pre;	//修正当前噪声协方差矩阵
	float x_now = x_pre + kal * ((float)x_measure - x_pre);	//当前最优估计
	//更新变量
	p->prediction_last = prediction_now;
	p->x_last = x_now;
}
