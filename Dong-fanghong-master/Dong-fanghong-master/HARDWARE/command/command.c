#include "command.h"
#include "bsp_pid.h"
#include "motor.h"
#include "math.h"
#include "servo.h"
#include "variable.h"
#include "stdio.h"
#define In_correct 20
#define close 90
#define open  130
#define count  5500 //初始值5750
#define speed_slow 300  //减速速度
#define speed_up 625    //加速速度
//初始5500
int N_servo=1;
int pos;
extern int openmv[1];
extern int openmv1[1];
extern u8 key_flag;
extern volatile int stop_flag;
extern volatile int flag_kualong;
extern volatile int pdf_pp;
int Speed_1=625;
int Speed_zuo=625;//用于出隆使车子变平
int Speed_you=625;
int Speed_2_6_10=625;//全哥初始850
int Speed_2 = 625; //全哥初始值为600

int dis_78 = 8; //78超声波距垄的距离


//#define Action 		{\
//	pill_Servo(N_servo*15-12,open);\
//	delay_ms(500);\
//	pill_Servo(N_servo*15-12,close);\
//    delay_ms(100);\
//    for (pos=(N_servo*15-12); pos <= ((N_servo+1)*15-12);pos+= 1) \
//    {\
//    pill_Servo(pos,close);\
//    delay_ms(15);\
//    }\
//	N_servo++;\
//}
#define Action USART_SendData(USART1,1);

Blance servo_handle;

void speed_PID(float TIM3_speed,float TIM4_speed)//速度pid
{
		int setpwm_3; 
		int setpwm_4;
		setpwm_3=(int)speed_pid_set(10,0,0,116,TIM3_speed);
		setpwm_4=(int)speed_pid_set(10,0,0,116,TIM4_speed);
		motion(setpwm_3,setpwm_3,-setpwm_4,-setpwm_4);
}

void com_PID(int Encoder_Timer3_sum,int Encoder_Timer4_sum)//串级pid
{
	int setpwm_3;
	int setpwm_4;
	float Encoder_Timer3_place;
	float Encoder_Timer4_place;
	Encoder_Timer3_place = location_pid_set(100,0,0,8553, Encoder_Timer3_sum);
	Encoder_Timer4_place = location_pid_set(100,0,0,8553, Encoder_Timer4_sum);
	setpwm_3=(int)speed_pid_set(10,0,0,11550,Encoder_Timer3_place);
	setpwm_4=(int)speed_pid_set(10,0,0,11550,Encoder_Timer4_place);
	motion(setpwm_3,setpwm_3,setpwm_4,setpwm_4);
}

//限幅函数
//k为要被限幅的参数，qujian为上下区间值
int xianfu(int k,int qujian){
	qujian=qujian>0?qujian:-qujian;
	if(k>qujian)
		return qujian;
	else if(k<-qujian)
		return qujian;
	else
		return k;
}

