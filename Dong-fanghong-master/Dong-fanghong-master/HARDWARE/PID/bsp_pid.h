#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	float err;          //偏差值
	float err_last;     //上一个偏差值
	float err_prev;
	float Kp,Ki,Kd;     //比例、积分、微分系数
	float integral;     //积分值
	int output_val;   //输出值
}PID;

float location_pid_set(float Kp, float Ki, float Kd, float actual_val,float target_val);
float speed_pid_set(float Kp, float Ki, float Kd, float target_val,float actual_val);
int blance_pid(float Kp, float Ki, float Kd,  float angle );// 竖直
float blance_pid_hor(float Kp, float Ki, float Kd, float angle);

#endif
