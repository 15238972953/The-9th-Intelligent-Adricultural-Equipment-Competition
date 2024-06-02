#include "encode.h"
#include "variable.h"
//int TIM3_nowcode=0;
//int TIM3_lastcode=0;
//int TIM4_nowcode=0;
//int TIM4_lastcode=0;
int TIM3_sumcode=0;
int TIM4_sumcode=0;

Encode  encode={0,0,0,0};

//int Encoder_Timer3_Overflow=0;                                      //���������������ÿ389*4���һ�Σ�
//int Encoder_Timer4_Overflow=0; 

//��ű��������ݵĽṹ��
EncoderStruct Encoders={0,0,0,0,0,0};
//EncoderFilter_Struct EncoAspeed_Filter = {0,0,0,0,{0}};
//EncoderFilter_Struct EncoBspeed_Filter = {0,0,0,0,{0}};


/*
********************************************************************************************************************
*                  void TIM3_Encoder_Init(u16 arr,u16 psc)
*
*Description : ��ʱ��3�����벶������
*Arguments   : ���ʱ��Ϊ����arr+1��*��psc+1��/84M
*Returns     : none
*Notes       : arr:�Զ���װֵ,psc:ʱ��Ԥ��Ƶ��
********************************************************************************************************************
*/
void TIM3_Encoder_Init(u16 arr,u16 psc)				//EncoderB
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_TIM3); 
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_TIM3);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);		//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//��������ж�

	TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE); 
}


void TIM4_Encoder_Init(u16 arr,u16 psc)				//EncoderC
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_TIM4); 
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4);

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Prescaler=psc;
	TIM_TimeBaseStructure.TIM_Period=arr;
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);		//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	//��������ж�

	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}

/*
********************************************************************************************************************
*                  void Read_Encoder_cnt(void)
*
*Description : ���ұ��������ٺ���
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Read_Encoder_cnt(void)
{
	Encoders.cntA= (short)TIM3->CNT;									//��short����Ч����������ݱ�Ϊ����λ
	Encoders.cntB= (short)TIM4->CNT;
	TIM3->CNT=0;
	TIM4->CNT=0;

}

/*
********************************************************************************************************************
*                  void Solve_EncoData(void)
*
*Description : ������������롢�ٶȺ���
*Arguments   : none
*Returns     : none
*Notes       :17*4*36=2448�������תһȦ��CNT����ֵ  3.14��Pi   6.4������ֱ��/cm  time������Ƶ��/s
********************************************************************************************************************
*/
void Solve_EncoData(float time)
{
	Read_Encoder_cnt();
	Encoders.disA=(Encoders.cntA/2448.0f)*3.14f*6.4f;						//��λ���� cm
	Encoders.disB=(Encoders.cntB/2448.0f)*3.14f*6.4f;
	Encoders.speedA=-Encoders.disA/time;												//��λ���� cm/s
	Encoders.speedB=-Encoders.disB/time; 
}

void sum_Read_Encoder_cnt(void)
{	
	Read_Encoder_cnt();
	TIM3_sumcode=Encoders.cntA+TIM3_sumcode ;
	TIM4_sumcode=Encoders.cntB+TIM4_sumcode ;
	encode.TIM3_sum_encode=TIM3_sumcode;
	encode.TIM4_sum_encode=TIM4_sumcode;
}
