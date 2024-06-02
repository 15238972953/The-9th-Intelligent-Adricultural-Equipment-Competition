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
//初始5500
int N_servo=1;
int pos;
extern int openmv[1];
extern int openmv1[1];
extern u8 key_flag;
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

//pdf

void bozhong1(void){  //奇数垄
	/*
		if(openmv1[0]){
			a=0;    //a==0表示已经收到数据，不在接收
			USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
			switch(openmv1[0]){
				case 1: 
					circle_servo1+=22.5;
					break;
				case 2:
					circle_servo2+=22.5;
					break;
				case 3:
					circle_servo1+=22.5;
					circle_servo2+=22.5;
					break;
				default:
					break;
			}
			pill_Servo(circle_servo1,circle_servo2,40);
			openmv1[0]=0;
		}
		*/
		
		if(IO_1==0&&IO_2==0){
			pill_Servo(circle_servo1 ,circle_servo2,90);
			a=1;
		}else if(IO_1==1&&IO_2==1){
			pill_Servo(circle_servo1,circle_servo2,40);
		}
		if(IO_6==0&&a==1){
			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		}
}

void bozhong2(void){  //偶数垄
	/*
		if(openmv[0]){
			b=0;    //a==0表示已经收到数据，不在接收
			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
			switch(openmv[0]){
				case 1: 
					circle_servo1+=22.5;
					break;
				case 2:
					circle_servo2+=22.5;
					break;
				case 3:
					circle_servo1+=22.5;
					circle_servo2+=22.5;
					break;
				default:
					break;
			}
			pill_Servo(circle_servo1,circle_servo2,40);
			openmv[0]=0;
		}
		*/
		if(IO_1==0&&IO_2==0){
			pill_Servo(circle_servo1 ,circle_servo2,90);
			b=1;
		}else if(IO_1==1&&IO_2==1){
			pill_Servo(circle_servo1,circle_servo2,40);
		}
		if(IO_5==0&&b==1){
			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		}
}


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

void servo_bla(void )
{
	switch(flag)
	{
		case 1: servo_handle.error_f=8-Sonic_Dis.A_2;
						servo_handle.error_b=8-Sonic_Dis.A_6;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
						break;
							
					//		//Set_angle(123,58,45,126); //横着		
		case 2:servo_handle.error_f=13-Sonic_Dis.A_8;
						servo_handle.error_b=13-Sonic_Dis.A_7;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
	//竖直						
		case 3: servo_handle.error_f=8-Sonic_Dis.A_5;
						servo_handle.error_b=8-Sonic_Dis.A_3;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}	
						break;
		//水平
		case 4:servo_handle.error_f=13-Sonic_Dis.A_7;
						servo_handle.error_b=13-Sonic_Dis.A_8;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
									
		case 5: servo_handle.error_f=8-Sonic_Dis.A_6;//初始位8
						servo_handle.error_b=8-Sonic_Dis.A_2;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(3,SERVO_S_3-S_1); single_set(40,SERVO_S_4-S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						break;
		case 6:servo_handle.error_f=13-Sonic_Dis.A_8;
						servo_handle.error_b=13-Sonic_Dis.A_7;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
							
		case 7: servo_handle.error_f=8-Sonic_Dis.A_5;
						servo_handle.error_b=8-Sonic_Dis.A_3;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(3,SERVO_S_3); single_set(4,SERVO_S_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(3,SERVO_S_3+S_1); single_set(4,SERVO_S_4+S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(3,SERVO_S_3-S_1); single_set(4,SERVO_S_4-S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_S_1); single_set(2,SERVO_S_2);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_S_1+S_1); single_set(2,SERVO_S_2+S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_S_1-S_1); single_set(2,SERVO_S_2-S_1);delay_ms(10);}	
						break;
							
		case 8:servo_handle.error_f=13-Sonic_Dis.A_7;
						servo_handle.error_b=13-Sonic_Dis.A_8;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
							
		case 9: servo_handle.error_f=8-Sonic_Dis.A_6;
						servo_handle.error_b=8-Sonic_Dis.A_2;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(3,133); single_set(4,40);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(3,133-S_1); single_set(40,40-S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(3,133+S_1); single_set(4,40+S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,35); single_set(2,134);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,35-S_1); single_set(2,134-S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,35+S_1); single_set(2,134+S_1);delay_ms(10);}
						break;
		case 10:servo_handle.error_f=13-Sonic_Dis.A_8;
						servo_handle.error_b=13-Sonic_Dis.A_7;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						break;
							
		case 11:servo_handle.error_f=13-Sonic_Dis.A_7;
						servo_handle.error_b=13-Sonic_Dis.A_8;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(1,SERVO_B_1); single_set(3,SERVO_B_3);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(1,SERVO_B_1+S_2); single_set(3,SERVO_B_3+S_2);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(1,SERVO_B_1-S_2); single_set(3,SERVO_B_3-S_2);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(2,SERVO_B_2); single_set(4,SERVO_B_4);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(2,SERVO_B_2+S_2); single_set(4,SERVO_B_4+S_2);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(2,SERVO_B_2-S_2); single_set(4,SERVO_B_4-S_2);delay_ms(10);}
						break;
		
		case 12: servo_handle.error_f=8-Sonic_Dis.A_6;
						servo_handle.error_b=8-Sonic_Dis.A_2;
