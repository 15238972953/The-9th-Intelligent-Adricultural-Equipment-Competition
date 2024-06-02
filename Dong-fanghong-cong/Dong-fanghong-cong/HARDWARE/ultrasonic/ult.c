#include "ult.h"
#include "filter.h"
#include "delay.h"
#include "oled.h"
#include "variable.h"

/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
******************************************************************************************************************
*/ 
			//��ʱ��5
u8   TIM5CH1_CAPTURE_STA=0;	//���벶��״̬		    PA0				
u32	 TIM5CH1_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM5CH2_CAPTURE_STA=0;	//���벶��״̬		    PA1				
u32	 TIM5CH2_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM5CH3_CAPTURE_STA=0;	//���벶��״̬		    PA2				
u32	 TIM5CH3_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM5CH4_CAPTURE_STA=0;	//���벶��״̬		    PA3			
u32	 TIM5CH4_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
    
		 //��ʱ��1
u8   TIM1CH1_CAPTURE_STA=0;	//���벶��״̬		    PB10				
u32	 TIM1CH1_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM1CH2_CAPTURE_STA=0;	//���벶��״̬		    PB11				
u32	 TIM1CH2_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM1CH3_CAPTURE_STA=0;	//���벶��״̬		    PB10				
u32	 TIM1CH3_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)
u8   TIM1CH4_CAPTURE_STA=0;	//���벶��״̬		    PB11				
u32  TIM1CH4_CAPTURE_VAL=0;	//���벶��ֵ(TIM2/TIM5��32λ)

long long temp_1=0;//ǰ                      T5C3        4
long long temp_2=0;//��                   //T1C2         1 
long long temp_3=0;                     //T5C2         5
long long temp_4=0;                     //T5C1         6
long long temp_5=0;                     //T5C4         3
long long temp_6=0;                     //T1C1         2
long long temp_7=0;                     //T5C4         3
long long temp_8=0; 

long long TEMPA_1=0;
long long TEMPA_2=0;
long long TEMPA_3=0;
long long TEMPA_4=0;
long long TEMPA_5=0;
long long TEMPA_6=0;
long long TEMPA_7=0;
long long TEMPA_8=0;

SonicDis Sonic_Dis={0,0,0,0,0,0,0,0};


Filter_Struct SonicA1_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA2_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA3_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA4_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA5_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA6_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA7_Filter = {0,0,0,0,{0}};
Filter_Struct SonicA8_Filter = {0,0,0,0,{0}};

/*
********************************************************************************************************************
*                  void  Uitra_Init(void)
*
*Description    :��ʼ��������
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Ultra_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOE,GPIO_Pin_15|GPIO_Pin_12|GPIO_Pin_10|GPIO_Pin_8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOD,GPIO_Pin_3|GPIO_Pin_1);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
  GPIO_ResetBits(GPIOC,GPIO_Pin_12|GPIO_Pin_3);
	
	
	
}
/***************************************************************************************

***************************************************************************************/

