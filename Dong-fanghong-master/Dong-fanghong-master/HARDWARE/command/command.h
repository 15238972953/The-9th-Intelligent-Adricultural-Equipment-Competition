#ifndef COM_H
#define COM_H
#include "sys.h"

#define S_1  4 //竖向第一段初始值4
#define S_2  8 //横向第一段初始值4  全哥初值为4
#define Speed_flag1 850 //初始值为730
#define Speed1_3 330
extern int Speed_1;//全哥初始740
extern int Speed_2_6_10;//全哥初始850
extern int Speed_2;
#define Speed_3 850  //全哥初始值为840
#define safe_dis 20 //初始值18
#define slow_sp  40
//竖直
#define SERVO_S_1  41
#define SERVO_S_2  137
#define SERVO_S_3  137
#define SERVO_S_4  43

//水平
#define SERVO_B_1  124
#define SERVO_B_2  53
#define SERVO_B_3  48
#define SERVO_B_4  131

typedef struct
{
	int error_f;// 2号超声波在前
	int error_b;
}Blance;

int xianfu(int k,int qujian);
void bozhong1(void); //奇数垄
void bozhong2(void); //偶数垄
void speed_PID(float TIM3_speed,float TIM4_speed);
void com_PID(int Encoder_Timer3_sum,int Encoder_Timer4_sum);
void servo_bla(void );
void Car_run_1(void );
void Car_run_2(void );
void Car_run_3(void);
void Car_run_4(void );
void Car_run_5(void );
void Car_run_6(void );
void Car_run_7(void );
void Car_run_8(void );
void Car_run_9(void);
void Car_run_10(void );
void Car_run_11(void );
void Car_run_12(void );
                                        


#endif