//						OLED_ShowIntNum(90,40,servo_handle.error_f,3,8,1);
//						OLED_ShowIntNum(90,50,servo_handle.error_b,3,8,1);
						if (servo_handle.error_f==0)
						{single_set(3,133); single_set(4,40);delay_ms(10);}
						else if (servo_handle.error_f>0)
						{single_set(3,133-S_1); single_set(40,40-S_1);delay_ms(10);}
						else if (servo_handle.error_f<0)
							{single_set(3,133+S_1); single_set(4,40+S_1);delay_ms(10);}
							
						if (servo_handle.error_b==0)
						{single_set(1,35); single_set(2,134);delay_ms(10);}
						else if (servo_handle.error_b>0)
						{single_set(1,35-S_1); single_set(2,134-S_1);delay_ms(10);}
						else if (servo_handle.error_b<0)
							{single_set(1,35+S_1); single_set(2,134+S_1);delay_ms(10);}
				
	}
}



void Car_run_1(void )
{
	
	flag =1;
	TIM3->CNT=0;
	TIM3_sumcode=0;
	
	//最开始跑定时器
	while(1)
	{		
		motion(-Speed_flag1,-Speed_flag1,Speed_flag1,-Speed_flag1);//--+-
		Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);//竖着
		if (Encoder_Timer3_sum>=6000) break;
	}
	//沿着墙走
	while(1)
	{
	servo_bla();
	motion(-Speed_flag1,-Speed_flag1,Speed_flag1,-Speed_flag1);//--+-
	if (  Sonic_Dis.A_1<45) break;	
	}
		while(1)
	{
	servo_bla();
	motion(-Speed_2,-Speed_2,Speed_2,-Speed_2); 
	if (Sonic_Dis.A_1<22)		break;	
	}
}

void Car_run_2(void )
{
	flag =2;
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
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if (Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)
				break;
	}
	
	while(1)
	{
		motion(-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10+PID_value);
		servo_bla();
		//bozhong1();
	if ( (Sonic_Dis.A_5>safe_dis && Sonic_Dis.A_5<slow_sp)||(Sonic_Dis.A_3>safe_dis && Sonic_Dis.A_3 <slow_sp)) break;	
	}
	
	while(1)
	{
	//	printf("%d",2);
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_5<safe_dis || Sonic_Dis.A_3<safe_dis)
		{TIM3->CNT=0;
		TIM3_sumcode=0;
		break;}
	}
}

void Car_run_3(void )
{
	flag =3;
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


	while(1)
	{	
		motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
		servo_bla();
		if (Encoder_Timer3_sum>count) break;
	}
}
	
void 	Car_run_4(void )
{
	flag =4;
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
		motion(Speed_2-PID_value,-Speed_2-PID_value,-Speed_2-PID_value,-Speed_2+PID_value);//+---
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
		//bozhong2();
		servo_bla();
		motion(Speed_1-PID_value,-Speed_1-PID_value,-Speed_1-PID_value,-Speed_1+PID_value);
	//servo_bla(13);
	if (Sonic_Dis.A_2<=slow_sp || Sonic_Dis.A_6<=slow_sp) break;	
	}
	while(1)
	{
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_2-PID_value,-Speed_2-PID_value,-Speed_2-PID_value,-Speed_2+PID_value);
		if (Sonic_Dis.A_2<safe_dis+2 || Sonic_Dis.A_6<safe_dis+2)
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}

}

void Car_run_5(void )
{
	flag =5;
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

	while(1)
	{	
		motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
        //motion(Speed_3,Speed_3,-Speed_3,Speed_3);
		servo_bla();
	//	printf("%d",Encoder_Timer3_sum);
		if (Encoder_Timer3_sum>(count-200)) break;
	}
}

