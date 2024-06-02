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
u8 USART3_RX_BUF[USART3_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
u16 USART3_RX_STA=0;       //����״̬���	
volatile int openmv[50]={0};   //����¢
volatile int b=1;
volatile int count_U3=0;
int y=0;
void uart3_init(unsigned long baudrate)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE); //ʹ��GPIOAʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);//ʹ��USART1ʱ��
 
	//����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_USART3); //GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_USART3); //GPIOA10����ΪUSART1
	
	//USART1�˿�����
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//�ٶ�50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����
	GPIO_Init(GPIOD,&GPIO_InitStructure); //��ʼ��PA9��PA10

   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = baudrate;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
  USART_Init(USART3, &USART_InitStructure); //��ʼ������1
	
  USART_Cmd(USART3, ENABLE);  //ʹ�ܴ���1 
	
	//USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//��������ж�

	//Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}

//���ʶ������������
//void USART3_IRQHandler(void)
//{
//    u8 res;
//    if(USART_GetITStatus(USART3,USART_IT_RXNE))
//			{
//        openmv[count_U3++]=USART_ReceiveData(USART3);
//				if(0==IO_1){
//					b=0;    //b==0��ʾ�Ѿ��յ����ݣ����ڽ���
//					USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);
//				}
//			}
//		USART_ClearITPendingBit(USART3,USART_IT_ORE);
//}

//��������⵽ʶ��һ�η���
void USART3_IRQHandler(void)
{
    u8 res;
    if(USART_GetITStatus(USART3,USART_IT_RXNE))
			{
        openmv[count_U3++]=USART_ReceiveData(USART3);
				if(count_U3>=2){
				b=0;    //b==0��ʾ�Ѿ��յ����ݣ����ڽ���
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