void servo_bla(void){
	int angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4;
	switch(flag) 
	{
		case 1: servo_handle.error_f=xianfu(5-Sonic_Dis.A_2,10);
						servo_handle.error_b=xianfu(6-Sonic_Dis.A_6,10);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
		
						angle_servo_1=SERVO_S_1+servo_handle.error_f;
						angle_servo_2=SERVO_S_2+servo_handle.error_f;
						angle_servo_3=SERVO_S_3+servo_handle.error_b;
						angle_servo_4=SERVO_S_4+servo_handle.error_b;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
		
//						if (servo_handle.error_f==0)
//						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
						break;
							
					//		//Set_angle(123,58,45,126); //横着		
		case 2:servo_handle.error_f=xianfu(dis_78-Sonic_Dis.A_8,20);
						servo_handle.error_b=xianfu(dis_78-Sonic_Dis.A_7,20);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
							
						angle_servo_1=SERVO_B_1-servo_handle.error_b;
						angle_servo_2=SERVO_B_2-servo_handle.error_f;
						angle_servo_3=SERVO_B_3-servo_handle.error_b;
						angle_servo_4=SERVO_B_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
							
//						if (servo_handle.error_f==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_B_1-S_2+1); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
	//竖直						
		case 3: 
			Sonic_Dis.A_3=xianfu(Sonic_Dis.A_3,15);
			Sonic_Dis.A_5=xianfu(Sonic_Dis.A_5,15);
			servo_handle.error_f=xianfu(2-Sonic_Dis.A_3,10);
			servo_handle.error_b=xianfu(4-Sonic_Dis.A_5,10);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);

						angle_servo_1=SERVO_S_1+servo_handle.error_b;
						angle_servo_2=SERVO_S_2+servo_handle.error_b;
						angle_servo_3=SERVO_S_3+servo_handle.error_f;
						angle_servo_4=SERVO_S_4+servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
						
//						if (servo_handle.error_f==0)
//						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(5);}
//						else if (servo_handle.error_f>0)
//						{single_set(3,SERVO_S_3+S_1+2); single_set(4,SERVO_S_4+S_1+2);delay_ms(5);}
//						else if (servo_handle.error_f<0)
//							{single_set(3,SERVO_S_3-S_1-2); single_set(4,SERVO_S_4-S_1-2);delay_ms(5);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(5);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_S_1+S_1+2); single_set(2,SERVO_S_2+S_1+2);delay_ms(5);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_S_1-S_1-2); single_set(2,SERVO_S_2-S_1-2);delay_ms(5);}	
						break;
		//水平
		case 4:
						servo_handle.error_f=xianfu(10-Sonic_Dis.A_7,10);
						servo_handle.error_b=xianfu(10-Sonic_Dis.A_8,10);
		
						angle_servo_1=SERVO_B_1+servo_handle.error_f;
						angle_servo_2=SERVO_B_2+servo_handle.error_b;
						angle_servo_3=SERVO_B_3+servo_handle.error_f;
						angle_servo_4=SERVO_B_4+servo_handle.error_b;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
					
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
									
		case 5: 
						Sonic_Dis.A_2=xianfu(Sonic_Dis.A_2,15);
						Sonic_Dis.A_6=xianfu(Sonic_Dis.A_6,15);
						servo_handle.error_f=xianfu(6-Sonic_Dis.A_6,10);//初始位8
						servo_handle.error_b=xianfu(5-Sonic_Dis.A_2,10);
						
						angle_servo_1=SERVO_S_1-servo_handle.error_b;
						angle_servo_2=SERVO_S_2-servo_handle.error_b;
						angle_servo_3=SERVO_S_3-servo_handle.error_f;
						angle_servo_4=SERVO_S_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						break;
		case 6:servo_handle.error_f=xianfu(8-Sonic_Dis.A_8,10);
						servo_handle.error_b=xianfu(8-Sonic_Dis.A_7,10);
						
						angle_servo_1=SERVO_B_1-servo_handle.error_b;
						angle_servo_2=SERVO_B_2-servo_handle.error_f;
						angle_servo_3=SERVO_B_3-servo_handle.error_b;
						angle_servo_4=SERVO_B_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
							
		case 7: 			
			Sonic_Dis.A_3=xianfu(Sonic_Dis.A_3,15);
			Sonic_Dis.A_5=xianfu(Sonic_Dis.A_5,15);
			servo_handle.error_f=xianfu(2-Sonic_Dis.A_3,10);
			servo_handle.error_b=xianfu(4-Sonic_Dis.A_5,10);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);

						angle_servo_1=SERVO_S_1+servo_handle.error_b;
						angle_servo_2=SERVO_S_2+servo_handle.error_b;
						angle_servo_3=SERVO_S_3+servo_handle.error_f;
						angle_servo_4=SERVO_S_4+servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}	
						break;
							
		case 8:						
						servo_handle.error_f=xianfu(10-Sonic_Dis.A_7,10);
						servo_handle.error_b=xianfu(10-Sonic_Dis.A_8,10);
		
						angle_servo_1=SERVO_B_1+servo_handle.error_f;
						angle_servo_2=SERVO_B_2+servo_handle.error_b;
						angle_servo_3=SERVO_B_3+servo_handle.error_f;
						angle_servo_4=SERVO_B_4+servo_handle.error_b;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2+2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
							
		case 9: Sonic_Dis.A_2=xianfu(Sonic_Dis.A_2,20);
						Sonic_Dis.A_6=xianfu(Sonic_Dis.A_6,20);
						servo_handle.error_f=xianfu(6-Sonic_Dis.A_6,15);//初始位8
						servo_handle.error_b=xianfu(5-Sonic_Dis.A_2,15);
						
						angle_servo_1=SERVO_S_1-servo_handle.error_b;
						angle_servo_2=SERVO_S_2-servo_handle.error_b;
						angle_servo_3=SERVO_S_3-servo_handle.error_f;
						angle_servo_4=SERVO_S_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						break;
		case 10:servo_handle.error_f=xianfu(8-Sonic_Dis.A_8,10);
						servo_handle.error_b=xianfu(8-Sonic_Dis.A_7,10);
						
						angle_servo_1=SERVO_B_1-servo_handle.error_b;
						angle_servo_2=SERVO_B_2-servo_handle.error_f;
						angle_servo_3=SERVO_B_3-servo_handle.error_b;
						angle_servo_4=SERVO_B_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
							
		case 11:						
						servo_handle.error_f=xianfu(10-Sonic_Dis.A_7,10);
						servo_handle.error_b=xianfu(10-Sonic_Dis.A_8,10);
		
						angle_servo_1=SERVO_B_1+servo_handle.error_f;
						angle_servo_2=SERVO_B_2+servo_handle.error_b;
						angle_servo_3=SERVO_B_3+servo_handle.error_f;
						angle_servo_4=SERVO_B_4+servo_handle.error_b;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
