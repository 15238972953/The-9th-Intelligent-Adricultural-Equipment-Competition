#ifndef __BSP_PID_H
#define	__BSP_PID_H
#include "stm32f4xx.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
	float err;          //ƫ��ֵ
	float err_last;     //��һ��ƫ��ֵ
	float Kp,Ki,Kd;     //���������֡�΢��ϵ��
	float integral;     //����ֵ
	int output_val;   //���ֵ
}PID;

float location_pid_set(float Kp, float Ki, float Kd, float actual_val,float target_val);
float speed_pid_set(float Kp, float Ki, float Kd, float target_val,float actual_val);
int blance_pid(float Kp, float Ki, float Kd,  float angle );// ��ֱ
float blance_pid_hor(float Kp, float Ki, float Kd, float angle);

#endif
