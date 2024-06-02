#include "switch.h"
void IO_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5| GPIO_Pin_6|GPIO_Pin_7 | GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);

//	EXTI_InitStructure.EXTI_Line = EXTI_Line4 |EXTI_Line5|EXTI_Line6|EXTI_Line7|EXTI_Line8|EXTI_Line9;//LINE0
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发 
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);//配置
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;//外部中断4
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure);//配置
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//外部中断9-5
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
//  NVIC_Init(&NVIC_InitStructure);//配置
}

//void EXTI4_IRQHandler(void)
//{
//	delay_ms(10);	//消抖
////	if(IO_4==1)	 
////	{
////		together_servo_steer(45,90,10);
////	}		 
//	 EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
//}

//void EXTI9_5IRQHandler(void)
//{
//	static int i=1;
//	delay_ms(10);	//消抖
//	
//	if(IO_2==1&&i==1)	 //第一次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	}
//	if(IO_3==1&&i==2)	 //第二次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	} 
//		if(IO_1==1&&i==3)	 //第三次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	} 
//		if(IO_3==1&&i==4)	 //第四次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	}
//			if(IO_1==1&&i==5)	 //第五次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	}
//			if(IO_3==1&&i==6)	 //第六次光电开关检测到
//	{
//		together_servo_steer(135,135,10); 
//		i++;
//	}
//			if(IO_1==1&&i==7)	 //第七次光电开关检测到
//	{
//		together_servo_steer(45,135,10); 
//		i++;
//	}
//	 EXTI_ClearITPendingBit(EXTI_Line0); //清除LINE0上的中断标志位 
//}