void Sonic_Get_Distance()
{
		if(TIM5CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_1=TIM5CH1_CAPTURE_STA&0X3F;
//				tempF*=0XFFFFFFFF;        //���ʱ���ܺ�
				temp_1+=TIM5CH1_CAPTURE_VAL; //�õ��ܵĸߵ�ƽʱ��
				TIM5CH1_CAPTURE_STA=0;      //������һ�β���
		}
		 TEMPA_1=(temp_1*340/2)/10000;   //�������뵥λcm
		 Enaverage_Filter(&SonicA1_Filter,TEMPA_1,5, &Sonic_Dis.A_1);


		if(TIM5CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_2=TIM5CH2_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_2+=TIM5CH2_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM5CH2_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_2=(temp_2*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA2_Filter,TEMPA_2,5, &Sonic_Dis.A_2);
			
    if(TIM5CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_3=TIM5CH3_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_3=TIM5CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM5CH3_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_3=(temp_3*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA3_Filter,TEMPA_3,5, &Sonic_Dis.A_3);
			
		if(TIM5CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_4=TIM5CH4_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_4+=TIM5CH4_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM5CH4_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_4=(temp_4*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA4_Filter,TEMPA_4,5, &Sonic_Dis.A_4);
			
		
		
		
		
		
		if(TIM1CH1_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_5=TIM1CH1_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_5+=TIM1CH1_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM1CH1_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_5=(temp_5*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA5_Filter,TEMPA_5,5, &Sonic_Dis.A_5);
		
		if(TIM1CH2_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_6=TIM1CH2_CAPTURE_STA&0X3F; 
				//temp_6*=0XFFFF;         //���ʱ���ܺ�
				temp_6+=TIM1CH2_CAPTURE_VAL;  //�õ��ܵĸߵ�ƽʱ��
				TIM1CH2_CAPTURE_STA=0;       //������һ�β���
		}
		TEMPA_6=(temp_6*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA6_Filter,TEMPA_6,5, &Sonic_Dis.A_6);
	
    if(TIM1CH3_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_7=TIM1CH3_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_7=TIM1CH3_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM1CH3_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_7=(temp_7*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA7_Filter,TEMPA_7,5, &Sonic_Dis.A_7);
			
		if(TIM1CH4_CAPTURE_STA&0X80)//�ɹ�������һ�θߵ�ƽ
		{
				temp_8=TIM1CH4_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //���ʱ���ܺ�
				temp_8+=TIM1CH4_CAPTURE_VAL;//�õ��ܵĸߵ�ƽʱ��
				TIM1CH4_CAPTURE_STA=0;     //������һ�β���
		}
		TEMPA_8=(temp_8*340/2)/10000; //�������뵥λcm
		Enaverage_Filter(&SonicA8_Filter,TEMPA_8,5, &Sonic_Dis.A_8);		

		if(Sonic_Dis.A_1>255)
			Sonic_Dis.A_1=255;
		else
			Sonic_Dis.A_1=Sonic_Dis.A_1;
		
		if(Sonic_Dis.A_2>255)
			Sonic_Dis.A_2=255;
		else
			Sonic_Dis.A_2=Sonic_Dis.A_2;
		
		if(Sonic_Dis.A_3>255)
			Sonic_Dis.A_3=255;
		else
			Sonic_Dis.A_3=Sonic_Dis.A_3;
		
		if(Sonic_Dis.A_4>255)
			Sonic_Dis.A_4=255;
		else
			Sonic_Dis.A_4=Sonic_Dis.A_4;
		
		if(Sonic_Dis.A_5>255)
			Sonic_Dis.A_5=255;
		else
			Sonic_Dis.A_5=Sonic_Dis.A_5;
		
		if(Sonic_Dis.A_6>255)
			Sonic_Dis.A_6=255;
		else
			Sonic_Dis.A_6=Sonic_Dis.A_6;
		
		if(Sonic_Dis.A_7>255)
			Sonic_Dis.A_7=255;
		else
			Sonic_Dis.A_7=Sonic_Dis.A_7;
		
		if(Sonic_Dis.A_8>255)
			Sonic_Dis.A_8=255;
		else
			Sonic_Dis.A_8=Sonic_Dis.A_8;
}




/******************************************************************************************************************
*                                 TIM5_Tim_Init(u32 arr,u16 psc)
*
*Description : ��ʱ��5��ʼ��,����Ϊ1ms
*Arguments   : arr����װ��ֵ   psc��Ԥ��Ƶϵ��
*Returns     : none
*Notes       : ��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.  
               Ft=��ʱ������Ƶ��,��λ:Mhz
*******************************************************************************************************************
*/
void TIM5_Sonic_Cap_Init(u32 arr,u16 psc)//PA0,1,2,3
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM5_ICInitStructure;
	
	/***********************Clock initial****************************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);      
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	/***********************GPIO initial****************************************/	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//ģʽ��Ϊ���ù���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//��Ϊ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//�ٶ���Ϊ100MHz 
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	 /***********************Timer initial****************************************/	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=arr;//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;//CC1S=04,ѡ������� IC4ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//�����ز���
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//ӳ�䵽TI1��
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//���������Ƶ,����Ƶ 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);//��������ж� ,����CC4IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);						// ��������ж� ,����CC2IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);						// ��������ж� ,����CC2IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_CC3,ENABLE);						// ��������ж� ,����CC2IE�����ж�	
	TIM_ITConfig(TIM5,TIM_IT_CC4,ENABLE);						// ��������ж� ,����CC2IE�����ж�	
	
  TIM_Cmd(TIM5,ENABLE );//ʹ�ܶ�ʱ��5
	
  /***********************NVIC initial****************************************/
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;//�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);//����ָ���Ĳ�����ʼ��VIC�Ĵ���	
	
}

void TIM1_Sonic_Cap_Init(u32 arr,u16 psc)
{
GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM1_ICInitStructure;
	
	/***********************Clock initial****************************************/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);      
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	
	/***********************GPIO initial****************************************/	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//????????
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//????
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//????100MHz 
  GPIO_Init(GPIOE,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOE,GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13|GPIO_Pin_14);
	
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_TIM1);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_TIM1);
	
	 /***********************Timer initial****************************************/	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//?????
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//??????
	TIM_TimeBaseStructure.TIM_Period=arr;//??????
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
	
	
	//???TIM5??????
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1;//CC1S=04,????? IC4???TI1?
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//?????
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//???TI1?
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//??????,??? 
  TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_2;//CC1S=04,????? IC4???TI1?
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//?????
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//???TI1?
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//??????,??? 
  TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_3;//CC1S=04,????? IC4???TI1?
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//?????
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//???TI1?
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//??????,??? 
  TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_4;//CC1S=04,????? IC4???TI1?
  TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//?????
  TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//???TI1?
  TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//??????,??? 
  TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ??????? ???
  TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);//?????? ,??CC4IE????	
	TIM_ITConfig(TIM1,TIM_IT_CC1,ENABLE);						// ?????? ,??CC2IE????	
	TIM_ITConfig(TIM1,TIM_IT_CC2,ENABLE);						// ?????? ,??CC2IE????	
	TIM_ITConfig(TIM1,TIM_IT_CC3,ENABLE);						// ?????? ,??CC2IE????	
	TIM_ITConfig(TIM1,TIM_IT_CC4,ENABLE);						// ?????? ,??CC2IE????	
	
  TIM_Cmd(TIM1,ENABLE );//?????5
	
  /***********************NVIC initial****************************************/
  NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//?????1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;//????2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ????
	NVIC_Init(&NVIC_InitStructure);//??????????VIC???	
}


