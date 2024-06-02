#include "switch.h"
#include "servo.h"
extern volatile int a;
extern volatile int b;
void IO_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;
//	NVIC_InitTypeDef   NVIC_InitStructure;
//	EXTI_InitTypeDef   EXTI_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_8| GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
//	
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource4);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource5);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource8);
//	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource9);

//	EXTI_InitStructure.EXTI_Line = EXTI_Line4 |EXTI_Line5|EXTI_Line8|EXTI_Line9;//LINE0
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //变化触发 
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
//		if(IO_1==0&&b==1){
//			USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
//		}else{
//			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//		}
//	 EXTI_ClearITPendingBit(EXTI_Line4); //清除LINE0上的中断标志位 
//}

//void EXTI9_5IRQHandler(void)
//{
//	if(EXTI_GetITStatus(EXTI_Line5)){
//		if(IO_2==0){
//			small_servo3=110;
//			b=1;
//		}else{
//			small_servo3=40;
//		}  
//		EXTI_ClearITPendingBit(EXTI_Line5); //清除LINE0上的中断标志位 
//	}else	if(EXTI_GetITStatus(EXTI_Line8)){
//		if(IO_5==0){
//			small_servo3=110;
//			a=1;
//		}else{
//			small_servo3=40;
//		}
//		EXTI_ClearITPendingBit(EXTI_Line8); //清除LINE0上的中断标志位 
//	}
//	else if(EXTI_GetITStatus(EXTI_Line9)){
//		if(IO_6==0&&a==1){
//			USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);
//		}else{
//			USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
//		} 
//	 EXTI_ClearITPendingBit(EXTI_Line9); //清除LINE0上的中断标志位 
//	}
//}

//判断是奇数垄还是偶数垄
int jioulong(int re_flag){
	if(6==re_flag || 10==re_flag){
		return 1;  //偶数垄
	}else if(4==re_flag || 8==re_flag || 11==re_flag){
		return 2;  //奇数垄
	}else{
		return 0;
	}
}
