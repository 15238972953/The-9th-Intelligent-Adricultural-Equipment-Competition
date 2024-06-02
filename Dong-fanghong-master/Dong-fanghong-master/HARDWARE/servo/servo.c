#include "servo.h"
#include "delay.h"

volatile int circle_servo1=12;
volatile int circle_servo2=7;
volatile int small_servo3=12;

/*180�������ź�����Ϊ20ms�����ߵ�ƽʱ��Ϊ0.5-2.5msʱ����Ӧ��0��-180�㣻
0.5ms--CNTΪ500   2.5ms--CNTΪ2500		1��==500+1*11.1111CNT	  N��==500+N*11.1111 CNT
�㣨������	 			0				45			90				135				180
HIGH��ʱ�䣩		0.5			1.0			1.5				2.0				2.5
CNT							500			1000		1500			2000			2500
*/
void TIM2_Init(void)//�ؽڶ����ʼ��
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE );
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE );
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3, GPIO_AF_TIM2);	
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_TIM2);	
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period=20000;
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//2*10^4 / 1*10^6==2*10^-2s==20ms
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low ;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM2,&TIM_OCInitStruct);
	TIM_OC2Init(TIM2,&TIM_OCInitStruct);
	TIM_OC3Init(TIM2,&TIM_OCInitStruct);
	TIM_OC4Init(TIM2,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM2,TIM_OCPreload_Enable );
	TIM_OC2PreloadConfig(TIM2,TIM_OCPreload_Enable );
	TIM_OC3PreloadConfig(TIM2,TIM_OCPreload_Enable );
	TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable );
					
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	
}

void Time8_Servo_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOC,ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM8 ,ENABLE );
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;      //���츴�����
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;        //����
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,  GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,  GPIO_AF_TIM8);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource13, GPIO_AF_TIM1);
//	GPIO_PinAFConfig(GPIOE,GPIO_PinSource14, GPIO_AF_TIM1);

	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up ;
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=168-1;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_Pulse = 0;
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable ;
	TIM_OCInitStruct.TIM_OutputNState=TIM_OutputNState_Disable ;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low;
	TIM_OCInitStruct.TIM_OCNPolarity=TIM_OCNPolarity_Low;
	TIM_OCInitStruct.TIM_OCIdleState=TIM_OCIdleState_Set;
	TIM_OCInitStruct.TIM_OCNIdleState=TIM_OCNIdleState_Reset;
	TIM_OC1Init(TIM8,&TIM_OCInitStruct);
	TIM_OC2Init(TIM8,&TIM_OCInitStruct);
//	TIM_OC3Init(TIM1,&TIM_OCInitStruct);
//	TIM_OC4Init(TIM1,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable );
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable );
//	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable );
//	TIM_OC4PreloadConfig(TIM1,TIM_OCPreload_Enable );
	TIM_ARRPreloadConfig(TIM8, ENABLE);
	
	TIM_CtrlPWMOutputs(TIM8, ENABLE); 
	TIM_Cmd(TIM8,ENABLE);
}

void TIM14_Init(void)//С�����ʼ��
{
	GPIO_InitTypeDef GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_OCInitTypeDef TIM_OCInitStruct;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE );
	
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;	
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource7,GPIO_AF_TIM14);
	
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_Period=20000-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=84-1;//2*10^4 / 1*10^6==2*10^-2s==20ms
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStruct);
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM2;
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_Low ;
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;
	TIM_OC1Init(TIM14,&TIM_OCInitStruct);
	
	TIM_OC1PreloadConfig(TIM14,TIM_OCPreload_Enable );
					
	TIM_ARRPreloadConfig(TIM14, ENABLE);
	
	TIM_Cmd(TIM14,ENABLE);
	
}

/*
����	conversion_CNT(int angle)
function �����Ƕ�ת��Ϊ��ʱ�� CNT
*/
int conversion_CNT(int angle)
{
	int cnt;
	cnt=(int)500+angle*11.111111;
	return cnt;
}

void Set_angle(int a,int b, int c, int d )
{
	int angle_a,angle_b,angle_c,angle_d;
		angle_a = conversion_CNT(a);
		angle_b = conversion_CNT(b);
		angle_c = conversion_CNT(c);
		angle_d = conversion_CNT(d);
		TIM_SetCompare1(TIM2,angle_a );
		TIM_SetCompare2(TIM2,angle_b );
		TIM_SetCompare3(TIM2,angle_c );
		TIM_SetCompare4(TIM2,angle_d );

}

void single_set(int N, int a)
{
	int angle_a;
		angle_a = conversion_CNT(a);
		switch(N)
		{
			case 1:TIM_SetCompare1(TIM2,angle_a );break;
			case 2:TIM_SetCompare2(TIM2,angle_a );break;
			case 3:TIM_SetCompare3(TIM2,angle_a );break;
			case 4:TIM_SetCompare4(TIM2,angle_a );break;
		}
}

