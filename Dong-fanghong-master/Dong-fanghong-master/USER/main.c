#include "variable.h"
int flag=0;
int code=0;
volatile int flag_kualong=0;
extern int openmv[1];
extern int openmv1[1];
u8 key_flag=0;
extern volatile int stop_flag;
extern volatile int pdf_pp;

//�����ʵ���Ӧ
//1 2  1��2�ŵ����Ҫ���40��PWM��ԭ���꣬������1��2������߱�3/4�ų���ԭ�򣬴���֤
//4 3
//�����ʵ���Ӧ
//1 2
//3 4
//������
//  1
//2   5  
// 8 7
//6   3
//  4

int main(void)																																																																																																																																																																																																
{
	ALL_Init();//ȫ�������ʼ��
	TLY_init();//�����Ǻ�����ʼ��
	Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);
	//Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);
	//pill_Servo(circle_servo1,circle_servo2,40); //���ֶ���ĳ�ʼ״̬
//	delay_ms(1000);
//	while(1){
//		motion(0,0,0,0);
//		Set_angle(SERVO_B_1,SERVO_B_2,SERVO_B_3,SERVO_B_4);
//	}
	
//	while(1){
//		key_flag=1;
//		if(KEY1==0){
//			Car_run_8();
//			while(1){
//				motion(0,0,0,0);
//			}
//		}else{
//			motion(0,0,0,0);
//		}
//		
//		}
	
	
//	while(1){
//		if(KEY1==0){
//			Car_run_3();
//			OLED_ShowNum(10,20,flag_kualong,1,8,1);
//			OLED_Refresh();
//			delay_ms(10);
//		}
//	while(1){
//		motion(0,0,0,0);
//	}
//}		
		

	
//	flag =2;
//	while(1){
//		motion(Speed_2_6_10,-Speed_2_6_10,Speed_2_6_10,-Speed_2_6_10);
//		//motion(Speed_2_6_10+PID_value,-Speed_2_6_10-PID_value,Speed_2_6_10-PID_value,-Speed_2_6_10+PID_value);
//		servo_bla();
//	}
//			while(1){
//				motion(640,-640,600,-600);  //��¡��ǰ��

//			}
//����������

//	motion(0,0,0,0);
//	while(1){
//		Ultra_Test1();
//	}
//	while(1){
//		if(KEY1==0){
//			GPIO_SetBits(GPIOB,GPIO_Pin_4);//GPIOF9,F10���øߣ�����
//			GPIO_ResetBits(GPIOB,GPIO_Pin_5);
//			GPIO_ResetBits(GPIOE,GPIO_Pin_4);//GPIOF9,F10���øߣ�����
//			GPIO_SetBits(GPIOE,GPIO_Pin_7);
//			TIM_SetCompare1(TIM10,600);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//			TIM_SetCompare1(TIM11,600);
//			TIM_SetCompare1(TIM9,600);
//			TIM_SetCompare2(TIM9,600);
//			delay_ms(10000);
//		}else{
//				TIM_SetCompare1(TIM10,0);	//�޸ıȽ�ֵ���޸�ռ�ձ�
//		TIM_SetCompare1(TIM11,0);
//		TIM_SetCompare1(TIM9,0);
//		TIM_SetCompare2(TIM9,0);
//		}
//	}
	//����
	/*
	int a=0;
	while(1){
		if(openmv1[0]){
			a=0;    //a===0��ʾ�Ѿ��յ����ݣ����ڽ���
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
		if(IO_1==0&&IO_2==0){
			motion(0,0,0,0);
			pill_Servo(circle_servo1 ,circle_servo2,90);
			delay_ms(500);
			pill_Servo(circle_servo1,circle_servo2,40);
			a=1;
		}
		if(IO_6==0&&a==1){
			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
		}
	}
	*/
//	int i=0;
//	while(1){
//		OLED_ShowNum(0,0,stop_flag,1,8,1);
//		OLED_Refresh();
//		delay_ms(10);
//	}

//while(1){
//		OLED_ShowNum(0,0,Sonic_Dis.A_7,2,8,1);
//		OLED_ShowNum(0,10,Sonic_Dis.A_8,2,8,1);
//		OLED_Refresh();
//		delay_ms(10);
//}
//while(1){
//	single_set(1,123);
//	single_set(2,56);
//	single_set(3,SERVO_B_3);
//	single_set(4,SERVO_B_4);
//}
//while(1){
//			if(KEY1==0) //ͣ������
//		{
//			while(1){
//			Car_run_10();
//			}
//		}
//}
	while(1)	
	{
//		printf("%lf",YAW);
		key_flag=1;
		if(KEY1==0) 
		{
//			flag=4;
//			USART_SendData(USART1, flag);
//			while(1){motion(0,0,0,0);}
		Car_run_1();
		Car_run_2();
		Car_run_4();
		Car_run_5();
		Car_run_6();
//		while(1){motion(0,0,0,0);}
		Car_run_7();
		Car_run_8();
		Car_run_9();
		Car_run_10();
		Car_run_7();
		Car_run_11();
		Car_run_12();
		flag=13;
		}
		
		if(KEY2==0) //��ͣ������
		{
		Car_run_1();
		Car_run_2();
		Car_run_3();
		Car_run_4();
		Car_run_5();
		Car_run_6();
		Car_run_7();
		Car_run_8();
		Car_run_9();
		Car_run_10();
		Car_run_11();
		Car_run_12();
		flag=13;
		}
		else motion(0,0,0,0);
	}
	return 0;
}		