//						if (servo_handle.error_f==0)
//						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
//						else if (servo_handle.error_f>0)
//						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
//						else if (servo_handle.error_f<0)
//							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2-2);delay_ms(10);}
//							
//						if (servo_handle.error_b==0)
//						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
//						else if (servo_handle.error_b>0)
//						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
//						else if (servo_handle.error_b<0)
//							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
		
		case 12: 
						Sonic_Dis.A_2=xianfu(Sonic_Dis.A_2,15);
						Sonic_Dis.A_6=xianfu(Sonic_Dis.A_6,15);
						servo_handle.error_f=xianfu(6-Sonic_Dis.A_6,10);//初始位8
						servo_handle.error_b=xianfu(5-Sonic_Dis.A_2,10);
						
						angle_servo_1=SERVO_S_1-servo_handle.error_b;
						angle_servo_2=SERVO_S_2-servo_handle.error_b;
						angle_servo_3=SERVO_S_3-servo_handle.error_f;
						angle_servo_4=SERVO_S_4-servo_handle.error_f;
						Set_angle(angle_servo_1,angle_servo_2,angle_servo_3,angle_servo_4);
				
	}
}

void Car_run_1(void )
{
	
	flag =1;
	USART_SendData(USART1, flag); 
	
	//最开始跑延时
	while(1){
		motion(Speed_flag1,Speed_flag1,Speed_flag1,Speed_flag1);//--+-
		Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
		if(Sonic_Dis.A_6<20)break;
	}
	//沿着墙走
	while(1)
	{
	servo_bla();
	motion(Speed_flag1,Speed_flag1,Speed_flag1,Speed_flag1);//--+-
	if (  Sonic_Dis.A_1<50) break;	
	}
		while(1)
	{
	servo_bla();
	motion(Speed1_3,Speed1_3,Speed1_3,Speed1_3); 
	if (Sonic_Dis.A_1<=25)		break;	
	}
}

void Car_run_2(void)
{
	flag =2;
	USART_SendData(USART1, flag); 
	motion(0,0,0,0);
  Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(500);
	Speed_2=700;
	while(1)
	{		
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
		motion(Speed_2+PID_value,-Speed_2-PID_value,Speed_2-PID_value,-Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)
				break;
	}
	Speed_2_6_10=700;
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2_6_10=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2_6_10=speed_up;
//		}
		motion(Speed_2_6_10+PID_value,-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,-Speed_2_6_10+PID_value);
		//motion(Speed_2_6_10,-Speed_2_6_10,Speed_2_6_10,-Speed_2_6_10);
		servo_bla();
		//bozhong1();
	if ( (Sonic_Dis.A_5>=safe_dis && Sonic_Dis.A_5<=slow_sp)&&(Sonic_Dis.A_3>=safe_dis && Sonic_Dis.A_3 <=slow_sp)) break;	
	}
	
	while(1)
	{
	//	printf("%d",2);
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_2+PID_value,-Speed_2-PID_value,Speed_2-PID_value,-Speed_2+PID_value);//-+++
		//motion(Speed_2,-Speed_2,Speed_2,-Speed_2);//-+++
