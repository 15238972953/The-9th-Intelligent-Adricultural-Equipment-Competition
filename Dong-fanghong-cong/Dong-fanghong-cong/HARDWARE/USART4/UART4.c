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
u8 UART4_RX_BUF[UART4_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
volatile int a=1;//ż��¢
u16 UART4_RX_STA=0;       //����״̬���	
volatile int xxx=0;
//int my_n4=50;  //����ͷ����Ĵ�С
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
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);//ʹ��USART1ʱ��
 
//	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_UART4); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_UART4); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOC,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = baudrate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(UART4, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(UART4, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);//�ȹر�����ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}
//���ʶ������������
//void UART4_IRQHandler(void)
//{
//    if(USART_GetITStatus(UART4,USART_IT_RXNE))
//			{
//        openmv1[count_U4++]=USART_ReceiveData(UART4);
//				if(0==IO_6){
//					a=0;    //b==0��ʾ�Ѿ��յ����ݣ����ڽ���
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
					a=0;    //a==0��ʾ�Ѿ��յ����ݣ����ڽ���
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