/*
********************************************************************************************************************
*                  void TIM5_IRQHandler(void)
*
*Description    :��ʱ��5�жϷ����� 
*Arguments   : none
*Returns     : TRIGx  ������x
*Notes       : none
*F  T5C3   5��λ
 B  T2C4   1
 R2 T2C3   3
 L2 T5C1   2
 R1 T5C4   4
 L1 T5C2
********************************************************************************************************************
*/
void TIM5_IRQHandler(void)
{
//----------------------------------------------------------------------------------------------------------------- 
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{
			if(TIM5CH1_CAPTURE_STA&0X40)        //�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH1_CAPTURE_STA|=0X80;	      //��ǳɹ�������һ��
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;   //��Ϊ���ֵ
				}else TIM5CH1_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//����ͨ��4���������¼�
		{
			if(TIM5CH1_CAPTURE_STA&0X40)//����һ���½��� 		
			{
				TIM5CH1_CAPTURE_STA|=0X80;	//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
				TIM5CH1_CAPTURE_STA=0;	//���
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM5,DISABLE); 	//�رն�ʱ��2
				TIM_SetCounter(TIM5,0);
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��2
			}
		}
	}
//----------------------------------------------------------------------------------------------------------------- 
	
	if((TIM5CH2_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{
			if(TIM5CH2_CAPTURE_STA&0X40)        //�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH2_CAPTURE_STA|=0X80;	      //��ǳɹ�������һ��
					TIM5CH2_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH2_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//����ͨ��4���������¼�
		{
			if(TIM5CH2_CAPTURE_STA&0X40)//����һ���½��� 		
			{
				TIM5CH2_CAPTURE_STA|=0X80;	//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
				TIM5CH2_CAPTURE_STA=0;	//���
				TIM5CH2_CAPTURE_VAL=0;
				TIM5CH2_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��2
				TIM_SetCounter(TIM5,0);
				TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��2
			}
		}
	}
	
	if((TIM5CH3_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{
			if(TIM5CH3_CAPTURE_STA&0X40)        //�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH3_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH3_CAPTURE_STA|=0X80;	      //��ǳɹ�������һ��
					TIM5CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH3_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)//����ͨ��4���������¼�
		{
			if(TIM5CH3_CAPTURE_STA&0X40)//����һ���½��� 		
			{
				TIM5CH3_CAPTURE_STA|=0X80;	//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH3_CAPTURE_VAL=TIM_GetCapture3(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
				TIM5CH3_CAPTURE_STA=0;	//���
				TIM5CH3_CAPTURE_VAL=0;
				TIM5CH3_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM5,DISABLE ); 	//�رն�ʱ��2
				TIM_SetCounter(TIM5,0);
				TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��2
			}
		}
	}
	
	if((TIM5CH4_CAPTURE_STA&0X80)==0)//��δ�ɹ�����	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//���
		{
			if(TIM5CH4_CAPTURE_STA&0X40)        //�Ѿ����񵽸ߵ�ƽ��
			{
				if((TIM5CH4_CAPTURE_STA&0X3F)==0X3F)//�ߵ�ƽ̫����
				{
					TIM5CH4_CAPTURE_STA|=0X80;	      //��ǳɹ�������һ��
					TIM5CH4_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH4_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)//����ͨ��4���������¼�
		{
			if(TIM5CH4_CAPTURE_STA&0X40)//����һ���½��� 		
			{
				TIM5CH4_CAPTURE_STA|=0X80;	//��ǳɹ�����һ�θߵ�ƽ����
				TIM5CH4_CAPTURE_VAL=TIM_GetCapture4(TIM5);//��ȡ��ǰ�Ĳ���ֵ.
				TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 ����Ϊ�����ز���
			}else //��δ��ʼ,��һ�β���������
			{
				TIM5CH4_CAPTURE_STA=0;	//���
				TIM5CH4_CAPTURE_VAL=0;
				TIM5CH4_CAPTURE_STA|=0X40;		//��ǲ�����������
				TIM_Cmd(TIM5,DISABLE); 	//�رն�ʱ��2
				TIM_SetCounter(TIM5,0);
				TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 ����Ϊ�½��ز���
				TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��2
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //����жϱ�־λ
}


	/********************************��ʱ��1 �����ж�**************************************/
void TIM1_CC_IRQHandler(void)
{ 		    	


	if((TIM1CH1_CAPTURE_STA&0X80)==0)//??????	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//??
		{
			if(TIM1CH1_CAPTURE_STA&0X40)        //?????????
			{
				if((TIM1CH1_CAPTURE_STA&0X3F)==0X3F)//??????
				{
					TIM1CH1_CAPTURE_STA|=0X80;	      //?????????
					TIM1CH1_CAPTURE_VAL=0XFFFF;
				}else TIM1CH1_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM1, TIM_IT_CC1) != RESET)//????4??????
		{
			if(TIM1CH1_CAPTURE_STA&0X40)//???????? 		
			{
				TIM1CH1_CAPTURE_STA|=0X80;	//??????????????
				TIM1CH1_CAPTURE_VAL=TIM_GetCapture1(TIM1);//????????.
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 ????????
			}else //????,????????
			{
				TIM1CH1_CAPTURE_STA=0;	//??
				TIM1CH1_CAPTURE_VAL=0;
				TIM1CH1_CAPTURE_STA|=0X40;		//?????????
				TIM_Cmd(TIM1,DISABLE); 	//?????2
				TIM_SetCounter(TIM1,0);
				TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);	//CC1P=1 ????????
				TIM_Cmd(TIM1,ENABLE); 	//?????2
			}
		}
	}

	
/******************************�����̨�2*****************************************/	

		if((TIM1CH2_CAPTURE_STA&0X80)==0)//?1?��3��1|2???
		{
			if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//��?3?
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//��??-2???��???��???��?			
				{
					if((TIM1CH2_CAPTURE_STA&0X3F)==0X3F)//??��???��?3�訢?
					{
						TIM1CH2_CAPTURE_STA|=0X80;		//����??3��1|2???��?��?��?
						TIM1CH2_CAPTURE_VAL=0XFFFF;
					}else TIM1CH2_CAPTURE_STA++;
				}	 
			}
			if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)//2???1���騦��2???��??t
			{
				if(TIM1CH2_CAPTURE_STA&0X40)		//2???��?��??????��?? 		
				{
					TIM1CH2_CAPTURE_STA|=0X80;		//����??3��1|2???��?��?��???��??????��
					TIM1CH2_CAPTURE_VAL=TIM_GetCapture2(TIM1);//??��?�̡�?���?2????��.
					TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC2P=0 ����???a��?��y??2???
				}else  								//?1?��?a��?,�̨���?��?2???��?��y??
				{
					TIM1CH2_CAPTURE_STA=0;			//????
					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_STA|=0X40;		//����??2???��?��?��?��y??
					TIM_Cmd(TIM1,DISABLE ); 	//1?��??������?��5
					TIM_SetCounter(TIM1,0);
					TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC2P=1 ����???a???��??2???
					TIM_Cmd(TIM1,ENABLE ); 	//��1?��?������?��5
				}		    
			}
		}
				    	
	/********************************�����̨�1**************************************/

	if((TIM1CH3_CAPTURE_STA&0X80)==0)//??????	
	{
		if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//??
		{
			if(TIM1CH3_CAPTURE_STA&0X40)        //?????????
			{
				if((TIM1CH3_CAPTURE_STA&0X3F)==0X3F)//??????
				{
					TIM1CH3_CAPTURE_STA|=0X80;	      //?????????
					TIM1CH3_CAPTURE_VAL=0XFFFF;
				}else TIM1CH3_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM1, TIM_IT_CC3) != RESET)//????4??????
		{
			if(TIM1CH3_CAPTURE_STA&0X40)//???????? 		
			{
				TIM1CH3_CAPTURE_STA|=0X80;	//??????????????
				TIM1CH3_CAPTURE_VAL=TIM_GetCapture3(TIM1);//????????.
				TIM_OC3PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC1P=0 ????????
			}else //????,????????
			{
				TIM1CH3_CAPTURE_STA=0;	//??
				TIM1CH3_CAPTURE_VAL=0;
				TIM1CH3_CAPTURE_STA|=0X40;		//?????????
				TIM_Cmd(TIM1,DISABLE); 	//?????2
				TIM_SetCounter(TIM1,0);
				TIM_OC3PolarityConfig(TIM1,TIM_ICPolarity_Falling);	//CC1P=1 ????????
				TIM_Cmd(TIM1,ENABLE); 
			}
		}
	}

	
/******************************������ 8*****************************************/	

		if((TIM1CH4_CAPTURE_STA&0X80)==0)//?1?��3��1|2???
		{
			if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//��?3?
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//��??-2???��???��???��?			
				{
					if((TIM1CH4_CAPTURE_STA&0X3F)==0X3F)//??��???��?3�訢?
					{
						TIM1CH4_CAPTURE_STA|=0X80;		//����??3��1|2???��?��?��?
						TIM1CH4_CAPTURE_VAL=0XFFFF;
					}else TIM1CH4_CAPTURE_STA++;
				}	 
			}
			if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)//2???1���騦��2???��??t
			{
				if(TIM1CH4_CAPTURE_STA&0X40)		//2???��?��??????��?? 		
				{
					TIM1CH4_CAPTURE_STA|=0X80;		//����??3��1|2???��?��?��???��??????��
					TIM1CH4_CAPTURE_VAL=TIM_GetCapture4(TIM1);//??��?�̡�?���?2????��.
					TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC2P=0 ����???a��?��y??2???
				}else  								//?1?��?a��?,�̨���?��?2???��?��y??
				{
					TIM1CH4_CAPTURE_STA=0;			//????
					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_STA|=0X40;		//����??2???��?��?��?��y??
					TIM_Cmd(TIM1,DISABLE ); 	//1?��??������?��5
					TIM_SetCounter(TIM1,0);
					TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC2P=1 ����???a???��??2???
					TIM_Cmd(TIM1,ENABLE ); 	//��1?��?������?��5
				}		    
			}
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //??3y?D??����????
}
		
		
		
		
//		Trig1 ��Ӧ������ 6��
//		Trig2 ��Ӧ������ 5��
//		Trig3 ��Ӧ������ 4��
//		Trig4 ��Ӧ������ 3��
//		Trig5 ��Ӧ������ 2��
//		Trig5 ��Ӧ������ 1��
void Ultrasonic_Trig_Start(void)
{
	 Trig1_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig1_off(); //F_Right_Trig_off()
	 Trig2_on(); //F_Right_Trig_on()
	 delay_us(15); //����10us����
	 Trig2_off(); //F_Right_Trig_off()
	 Trig3_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig3_off(); //F_Right_Trig_off()
	 Trig4_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig4_off(); //F_Right_Trig_off()
   Trig5_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig5_off(); //F_Right_Trig_off()
   Trig6_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig6_off(); //F_Right_Trig_off()
	 Trig7_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig7_off(); //F_Right_Trig_off()
	 Trig8_on(); //F_Right_Trig_on()
	 delay_us(15);  //����10us����
	 Trig8_off(); //F_Right_Trig_off()
}

