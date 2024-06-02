#include "variable.h"
int flag=0;
int code=0;

extern volatile int openmv[50];
extern volatile int openmv1[50];
u8 key_flag=0;
extern volatile int a;
extern volatile int b;
extern volatile int re_flag;
int color_U3=0;
int color_U4=0;
extern volatile int count_U3;
extern volatile int count_U4;
extern int pdf;
u8 flag_stop1=1;
u8 flag_stop2=1;
u8 flag_slow1=1;
u8 flag_slow2=1;
u8 flag_up1=1;
u8 flag_up2=1;
//得到众数
int findMode(int arr[], int size) {
    // 计算每个元素出现的次数
    int count[size];
    for (int i = 0; i < size; i++) {
        count[i] = 0;
        for (int j = 0; j < size; j++) {
            if (arr[j] == arr[i]) {
                count[i]++;
            }
        }
    }
    // 找到出现次数最多的元素
    int maxCount = 0;
    int mode = arr[0];
    for (int i = 0; i < size; i++) {
        if (count[i] > maxCount) {
            maxCount = count[i];
            mode = arr[i];
        }
    }
    return mode;
}


int main(void)
{
	ALL_Init();//全部外设初始化
	TLY_init();//陀螺仪函数初始化
	Set_angle(SERVO_S_1,SERVO_S_2,SERVO_S_3,SERVO_S_4);
	pill_Servo(circle_servo1,circle_servo2,small_servo3); //播种舵机的初始状态
	
	//测试
	//单次检测
	//奇数垄
//	while(1){
//		if(0==IO_1&&1==b){
//			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//		}
//		if(0==IO_2){
//			small_servo3=110;
//			if(1==IO_1){
//				b=1;
//			}
//		}else{
//			small_servo3=40;
//		}
//		pill_Servo(circle_servo1,circle_servo2,small_servo3);
//		OLED_ShowNum(0,0,IO_1,1,8,1);
//		OLED_ShowNum(10,0,IO_2,1,8,1);
//		OLED_ShowNum(0,10,openmv[0],1,8,1);
//		OLED_ShowNum(0,20,openmv[1],1,8,1);
//		OLED_ShowNum(0,30,openmv[2],1,8,1);
//		OLED_ShowNum(0,40,openmv[3],1,8,1);
//		OLED_ShowNum(0,50,openmv[4],1,8,1);
//		OLED_Refresh();
//		delay_ms(10);
//	}

//偶数龙
//	while(1){
//		if(0==IO_6&&1==a){
//			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//		}
//		if(0==IO_5){
//			small_servo3=110;
//			if(1==IO_6){
//				a=1;
//			}
//		}else{
//			small_servo3=40;
//		}
//		pill_Servo(circle_servo1,circle_servo2,small_servo3);
//		OLED_ShowNum(0,0,IO_6,1,8,1);
//		OLED_ShowNum(10,0,IO_5,1,8,1);
//		OLED_ShowNum(0,10,openmv1[0],1,8,1);
//		OLED_ShowNum(0,20,openmv1[1],1,8,1);
//		OLED_ShowNum(0,30,openmv1[2],1,8,1);
//		OLED_ShowNum(0,40,openmv1[3],1,8,1);
//		OLED_ShowNum(0,50,openmv1[4],1,8,1);
//		OLED_Refresh();
//		delay_ms(10);
//	}
	

//	while(1){
//		if(0==b){
//			b=1;
//			color_U3=findMode(openmv,count_U3);
//			count_U3=0;
//			memset(openmv, 0, sizeof(openmv));
//			switch(color_U3){
//						case 1: 
//							circle_servo1+=22.5;
//							break;
//						case 2:
//							circle_servo2+=22.5;
//							break;
//						case 3:
//							circle_servo1+=22.5;
//							circle_servo2+=22.5;
//							break;
//						default:
//							break;
//					}
//		}
//		if(0==IO_2){
//			small_servo3=110;
//			if(1==IO_1){
//				USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//			}
//		}else{
//			small_servo3=40;
//		}
//		pill_Servo(circle_servo1,circle_servo2,small_servo3);
//		OLED_ShowNum(0,0,color_U3,1,8,1);
//		OLED_ShowNum(0,10,count_U3,2,8,1);
//		OLED_ShowNum(0,20,openmv[0],1,8,1);
//		OLED_ShowNum(10,20,openmv[1],1,8,1);
//		OLED_ShowNum(20,20,openmv[2],1,8,1);
//		OLED_ShowNum(30,20,openmv[3],1,8,1);
//		OLED_ShowNum(40,20,openmv[4],1,8,1);
//		OLED_Refresh();
//		delay_ms(10);
//	}


		//24.5.11测试
//		USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//		while(1){
//			
//		}
	
	//24.4.26测试
	
//	while(1){
//		//奇数垄
//				if(0==IO_1){
//					if(1==flag_slow1){
//					USART_SendData(USART2,2); //减速指令
//						flag_slow1=0;
//					}
//					if(1==b){
//						USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//					}
//				}else{
//					flag_slow1=1;
//				}
////不停车播种
//				if(0==IO_2){
//					small_servo3=0;
//					if(1==flag_up1){
//						USART_SendData(USART2,1); //加速指令
//						flag_up1=0;
//					}
//					if(1==IO_1){
//						b=1;
//					}
//				}else{
//					flag_up1=1;
//					small_servo3=80;
//				}
//				OLED_ShowNum(40,20,openmv[1],1,8,1);
//				OLED_Refresh();
//				delay_ms(10);
//				pill_Servo(circle_servo1,circle_servo2,small_servo3);
//				
//				//偶数垄
//				
////				if(0==IO_6){
////					if(1==flag_slow2){
////					USART_SendData(USART2,2); //减速指令
////						flag_slow2=0;
////					}
////					if(1==a){
////						USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
////					}
////				}else{
////					flag_slow2=1;
////				}
//////不停车播种			
////				
////				if(0==IO_5){
////					small_servo3=0;
////					if(1==flag_up2){
////						USART_SendData(USART2,1); //加速指令
////						flag_up2=0;
////					}
////					if(1==IO_6){
////						a=1;
////					}
////				}else{
////					flag_up2=1;
////					small_servo3=80;
////				}
////				OLED_ShowNum(40,20,pdf,1,8,1);
////				OLED_Refresh();
////				delay_ms(10);
////				pill_Servo(circle_servo1,circle_servo2,small_servo3);
//			}	
//	
		//24.4.26测试
			
			
while(1){
	u8 key = KEY_Scan(0);
	if(2==key){
		circle_servo1+=22.5;
	}else if(3==key){
		circle_servo2+=23;
	}
	else if(4==key){
		circle_servo1=4.0;
		circle_servo2=9.0;
	while(1){
		if(1==jioulong(re_flag)){
			while(1==jioulong(re_flag)){
//				if(0==IO_1&&1==b){
//					USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//				}
				if(0==IO_1){
//					if(1==flag_slow1){
//					USART_SendData(USART2,2); //减速指令
//						flag_slow1=0; 
//					}
					if(1==b){
						USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
					}
				}
//				else{
//					flag_slow1=1;
//				}
//不停车播种
				if(0==IO_2){
					small_servo3=0;
					//pill_Servo(circle_servo1,circle_servo2,small_servo3);
					//delay_ms(300);
//					if(1==flag_up1){
//						USART_SendData(USART2,1); //加速指令
//						flag_up1=0;
//					}
					if(1==IO_1){
						b=1;
					}
				}else{
					//flag_up1=1;
					small_servo3=80;
				}
				
//停车播种
				/*
			if(0==IO_1){
				if(1==flag_slow1){
					USART_SendData(USART2,2); //减速指令
				}
				flag_slow1=0;
			}
			if(0==IO_2 && 0==IO_5){
				if(1==flag_stop1){
					USART_SendData(USART2,1); //停车指令
				}
				flag_stop1=0;
				small_servo3=110;
			}
			else{
				flag_stop1=1;
				flag_slow1=1;
				small_servo3=40;
			}
			*/
				pill_Servo(circle_servo1,circle_servo2,small_servo3);
			}
		}else if(2==jioulong(re_flag)){
			while(2==jioulong(re_flag)){
//				if(0==IO_6&&1==a){
//					USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//				}
				if(0==IO_6){
//					if(1==flag_slow2){
//					USART_SendData(USART2,2); //减速指令
//						flag_slow2=0;
//					}
					if(1==a){
						USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
					}
				}
//				else{
//					flag_slow2=1;
//				}
//不停车播种			
				
				if(0==IO_5){
					small_servo3=0;
					//pill_Servo(circle_servo1,circle_servo2,small_servo3);
					//delay_ms(300);
//					if(1==flag_up2){
//						USART_SendData(USART2,1); //加速指令
//						flag_up2=0;
//					}
					if(1==IO_6){
						a=1;
					}
				}
				else{
					//flag_up2=1;
					small_servo3=80;
				}
				
//停车播种
				/*
			if(0==IO_1){
				if(1==flag_slow2){
					USART_SendData(USART2,2); //减速指令
				}
				flag_slow2=0;
			}
			if(0==IO_2 && 0==IO_5){
				if(1==flag_stop2){
					USART_SendData(USART2,1); //停车指令
				}
				flag_stop2=0;
				small_servo3=110;
			}
			else{
				flag_stop2=1;
				flag_slow2=1;
				small_servo3=40;
			}		
			*/
				pill_Servo(circle_servo1,circle_servo2,small_servo3);
			}
		}else{
		}
	}
}
	pill_Servo(circle_servo1,circle_servo2,small_servo3);
}

	return 0;
}		
