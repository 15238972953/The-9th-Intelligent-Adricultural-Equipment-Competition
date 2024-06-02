#include "encode.h"
#include "variable.h"
//int TIM3_nowcode=0;
//int TIM3_lastcode=0;
//int TIM4_nowcode=0;
//int TIM4_lastcode=0;
int TIM3_sumcode=0;
int TIM4_sumcode=0;

Encode  encode={0,0,0,0};

//int Encoder_Timer3_Overflow=0;                                      //编码器溢出次数（每389*4溢出一次）
//int Encoder_Timer4_Overflow=0; 

//存放编码器数据的结构体
EncoderStruct Encoders={0,0,0,0,0,0};
//EncoderFilter_Struct EncoAspeed_Filter = {0,0,0,0,{0}};
//EncoderFilter_Struct EncoBspeed_Filter = {0,0,0,0,{0}};


/*
********************************************************************************************************************
*                  void TIM3_Encoder_Init(u16 arr,u16 psc)
*
*Description : 定时器3的输入捕获配置
*Arguments   : 溢出时间为：（arr+1）*（psc+1）/84M
*Returns     : none
*Notes       : arr:自动重装值,psc:时钟预分频数
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

	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);		//清除TIM的更新标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);	//允许更新中断

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

	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 6;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);		//清除TIM的更新标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);	//允许更新中断

	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}

/*
********************************************************************************************************************
*                  void Read_Encoder_cnt(void)
*
*Description : 左右编码器测速函数
*Arguments   : none
*Returns     : none
*Notes       : none
********************************************************************************************************************
*/
void Read_Encoder_cnt(void)
{
	Encoders.cntA= (short)TIM3->CNT;									//（short）等效：将最高数据变为符号位
	Encoders.cntB= (short)TIM4->CNT;
	TIM3->CNT=0;
	TIM4->CNT=0;

}

/*
********************************************************************************************************************
*                  void Solve_EncoData(void)
*
*Description : 编码器解算距离、速度函数
*Arguments   : none
*Returns     : none
*Notes       :17*4*36=2448：电机正转一圈，CNT计数值  3.14：Pi   6.4：轮子直径/cm  time：解算频率/s
********************************************************************************************************************
*/
void Solve_EncoData(float time)
{
	Read_Encoder_cnt();
	Encoders.disA=(Encoders.cntA/2448.0f)*3.14f*6.4f;						//单位厘米 cm
	Encoders.disB=(Encoders.cntB/2448.0f)*3.14f*6.4f;
	Encoders.speedA=-Encoders.disA/time;												//单位厘米 cm/s
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
