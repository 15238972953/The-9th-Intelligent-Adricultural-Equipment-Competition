#include "timer.h"
#include "ult.h"
#include "key.h"
#include "oled.h"
#include "encode.h"
#include "UART1.h"
#include "variable.h"
#include "command.h"
extern volatile int stop_flag;
extern volatile int flag_kualong;
extern 	int pdf;
int Encoder_Timer3_sum=0;//��ʱ��3����������
int Encoder_Timer4_sum=0;//��ʱ��4����������
int PID_value;
int empt=0; //�жϱ�־λ���Ƿ�ͣ��ʩҩ�ı�־

void TIM6_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	TIM_Cmd(TIM6,DISABLE); //��ʼ��ʱ�Ȳ�������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM7_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///ʹ��TIM7ʱ��
	
    TIM_TimeBaseInitStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	TIM_Cmd(TIM7,ENABLE); //��ʼ��ʱ�Ȳ�������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM7_IRQHandler(void)                          //�õ��������صĺ�λ��
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)  
    //����ж�
	{	
			sum_Read_Encoder_cnt();
			Encoder_Timer3_sum=abs(encode.TIM3_sum_encode);
			Encoder_Timer4_sum=abs(encode.TIM4_sum_encode);
//			PID_value=blance_pid_hor(25,0,10,YAW);
            
			Sonic_Get_Distance();
		//printf("Angle:%f \r\n",(float)stcAngle.Angle[2]/32768*180);
			//printf("%f",YAW);
			OLED_ShowIntNum(45,0,YAW*100,5,8,1);
        //USART_SendData(USART1,1);
            //printf("%f\n",YAW);
            if(flag%2==0||flag==11)
                PID_value=blance_pid_hor(30,0.1,10,YAW);//ȫ��pid��100,0.1,1//����pid
            else
                PID_value=blance_pid_hor(10,1,10,YAW);//25,1,10//35,1,15//����pid
            //OLED_ShowIntNum(50,50,PID_value,5,8,1);
			//OLED_ShowIntNum(50,0,YAW*100,5,8,1);
//				OLED_ShowIntNum(0,0,Encoder_Timer3_sum ,5,8,1);
//			OLED_ShowIntNum(0,10,Encoder_Timer4_sum ,5,8,1);
			//Ultra_Test1();
			OLED_ShowNum(40,10,flag ,2,8,1); //��ʾflag��־��
			OLED_ShowNum(0,0,stop_flag ,2,8,1);
			//OLED_ShowNum(40,40,pdf,1,8,1);
			OLED_Refresh();
		
			if(flag==2 ||flag==6 )
			{
				usart3_fangliao();
				if(flag_openmv==1 && IO_3==0)
					{
						empt=1; 
					}
			}
			
			if(flag==4 ||flag==8 ||flag==11 )
			{
				usart4_fangliao();
				if(flag_openmv==1 && IO_3==0)
					{
						empt=1;
					}
			}
	
	}


	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);        //����жϱ�־λ
}

void TIM13_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  ///ʹ��TIM7ʱ��
	
  TIM_TimeBaseInitStructure.TIM_Period = arr;   //�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
	TIM_Cmd(TIM13,ENABLE); //��ʼ��ʱ�Ȳ�������ʱ��7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIM14_init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///ʹ��TIM7ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr;   //�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//��ʼ��TIM7
//	
//	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //����ʱ��6�����ж�
//	TIM_Cmd(TIM14,ENABLE); //��ʼ��ʱ�Ȳ�������ʱ��7
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //��ʱ��6�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

void TIM8_UP_TIM13_IRQHandler() //��ʱ��13�ж� ��ʾ
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET)      //����ж�
		{  

		Ultrasonic_Trig_Start();
		//tly();
			
	}
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
}

//ͨ�ö�ʱ��3�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��3!
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///ʹ��TIM3ʱ��
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
//	TIM_Cmd(TIM3,DISABLE); //ʹ�ܶ�ʱ��3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //��ʱ��3�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //�����ȼ�3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}

////��ʱ��3�жϷ�����
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //����ж�
//	{
//		flag_kualong++;
//		
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //����жϱ�־λ
//}




void TIM6_DAC_IRQHandler(void)//��ʾ ���� 
{
		if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET){
			flag_kualong++;
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
	
