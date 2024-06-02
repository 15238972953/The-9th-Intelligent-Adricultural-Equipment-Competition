//#include <stdio.h>
//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
//#include "stm32f4xx_usart.h"
//#include "stm32f4xx_rcc.h"
//#include "misc.h"
//#include "UART3.h"
//#include "led.h" 
//#include "sys.h"
//#include "key.h"
//#include "guangdian.h"
#include "variable.h"
u8 USART3_RX_BUF[USART3_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u16 USART3_RX_STA=0;       //接收状态标记	
volatile int openmv[50]={0};   //奇数垄
volatile int b=1;
volatile int count_U3=0;
int y=0;
void uart3_init(unsigned long baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//使能USART1时钟
 
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA10复用为USART1
	
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD,&GPIO_InitStructure); //初始化PA9，PA10

   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = baudrate;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART3, ENABLE);  //使能串口1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//开启相关中断

	//Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

//多次识别求众数方案
//void USART3_IRQHandler(void)
//{
//    u8 res;
//    if(USART_GetITStatus(USART3,USART_IT_RXNE))
//			{
//        openmv[count_U3++]=USART_ReceiveData(USART3);
//				if(0==IO_1){
//					b=0;    //b==0表示已经收到数据，不在接收
//					USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//				}
//			}
//		USART_ClearITPendingBit(USART3,USART_IT_ORE);
//}

//传感器检测到识别一次方案
void USART3_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART3,USART_IT_RXNE))
			{
        openmv[count_U3++]=USART_ReceiveData(USART3);
				if(count_U3>=2){
				b=0;    //b==0表示已经收到数据，不在接收
				count_U3=0;
				USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
				switch(openmv[1]){
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
//				openmv[1]=0;
			}
			}
		USART_ClearITPendingBit(USART3,USART_IT_ORE);
}
void usart3_fangliao(void)
{
	if(IO_1==0)
	{
		y=1;
	}
	if(IO_1==1&&y==1)
	{
		if(openmv[0]==0x02)
		{
			flag_openmv=1;
			openmv[0]=0x01;
		}
		else
		{
		  flag_openmv=0;
		}
		y=0;
	}
}