//void Ultrasonic_Trig_Start(void)
//{
//	Trig1_on();
//	Trig2_on();
//	Trig3_on();
//	Trig4_on();
//	Trig5_on();
//	Trig6_on();
//	Trig7_on();  
//	Trig8_on();
//}

void Ultrasonic_Trig_end(void)
{
	Trig1_off();
	Trig2_off();
	Trig3_off();
	Trig4_off(); 
	Trig5_off();
	Trig6_off();
	Trig7_off();
	Trig8_off();
}



void Ultra_Test1(void)
{	
	  OLED_ShowNum(0,20,Sonic_Dis.A_1 ,3,8,1);
		OLED_ShowNum(0,30,Sonic_Dis.A_2 ,3,8,1);
		OLED_ShowNum(0,40,Sonic_Dis.A_3 ,3,8,1);
	  OLED_ShowNum(0,50,Sonic_Dis.A_4,3,8,1);
	
	  OLED_ShowNum(30,20,Sonic_Dis.A_5,3,8,1);
	  OLED_ShowNum(30,30,Sonic_Dis.A_6 ,3,8,1);
		OLED_ShowNum(30,40,Sonic_Dis.A_7,3,8,1);
	  OLED_ShowNum(30,50,Sonic_Dis.A_8 ,3,8,1);


}



float  center_L(int a, int b)
{
	return (a+b)/2;
}