//		if (Sonic_Dis.A_5<safe_dis+5){
//			Speed_zuo=0;
//		}
//		if(Sonic_Dis.A_3<safe_dis){
//			Speed_you=0;
//		}
		if(Sonic_Dis.A_5<safe_dis+5 || Sonic_Dis.A_3<safe_dis+5){
			break;
		}
	}
}

void Car_run_3(void )
{
	flag =3;
	int i=0,k=0;
//	USART_SendData(USART1, flag); 
	motion(0,0,0,0);
    Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
	delay_ms(200);
	//TIM_Cmd(TIM6,ENABLE);
	while(1)
	{	
		motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
		servo_bla();
//		if(22==flag_kualong){
//			flag_kualong=0;
//			break;
//		}
		if(0==IO_5 && 0==k){
			i++;
			if(2==i){
				break;
			}
			k=1;
		}else if(1==IO_5){
			k=0;
		}
	}
	//TIM_Cmd(TIM6,DISABLE);
}
	
void 	Car_run_4(void )
{
	flag =4;
	Speed_2=575;
	Speed_1=600;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //关闭串口3
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); //开启串口4
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
		motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);//+---
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_1=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_1=speed_up;
//		}
		servo_bla();
		motion(-Speed_1+PID_value,Speed_1-PID_value,-Speed_1-PID_value,Speed_1+PID_value);
	//servo_bla(13);
	if ((Sonic_Dis.A_2<=slow_sp)&&(Sonic_Dis.A_2>=safe_dis) && (Sonic_Dis.A_6<=slow_sp)&&(Sonic_Dis.A_6>=safe_dis)) break;	
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_1+PID_value,Speed_1-PID_value,-Speed_1-PID_value,Speed_1+PID_value);
		if (Sonic_Dis.A_2<safe_dis && Sonic_Dis.A_6<safe_dis)
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}

}

void Car_run_5(void )
{
	flag =5;
	int i=0,k=0;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	Encoder_Timer3_sum=0;
	TIM4->CNT=0;
	TIM4_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
	Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	//TIM_Cmd(TIM6,ENABLE);
	while(1)
	{	
		motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
        //motion(Speed_3,Speed_3,-Speed_3,Speed_3);
		servo_bla();
//		if(20==flag_kualong){
//			flag_kualong=0;
//			break;
//		}
		if(0==IO_6 && 0==k){
			i++;
			if(2==i){
				break;
			}
			k=1;
		}else if(1==IO_6){
			k=0;
		}
	}
	//TIM_Cmd(TIM6,DISABLE);
}

void Car_run_6(void )
{
	flag =6;
	Speed_2=625;
	Speed_2_6_10=625;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM4->CNT=0;
	TIM4_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE); //关闭串口3
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE); //开启串口4
	while(1)
	{		
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
		motion(Speed_2+PID_value,-Speed_2-PID_value,Speed_2-PID_value,-Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)
				break;
	}
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2_6_10=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2_6_10=speed_up;
//		}
		motion(Speed_2_6_10+PID_value,-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,-Speed_2_6_10+PID_value);
		//motion(Speed_2_6_10,-Speed_2_6_10,Speed_2_6_10,-Speed_2_6_10);
		servo_bla();
		//bozhong1();
	if ( (Sonic_Dis.A_5>=safe_dis && Sonic_Dis.A_5<=slow_sp)&&(Sonic_Dis.A_3>=safe_dis && Sonic_Dis.A_3 <=slow_sp)) break;	
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_2+PID_value,-Speed_2-PID_value,Speed_2-PID_value,-Speed_2+PID_value);//-+++
		//motion(Speed_2,-Speed_2,Speed_2,-Speed_2);//-+++

		if(Sonic_Dis.A_5<=safe_dis && Sonic_Dis.A_3<=safe_dis){
			break;
		}
	}
}

void Car_run_7(void )
{
	flag =7;
	int i=0,k=0;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	Encoder_Timer3_sum=0;
	TIM4->CNT=0;
	TIM4_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	//TIM_Cmd(TIM6,ENABLE);
	while(1)
	{	
		motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
		servo_bla();
//		if(20==flag_kualong){
//			flag_kualong=0;
//			break;
//		}
		if(0==IO_5 && 0==k){
			i++;
			if(2==i){
				break;
			}
			k=1;
		}else if(1==IO_5){
			k=0;
		}
	}
	//TIM_Cmd(TIM6,DISABLE);
}

