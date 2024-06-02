#ifndef COM_H
#define COM_H
#include "sys.h"

#define S_1  3 //竖向第一段初始值4
#define S_2  4 //横向第一段初始值4
#define Speed_flag1 999 //初始值为730
#define Speed_1 740//初始730
#define Speed_2_6_10 850//初始730
#define Speed_2 600 //初始值为500
#define Speed_3 840
#define safe_dis 15 //初始值18
#define slow_sp  40
//竖直
#define SERVO_S_1  38
#define SERVO_S_2  143
#define SERVO_S_3  133
#define SERVO_S_4  47

//水平
#define SERVO_B_1  98
#define SERVO_B_2  58
#define SERVO_B_3  45
#define SERVO_B_4  134

typedef struct
{
	int error_f;// 2号超声波在前
	int error_b;
}Blance;

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
