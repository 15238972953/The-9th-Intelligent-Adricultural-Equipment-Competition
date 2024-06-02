//#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_usart.h"
//#include "stm32f4xx_rcc.h"
//#include "misc.h"
//#include "UART4.h"
//#include "led.h" 
//#include "sys.h"
//#include "guangdian.h"
//#include "key.h" 
#include "variable.h"
u8 UART4_RX_BUF[UART4_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
volatile int a=1;//偶数垄
u16 UART4_RX_STA=0;       //接收状态标记	
volatile int xxx=0;
//int my_n4=50;  //摄像头数组的大小
volatile int openmv1[50]={0};
volatile int count_U4=0;
int flag_openmv=0;
extern int count;
int x=0;
int pdf=0;
void uart4_init(unsigned long baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//使能USART1时钟
 
//	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = baudrate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(UART4, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(UART4, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//先关闭相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}
//多次识别求众数方案
//void UART4_IRQHandler(void)
//{
//    if(USART_GetITStatus(UART4,USART_IT_RXNE))
//			{
//        openmv1[count_U4++]=USART_ReceiveData(UART4);
//				if(0==IO_6){
//					a=0;    //b==0表示已经收到数据，不在接收
//					USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
//				}
//			}
//		USART_ClearITPendingBit(UART4,USART_IT_ORE);
//}


void UART4_IRQHandler(void)
{
    if(USART_GetITStatus(UART4,USART_IT_RXNE))
			{
				//xxx=USART_ReceiveData(UART4);
        openmv1[count_U4++]=USART_ReceiveData(UART4);
				if(count_U4>=2){
					a=0;    //a==0表示已经收到数据，不在接收
					count_U4=0;
					USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
					switch(openmv1[1]){
				case 1: 
					circle_servo1+=22.5;
					break;
				case 2:
					circle_servo2+=23;
					break;
				case 3:
					circle_servo1+=22.5;
					circle_servo2+=23;
					break;
				default:
					break;
			}
					//openmv1[1]=0;
			}
			}
	USART_ClearITPendingBit(UART4,USART_IT_ORE);
}


void usart4_fangliao(void)
{
	if(IO_5==0)
	{
		
		x=1;
	}
	if(IO_5==1&&x==1)
	{
//		USART_Cmd(UART4,ENABLE);
		if(openmv1[0]==0x02)
		{
			flag_openmv=1;
			openmv1[0]=0x01;
		}
		else
		{
		  flag_openmv=0;
		}
		x=0;
	}
}