void 	Car_run_8(void )
{
	flag =8;
	Speed_2=625;
	Speed_2_6_10=625;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); //关闭串口3
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE); //开启串口4
	
	while(1)
	{		
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
		motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);//+---
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2_6_10=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2_6_10=speed_up;
//		}
		motion(-Speed_2_6_10+PID_value,Speed_2_6_10-PID_value,-Speed_2_6_10-PID_value,Speed_2_6_10+PID_value);//+---
		servo_bla();
	if ((Sonic_Dis.A_2<=slow_sp)&&(Sonic_Dis.A_2>=safe_dis) && (Sonic_Dis.A_6<=slow_sp)&&(Sonic_Dis.A_6>=safe_dis)) break;	
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);
		if (Sonic_Dis.A_2<safe_dis || Sonic_Dis.A_6<safe_dis)
		{break;}
	}
}

void Car_run_9(void )
{
	flag =9;
	int i=0,k=0;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	Encoder_Timer3_sum=0;
	TIM4->CNT=0;
	TIM4_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	//TIM_Cmd(TIM6,ENABLE);
	while(1)
	{	
		motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
        //motion(Speed_3,Speed_3,-Speed_3,Speed_3);
		servo_bla();
//		if(20==flag_kualong){
//			flag_kualong=0;
//			break;
//		}
		if(0==IO_6 && 0==k){
			i++;
			if(2==i){
				break;
			}
			k=1;
		}else if(1==IO_6){
			k=0;
		}
	}
	//TIM_Cmd(TIM6,DISABLE);
}

void Car_run_10(void )
{
	flag =10;
	Speed_2=625;
	Speed_2_6_10=625;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);

	while(1)
	{		
		motion(Speed_2+PID_value,-Speed_2-PID_value,Speed_2-PID_value,-Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)
				break;
	}
	while(1)
	{
		motion(Speed_2_6_10+PID_value,-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,-Speed_2_6_10+PID_value);
		//motion(Speed_2_6_10,-Speed_2_6_10,Speed_2_6_10,-Speed_2_6_10);
		servo_bla();
		//bozhong1();
	if ( (Sonic_Dis.A_5>=safe_dis && Sonic_Dis.A_5<=slow_sp)&&(Sonic_Dis.A_3>=safe_dis && Sonic_Dis.A_3 <=slow_sp)) break;
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_zuo+PID_value,-Speed_zuo-PID_value,Speed_you-PID_value,-Speed_you+PID_value);//-+++
		//motion(Speed_2,-Speed_2,Speed_2,-Speed_2);//-+++
		if(Sonic_Dis.A_5<=safe_dis || Sonic_Dis.A_3<=safe_dis){
			break;
		}
	}
}

void 	Car_run_11(void )
{
	flag =11;
	Speed_2=600;
	
	USART_SendData(USART1, flag); 
    empt=0;
    flag_openmv=0;
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);

	while(1)
	{		
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
	motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);//+---
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if((Sonic_Dis.A_7<=In_correct && Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
//		if(2==stop_flag){
//			stop_flag=0;
//			Speed_2=speed_slow;
//		}else{
//			stop_flag=0;
//			Speed_2=speed_up;
//		}
		motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);//+---
		servo_bla();
	if (Sonic_Dis.A_2<=slow_sp&&Sonic_Dis.A_2>=safe_dis && Sonic_Dis.A_6<=slow_sp&&Sonic_Dis.A_6>=safe_dis) break;	
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2+PID_value,Speed_2-PID_value,-Speed_2-PID_value,Speed_2+PID_value);
		if (Sonic_Dis.A_2<19 || Sonic_Dis.A_6<20)
		{break;}
	}

}

void Car_run_12(void )
{
	flag =12;
	USART_SendData(USART1, flag); 
	TIM3->CNT=0;
	TIM3_sumcode=0;
	Encoder_Timer3_sum=0;
	TIM4->CNT=0;
	TIM4_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
    
	while(1)
	{	
		motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
		servo_bla();
    if (Sonic_Dis.A_6>=25) 
			break;
	}
	Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);
	while(1)
	{	
        motion(-Speed_3+PID_value,-Speed_3-PID_value,-Speed_3-PID_value,-Speed_3+PID_value); //++-+
        //motion(Speed_3,Speed_3,-Speed_3,Speed_3);
		if(28==flag_kualong){
			flag_kualong=0;
			break;
		}
	}
	TIM_Cmd(TIM13,DISABLE);
}