void pill_Servo(int a,int b,int c)
{
	int angle_a;
	int angle_b;
	int angle_c;
		angle_a = conversion_CNT(a);
		angle_b = conversion_CNT(b);
		angle_c = conversion_CNT(c);
		TIM_SetCompare1(TIM8,angle_a);
		TIM_SetCompare2(TIM8,angle_b);
		TIM_SetCompare1(TIM14,angle_c);
}
/**
  * @brief  ѡ���N����� Previous��һ��ʼ�Ƕ�ֵ �ﵽ�Ƕ�ֵ ����һ�������ת
  *           
  * @note     
  *           
  * @param   
  *         
  * @retval 
  */
void single_servo_steer(int Previous,int current, int N, int time_us)
{
	int temp;
	int Previous_CNT=conversion_CNT(Previous);
	int	current_CNT =conversion_CNT(current);
	switch(N)
	{
		case 1:
		{
			if(Previous_CNT<current_CNT )
				{
					for(temp=Previous_CNT;temp<=current_CNT;temp++)
					{
						TIM_SetCompare1(TIM2,temp);
						delay_ms(time_us);
					}
				}
			else
				{
					for(temp=Previous_CNT;temp>=current_CNT;temp--)
					{
						TIM_SetCompare1(TIM2,temp);
						delay_ms(time_us);
					}
				}
		}
			break;
		
		case 2:
				{
			if(Previous_CNT<current_CNT )
				{
					for(temp=Previous_CNT;temp<=current_CNT;temp++)
					{
						TIM_SetCompare2(TIM2,temp);
						delay_ms(time_us);
					}
				}
			else
				{
					for(temp=Previous_CNT;temp>=current_CNT;temp--)
					{
						TIM_SetCompare2(TIM2,temp);
						delay_ms(time_us);
					}
				}
		}
			break;
		
		case 3:
				{
			if(Previous_CNT<current_CNT )
				{
					for(temp=Previous_CNT;temp<=current_CNT;temp++)
					{
						TIM_SetCompare3(TIM2,temp);
						delay_ms(time_us);
					}
				}
			else
				{
					for(temp=Previous_CNT;temp>=current_CNT;temp--)
					{
						TIM_SetCompare3(TIM2,temp);
						delay_ms(time_us);
					}
				}
		}
			break;
		
		case 4:
				{
			if(Previous_CNT<current_CNT )
				{
					for(temp=Previous_CNT;temp<=current_CNT;temp++)
					{
						TIM_SetCompare4(TIM2,temp);
						delay_ms(time_us);
					}
				}
			else
				{
					for(temp=Previous_CNT;temp>=current_CNT;temp--)
					{
						TIM_SetCompare4(TIM2,temp);
						delay_ms(time_us);
					}
				}
		}
			break;
			
	}
}

void together_servo_steer(int Previous,int current, int time_us)//�ĸ������תͬһ�Ƕ�
{
	int temp;
	int Previous_CNT=conversion_CNT(Previous);
	int	current_CNT =conversion_CNT(current);
			if(Previous_CNT<current_CNT )
				{
					for(temp=Previous_CNT;temp<=current_CNT;temp++)
					{
						TIM_SetCompare1(TIM2,temp);
						TIM_SetCompare2(TIM2,temp);
						TIM_SetCompare3(TIM2,temp);
						TIM_SetCompare4(TIM2,temp);
						delay_ms(time_us);
					}
				}
			else
				{
					for(temp=Previous_CNT;temp>=current_CNT;temp--)
					{
						TIM_SetCompare1(TIM2,temp);
						TIM_SetCompare2(TIM2,temp);
						TIM_SetCompare3(TIM2,temp);
						TIM_SetCompare4(TIM2,temp);
					}
				}

}

void set_balance_angle(float a,float b,float c,float d)//1000Ϊ��ʼ�Ƕ�45��
{
	int angle_a,angle_b,angle_c,angle_d;
		angle_a=a*180/pi;
		angle_b=b*180/pi;
		angle_c=c*180/pi;
		angle_d=d*180/pi;
	if(angle_a>0) 
		TIM_SetCompare1(TIM2,conversion_CNT(40)+conversion_CNT(angle_a));
	else
		TIM_SetCompare1(TIM2,conversion_CNT(40)-conversion_CNT(angle_a));
	
	if(angle_b>0) 
		TIM_SetCompare2(TIM2,conversion_CNT(143)+conversion_CNT(angle_b));
	else
		TIM_SetCompare2(TIM2,conversion_CNT(143)-conversion_CNT(angle_b));
	
		if(angle_c>0) 
		TIM_SetCompare3(TIM2,conversion_CNT(133)+conversion_CNT(angle_c));
	else
		TIM_SetCompare3(TIM2,conversion_CNT(133)-conversion_CNT(angle_c));
	
		if(angle_d>0) 
		TIM_SetCompare4(TIM2,conversion_CNT(40)+conversion_CNT(angle_d));
	else
		TIM_SetCompare4(TIM2,conversion_CNT(40)-conversion_CNT(angle_d));

}
