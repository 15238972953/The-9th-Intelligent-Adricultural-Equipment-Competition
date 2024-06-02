#ifndef COM_H
#define COM_H
#include "sys.h"

#define S_1  3 //�����һ�γ�ʼֵ4
#define S_2  4 //�����һ�γ�ʼֵ4
#define Speed_flag1 999 //��ʼֵΪ730
#define Speed_1 740//��ʼ730
#define Speed_2_6_10 850//��ʼ730
#define Speed_2 600 //��ʼֵΪ500
#define Speed_3 840
#define safe_dis 15 //��ʼֵ18
#define slow_sp  40
//��ֱ
#define SERVO_S_1  38
#define SERVO_S_2  143
#define SERVO_S_3  133
#define SERVO_S_4  47

//ˮƽ
#define SERVO_B_1  98
#define SERVO_B_2  58
#define SERVO_B_3  45
#define SERVO_B_4  134

typedef struct
{
	int error_f;// 2�ų�������ǰ
	int error_b;
}Blance;

void bozhong1(void); //����¢
void bozhong2(void); //ż��¢
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
