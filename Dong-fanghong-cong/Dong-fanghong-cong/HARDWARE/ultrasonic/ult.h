/*
********************************************************************************************************************
*Filename              :UltrasonicWave.h
*Programmer(s)    :Lab 416
*Description          :UltrasonicWave function
********************************************************************************************************************
*/

#ifndef __sonic_H
#define	__sonic_H

#include "sys.h"









//#define TRIGALL do{TRIGF=1;TRIGL_1=1;for(i=0;i<750;i++);TRIGF=0;TRIGL_1=0;for(i=0;i<1500;i++);TRIGB=1;TRIGR_1=1;for(i=0;i<750;i++);TRIGB=0;TRIGR_1=0;}while(0);
////#define TRIG_A do{TIM_ITConfig(TIM5,TIM_IT_CC4,ENABLE);TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);TIM_ITConfig(TIM2,TIM_IT_CC3,ENABLE);\
////	                TIM_ITConfig(TIM5,TIM_IT_CC1,DISABLE);TIM_ITConfig(TIM5,TIM_IT_CC3,DISABLE);TIM_ITConfig(TIM2,TIM_IT_CC4,DISABLE);\
////                  TRIGF=1;TRIGL_1=1;TRIGR_2=1;for(i=0;i<750;i++);TRIGF=0;TRIGL_1=0;TRIGR_2=0;}while(0);//原
////#define TRIG_B do{TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);TIM_ITConfig(TIM5,TIM_IT_CC3,ENABLE);TIM_ITConfig(TIM2,TIM_IT_CC4,ENABLE);\
////	                TIM_ITConfig(TIM5,TIM_IT_CC4,DISABLE);TIM_ITConfig(TIM5,TIM_IT_CC2,DISABLE);TIM_ITConfig(TIM2,TIM_IT_CC3,DISABLE);\
////                  TRIGB=1;TRIGR_1=1;TRIGL_2=1;for(i=0;i<750;i++);TRIGB=0;TRIGR_1=0;TRIGL_2=0;}while(0);
////	
//inline void TRIG_A (void){int i=0;do{TIM_ITConfig(TIM5,TIM_IT_CC4,ENABLE);TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);TIM_ITConfig(TIM2,TIM_IT_CC3,ENABLE);
//	                TIM_ITConfig(TIM5,TIM_IT_CC1,DISABLE);TIM_ITConfig(TIM5,TIM_IT_CC3,DISABLE);TIM_ITConfig(TIM2,TIM_IT_CC4,DISABLE);
//                  TRIGF=1;TRIGL_1=1;TRIGR_2=1;for(i=0;i<750;i++);TRIGF=0;TRIGL_1=0;TRIGR_2=0;}while(0);}
//inline void  TRIG_B(void) {int i=0;do{TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);TIM_ITConfig(TIM5,TIM_IT_CC3,ENABLE);TIM_ITConfig(TIM2,TIM_IT_CC4,ENABLE);\
//	                TIM_ITConfig(TIM5,TIM_IT_CC4,DISABLE);TIM_ITConfig(TIM5,TIM_IT_CC2,DISABLE);TIM_ITConfig(TIM2,TIM_IT_CC3,DISABLE);\
//                  TRIGB=1;TRIGR_1=1;TRIGL_2=1;for(i=0;i<750;i++);TRIGB=0;TRIGR_1=0;TRIGL_2=0;}while(0);}
//		


#define TIME_OUT 10000        //超声波捕获溢出时间阀值
#define Trig1_on()  (GPIO_SetBits(GPIOE,GPIO_Pin_15))             //开启Front_Trig  F_Right_Trig_on()
#define Trig1_off() (GPIO_ResetBits(GPIOE,GPIO_Pin_15))           //关闭Front_Trig  F_Right_Trig_off()

#define Trig2_on()  (GPIO_SetBits(GPIOE,GPIO_Pin_12))             //开启Front_Trig
#define Trig2_off() (GPIO_ResetBits(GPIOE,GPIO_Pin_12))  

#define Trig3_on()  (GPIO_SetBits(GPIOE,GPIO_Pin_10))             //开启Front_Trig
#define Trig3_off() (GPIO_ResetBits(GPIOE,GPIO_Pin_10)) 

#define Trig4_on()  (GPIO_SetBits(GPIOD,GPIO_Pin_1))             //开启Front_Trig
#define Trig4_off() (GPIO_ResetBits(GPIOD,GPIO_Pin_1)) 
				  
#define Trig5_on()  (GPIO_SetBits(GPIOE,GPIO_Pin_8))             //开启Front_Trig
#define Trig5_off() (GPIO_ResetBits(GPIOE,GPIO_Pin_8)) 
	
#define Trig6_on()  (GPIO_SetBits(GPIOD,GPIO_Pin_3))             //开启Front_Trig
#define Trig6_off() (GPIO_ResetBits(GPIOD,GPIO_Pin_3)) 

#define Trig7_on()  (GPIO_SetBits(GPIOC,GPIO_Pin_12))             //开启Front_Trig
#define Trig7_off() (GPIO_ResetBits(GPIOC,GPIO_Pin_12)) 

#define Trig8_on()  (GPIO_SetBits(GPIOC,GPIO_Pin_3))             //开启Front_Trig
#define Trig8_off() (GPIO_ResetBits(GPIOC,GPIO_Pin_3)) 



typedef struct 
{
	short int A_1;
	short int A_2;
	short int A_3;
	short int A_4;
	short int A_5;
  short int A_6;
	short int A_7;
	short int A_8;
}SonicDis;


/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
******************************************************************************************************************
*/

void Ultra_Init(void);//超声波IO口初始化函数
void Ultrasonic_Trig_Start(void);
void Ultrasonic_Trig_end(void);
void Sonic_Get_Distance(void);//测距函数
void Ultra_Test1(void);
void Ultra_Test2(void);
void TIM5_Sonic_Cap_Init(u32 arr,u16 psc);
void TIM1_Sonic_Cap_Init(u32 arr,u16 psc);
//void TIM5_IRQHandler(void);
int Sonic_Length(float l1,float l2);
int Sonic_Length_l(float l1,float l2);
void TRIG_A (void);
void  TRIG_B(void);
void Mea_dis_B(void);
float  center_L(int a, int b);
#endif 