void Car_run_6(void )
{
	flag =6;
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
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		if ((Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
		//bozhong1();
		servo_bla();	
		motion(-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10+PID_value);
	//servo_bla_2(13);
	if (Sonic_Dis.A_5<slow_sp || Sonic_Dis.A_3<slow_sp) break;	
	}
	while(1)
	{
	Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_5<safe_dis || Sonic_Dis.A_3<safe_dis)
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}
}

void Car_run_7(void )
{
	flag =7;
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

	while(1)
	{	
		motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
		servo_bla();
		if (Encoder_Timer3_sum>count) break;
	}
}

void 	Car_run_8(void )
{
	flag =8;
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
		motion(Speed_2-PID_value,-Speed_2-PID_value,-Speed_2-PID_value,-Speed_2+PID_value);//+---
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
		servo_bla();
		//bozhong2();
	motion(Speed_1-PID_value,-Speed_1-PID_value,-Speed_1-PID_value,-Speed_1+PID_value);
	if (Sonic_Dis.A_2<=slow_sp || Sonic_Dis.A_6<=slow_sp) break;	
	}
	while(1)
	{
	Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_2-PID_value,-Speed_2-PID_value,-Speed_2-PID_value,-Speed_2+PID_value);
		if (Sonic_Dis.A_2<safe_dis+2 || Sonic_Dis.A_6<safe_dis+2)
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}
}

void Car_run_9(void )
{
	flag =9;
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

	while(1)
	{	
		motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
        //motion(Speed_3,Speed_3,-Speed_3,Speed_3); //++-+
		servo_bla();
		if (Encoder_Timer3_sum>(count-200)) break;
	}
}

void Car_run_10(void )
{
	flag =10;
	TIM3->CNT=0;
	TIM3_sumcode=0;
	TIM_Cmd(TIM13,DISABLE);
	motion(0,0,0,0);
    Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
	delay_ms(200);
	TIM_Cmd(TIM13,ENABLE);

	while(1)
	{		
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
		servo_bla();
		//bozhong1();
	motion(-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,Speed_2_6_10+PID_value);
	if (Sonic_Dis.A_5<slow_sp || Sonic_Dis.A_3<slow_sp) break;	
	}
	while(1)
	{
	Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(-Speed_2-PID_value,Speed_2-PID_value,Speed_2-PID_value,Speed_2+PID_value);//-+++
		if (Sonic_Dis.A_5<safe_dis || Sonic_Dis.A_3<safe_dis)
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}
}

void 	Car_run_11(void )
{
	flag =11;
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
	motion(Speed_1-PID_value,-Speed_1-PID_value,-Speed_1-PID_value,-Speed_1+PID_value);//+---
	Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		if ((Sonic_Dis.A_7<=In_correct || Sonic_Dis.A_8<=In_correct)) break;
	}
	while(1)
	{
	servo_bla();
    if(empt==1)
	{
	//TIM_Cmd(TIM13,DISABLE);
//		motion(0,0,0,0);
//		delay_ms(1000);
	Action
	//TIM_Cmd(TIM13,ENABLE);
	empt=0;
	flag_openmv=0;
	}	
	motion(Speed_1-PID_value,-Speed_1-PID_value,-Speed_1-PID_value,-Speed_1+PID_value);
	if (Sonic_Dis.A_2<=slow_sp) break;	
	}
	while(1)
	{
	Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);//横着
		motion(Speed_2-PID_value,-Speed_2-PID_value,-Speed_2-PID_value,-Speed_2+PID_value);
		if (Sonic_Dis.A_2<(safe_dis-3))
		{TIM3->CNT=0;
		TIM3_sumcode=0;break;}
	}

}

void Car_run_12(void )
{
	flag =12;
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

//    
	while(1)
	{	
        motion(Speed_flag1,Speed_flag1,-Speed_flag1,Speed_flag1); //++-+
		//motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
		servo_bla();
	//	printf("%d",Encoder_Timer3_sum);
        		if (Sonic_Dis.A_6>23) break;
//		if (Encoder_Timer3_sum>(count)||Encoder_Timer4_sum>(count)) break;
	}
	while(1)
	{	
        Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3-5,SERVO_S_4-5);//竖着
        //motion(Speed_3+PID_value,Speed_3+PID_value,-Speed_3+PID_value,Speed_3-PID_value); //++-+
		motion(Speed_flag1,Speed_flag1,-Speed_flag1,Speed_flag1); //++-+
		
//		if (Sonic_Dis.A_6>25) break;
     if (Encoder_Timer3_sum>8300) break;
	}
}

