#ifndef Servo_H
#define Servo_H
#include "sys.h"
#define pi 3.1415

extern volatile int circle_servo1;
extern volatile int circle_servo2;
extern volatile int small_servo3;

void TIM2_Init(void);
void Time8_Servo_Init(void);
void TIM14_Init(void);//С���
int conversion_CNT (int angle);
void Set_angle(int a,int b, int c, int d );
void single_servo_steer(int Previous,int current, int N, int time_us);
void together_servo_steer(int Previous,int current, int time_us);
void set_balance_angle(float a,float b,float c,float d);
void single_set(int N, int a);
void pill_Servo(int a,int b,int c);

#endif
