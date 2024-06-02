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
			//定时器5
u8   TIM5CH1_CAPTURE_STA=0;	//输入捕获状态		    PA0				
u32	 TIM5CH1_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM5CH2_CAPTURE_STA=0;	//输入捕获状态		    PA1				
u32	 TIM5CH2_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM5CH3_CAPTURE_STA=0;	//输入捕获状态		    PA2				
u32	 TIM5CH3_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM5CH4_CAPTURE_STA=0;	//输入捕获状态		    PA3			
u32	 TIM5CH4_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
    
		 //定时器1
u8   TIM1CH1_CAPTURE_STA=0;	//输入捕获状态		    PB10				
u32	 TIM1CH1_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM1CH2_CAPTURE_STA=0;	//输入捕获状态		    PB11				
u32	 TIM1CH2_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM1CH3_CAPTURE_STA=0;	//输入捕获状态		    PB10				
u32	 TIM1CH3_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)
u8   TIM1CH4_CAPTURE_STA=0;	//输入捕获状态		    PB11				
u32  TIM1CH4_CAPTURE_VAL=0;	//输入捕获值(TIM2/TIM5是32位)

long long temp_1=0;//前                      T5C3        4
long long temp_2=0;//后                   //T1C2         1 
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
*Description    :初始化超声波
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
		if(TIM5CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_1=TIM5CH1_CAPTURE_STA&0X3F;
//				tempF*=0XFFFFFFFF;        //溢出时间总和
				temp_1+=TIM5CH1_CAPTURE_VAL; //得到总的高电平时间
				TIM5CH1_CAPTURE_STA=0;      //开启下一次捕获
		}
		 TEMPA_1=(temp_1*340/2)/10000;   //测量距离单位cm
		 Enaverage_Filter(&SonicA1_Filter,TEMPA_1,5, &Sonic_Dis.A_1);


		if(TIM5CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_2=TIM5CH2_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_2+=TIM5CH2_CAPTURE_VAL;//得到总的高电平时间
				TIM5CH2_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_2=(temp_2*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA2_Filter,TEMPA_2,5, &Sonic_Dis.A_2);
			
    if(TIM5CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_3=TIM5CH3_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_3=TIM5CH3_CAPTURE_VAL;//得到总的高电平时间
				TIM5CH3_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_3=(temp_3*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA3_Filter,TEMPA_3,5, &Sonic_Dis.A_3);
			
		if(TIM5CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_4=TIM5CH4_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_4+=TIM5CH4_CAPTURE_VAL;//得到总的高电平时间
				TIM5CH4_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_4=(temp_4*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA4_Filter,TEMPA_4,5, &Sonic_Dis.A_4);
			
		
		
		
		
		
		if(TIM1CH1_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_5=TIM1CH1_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_5+=TIM1CH1_CAPTURE_VAL;//得到总的高电平时间
				TIM1CH1_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_5=(temp_5*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA5_Filter,TEMPA_5,5, &Sonic_Dis.A_5);
		
		if(TIM1CH2_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_6=TIM1CH2_CAPTURE_STA&0X3F; 
				//temp_6*=0XFFFF;         //溢出时间总和
				temp_6+=TIM1CH2_CAPTURE_VAL;  //得到总的高电平时间
				TIM1CH2_CAPTURE_STA=0;       //开启下一次捕获
		}
		TEMPA_6=(temp_6*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA6_Filter,TEMPA_6,5, &Sonic_Dis.A_6);
	
    if(TIM1CH3_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_7=TIM1CH3_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_7=TIM1CH3_CAPTURE_VAL;//得到总的高电平时间
				TIM1CH3_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_7=(temp_7*340/2)/10000; //测量距离单位cm
		Enaverage_Filter(&SonicA7_Filter,TEMPA_7,5, &Sonic_Dis.A_7);
			
		if(TIM1CH4_CAPTURE_STA&0X80)//成功捕获到了一次高电平
		{
				temp_8=TIM1CH4_CAPTURE_STA&0X3F; 
//				tempB*=0XFFFFFFFF;         //溢出时间总和
				temp_8+=TIM1CH4_CAPTURE_VAL;//得到总的高电平时间
				TIM1CH4_CAPTURE_STA=0;     //开启下一次捕获
		}
		TEMPA_8=(temp_8*340/2)/10000; //测量距离单位cm
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
*Description : 定时器5初始化,周期为1ms
*Arguments   : arr：重装载值   psc：预分频系数
*Returns     : none
*Notes       : 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.  
               Ft=定时器工作频率,单位:Mhz
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
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//模式设为复用功能
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN;//设为下拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//速度设为100MHz 
  GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);
	
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5);
  GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_TIM5);
	
	 /***********************Timer initial****************************************/	 
	TIM_TimeBaseStructure.TIM_Prescaler=psc;//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;//自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	
	//初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;//CC1S=04,选择输入端 IC4映射到TI1上
  TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;//上升沿捕获
  TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;//映射到TI1上
  TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;//配置输入分频,不分频 
  TIM5_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 配置输入滤波器 不滤波
  TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);//允许更新中断 ,允许CC4IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);						// 允许更新中断 ,允许CC2IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_CC2,ENABLE);						// 允许更新中断 ,允许CC2IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_CC3,ENABLE);						// 允许更新中断 ,允许CC2IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_CC4,ENABLE);						// 允许更新中断 ,允许CC2IE捕获中断	
	
  TIM_Cmd(TIM5,ENABLE );//使能定时器5
	
  /***********************NVIC initial****************************************/
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);//根据指定的参数初始化VIC寄存器	
	
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
*Description    :定时器5中断服务函数 
*Arguments   : none
*Returns     : TRIGx  超声波x
*Notes       : none
*F  T5C3   5号位
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
	if((TIM5CH1_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{
			if(TIM5CH1_CAPTURE_STA&0X40)        //已经捕获到高电平了
			{
				if((TIM5CH1_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH1_CAPTURE_STA|=0X80;	      //标记成功捕获了一次
					TIM5CH1_CAPTURE_VAL=0XFFFFFFFF;   //已为最大值
				}else TIM5CH1_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)//捕获通道4发生捕获事件
		{
			if(TIM5CH1_CAPTURE_STA&0X40)//捕获到一个下降沿 		
			{
				TIM5CH1_CAPTURE_STA|=0X80;	//标记成功捕获到一次高电平脉宽
				TIM5CH1_CAPTURE_VAL=TIM_GetCapture1(TIM5);//获取当前的捕获值.
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
				TIM5CH1_CAPTURE_STA=0;	//清空
				TIM5CH1_CAPTURE_VAL=0;
				TIM5CH1_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE); 	//关闭定时器2
				TIM_SetCounter(TIM5,0);
				TIM_OC1PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器2
			}
		}
	}
//----------------------------------------------------------------------------------------------------------------- 
	
	if((TIM5CH2_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{
			if(TIM5CH2_CAPTURE_STA&0X40)        //已经捕获到高电平了
			{
				if((TIM5CH2_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH2_CAPTURE_STA|=0X80;	      //标记成功捕获了一次
					TIM5CH2_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH2_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)//捕获通道4发生捕获事件
		{
			if(TIM5CH2_CAPTURE_STA&0X40)//捕获到一个下降沿 		
			{
				TIM5CH2_CAPTURE_STA|=0X80;	//标记成功捕获到一次高电平脉宽
				TIM5CH2_CAPTURE_VAL=TIM_GetCapture2(TIM5);//获取当前的捕获值.
				TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
				TIM5CH2_CAPTURE_STA=0;	//清空
				TIM5CH2_CAPTURE_VAL=0;
				TIM5CH2_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器2
				TIM_SetCounter(TIM5,0);
				TIM_OC2PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器2
			}
		}
	}
	
	if((TIM5CH3_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{
			if(TIM5CH3_CAPTURE_STA&0X40)        //已经捕获到高电平了
			{
				if((TIM5CH3_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH3_CAPTURE_STA|=0X80;	      //标记成功捕获了一次
					TIM5CH3_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH3_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)//捕获通道4发生捕获事件
		{
			if(TIM5CH3_CAPTURE_STA&0X40)//捕获到一个下降沿 		
			{
				TIM5CH3_CAPTURE_STA|=0X80;	//标记成功捕获到一次高电平脉宽
				TIM5CH3_CAPTURE_VAL=TIM_GetCapture3(TIM5);//获取当前的捕获值.
				TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
				TIM5CH3_CAPTURE_STA=0;	//清空
				TIM5CH3_CAPTURE_VAL=0;
				TIM5CH3_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE ); 	//关闭定时器2
				TIM_SetCounter(TIM5,0);
				TIM_OC3PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器2
			}
		}
	}
	
	if((TIM5CH4_CAPTURE_STA&0X80)==0)//还未成功捕获	
	{
		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)//溢出
		{
			if(TIM5CH4_CAPTURE_STA&0X40)        //已经捕获到高电平了
			{
				if((TIM5CH4_CAPTURE_STA&0X3F)==0X3F)//高电平太长了
				{
					TIM5CH4_CAPTURE_STA|=0X80;	      //标记成功捕获了一次
					TIM5CH4_CAPTURE_VAL=0XFFFFFFFF;
				}else TIM5CH4_CAPTURE_STA++;
			}
		}
		if(TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)//捕获通道4发生捕获事件
		{
			if(TIM5CH4_CAPTURE_STA&0X40)//捕获到一个下降沿 		
			{
				TIM5CH4_CAPTURE_STA|=0X80;	//标记成功捕获到一次高电平脉宽
				TIM5CH4_CAPTURE_VAL=TIM_GetCapture4(TIM5);//获取当前的捕获值.
				TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Rising); //CC1P=0 设置为上升沿捕获
			}else //还未开始,第一次捕获上升沿
			{
				TIM5CH4_CAPTURE_STA=0;	//清空
				TIM5CH4_CAPTURE_VAL=0;
				TIM5CH4_CAPTURE_STA|=0X40;		//标记捕获到了上升沿
				TIM_Cmd(TIM5,DISABLE); 	//关闭定时器2
				TIM_SetCounter(TIM5,0);
				TIM_OC4PolarityConfig(TIM5,TIM_ICPolarity_Falling);	//CC1P=1 设置为下降沿捕获
				TIM_Cmd(TIM5,ENABLE ); 	//使能定时器2
			}
		}
	}
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //清除中断标志位
}


	/********************************定时器1 捕获中断**************************************/
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

	
/******************************í¨μà2*****************************************/	

		if((TIM1CH2_CAPTURE_STA&0X80)==0)//?1?′3é1|2???
		{
			if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//ò?3?
			{
				if(TIM1CH2_CAPTURE_STA&0X40)//ò??-2???μ???μ???á?			
				{
					if((TIM1CH2_CAPTURE_STA&0X3F)==0X3F)//??μ???ì?3¤á?
					{
						TIM1CH2_CAPTURE_STA|=0X80;		//±ê??3é1|2???á?ò?′?
						TIM1CH2_CAPTURE_VAL=0XFFFF;
					}else TIM1CH2_CAPTURE_STA++;
				}	 
			}
			if(TIM_GetITStatus(TIM1, TIM_IT_CC2) != RESET)//2???1·￠éú2???ê??t
			{
				if(TIM1CH2_CAPTURE_STA&0X40)		//2???μ?ò??????μ?? 		
				{
					TIM1CH2_CAPTURE_STA|=0X80;		//±ê??3é1|2???μ?ò?′???μ??????í
					TIM1CH2_CAPTURE_VAL=TIM_GetCapture2(TIM1);//??è?μ±?°μ?2????μ.
					TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC2P=0 éè???aé?éy??2???
				}else  								//?1?′?aê?,μúò?′?2???é?éy??
				{
					TIM1CH2_CAPTURE_STA=0;			//????
					TIM1CH2_CAPTURE_VAL=0;
					TIM1CH2_CAPTURE_STA|=0X40;		//±ê??2???μ?á?é?éy??
					TIM_Cmd(TIM1,DISABLE ); 	//1?±??¨ê±?÷5
					TIM_SetCounter(TIM1,0);
					TIM_OC2PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC2P=1 éè???a???μ??2???
					TIM_Cmd(TIM1,ENABLE ); 	//ê1?ü?¨ê±?÷5
				}		    
			}
		}
				    	
	/********************************í¨μà1**************************************/

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

	
/******************************超声波 8*****************************************/	

		if((TIM1CH4_CAPTURE_STA&0X80)==0)//?1?′3é1|2???
		{
			if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)//ò?3?
			{
				if(TIM1CH4_CAPTURE_STA&0X40)//ò??-2???μ???μ???á?			
				{
					if((TIM1CH4_CAPTURE_STA&0X3F)==0X3F)//??μ???ì?3¤á?
					{
						TIM1CH4_CAPTURE_STA|=0X80;		//±ê??3é1|2???á?ò?′?
						TIM1CH4_CAPTURE_VAL=0XFFFF;
					}else TIM1CH4_CAPTURE_STA++;
				}	 
			}
			if(TIM_GetITStatus(TIM1, TIM_IT_CC4) != RESET)//2???1·￠éú2???ê??t
			{
				if(TIM1CH4_CAPTURE_STA&0X40)		//2???μ?ò??????μ?? 		
				{
					TIM1CH4_CAPTURE_STA|=0X80;		//±ê??3é1|2???μ?ò?′???μ??????í
					TIM1CH4_CAPTURE_VAL=TIM_GetCapture4(TIM1);//??è?μ±?°μ?2????μ.
					TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Rising); //CC2P=0 éè???aé?éy??2???
				}else  								//?1?′?aê?,μúò?′?2???é?éy??
				{
					TIM1CH4_CAPTURE_STA=0;			//????
					TIM1CH4_CAPTURE_VAL=0;
					TIM1CH4_CAPTURE_STA|=0X40;		//±ê??2???μ?á?é?éy??
					TIM_Cmd(TIM1,DISABLE ); 	//1?±??¨ê±?÷5
					TIM_SetCounter(TIM1,0);
					TIM_OC4PolarityConfig(TIM1,TIM_ICPolarity_Falling);		//CC2P=1 éè???a???μ??2???
					TIM_Cmd(TIM1,ENABLE ); 	//ê1?ü?¨ê±?÷5
				}		    
			}
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_CC1|TIM_IT_CC2|TIM_IT_CC3|TIM_IT_CC4|TIM_IT_Update); //??3y?D??±ê????
}
		
		
		
		
//		Trig1 对应超声波 6；
//		Trig2 对应超声波 5；
//		Trig3 对应超声波 4；
//		Trig4 对应超声波 3；
//		Trig5 对应超声波 2；
//		Trig5 对应超声波 1；
void Ultrasonic_Trig_Start(void)
{
	 Trig1_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig1_off(); //F_Right_Trig_off()
	 Trig2_on(); //F_Right_Trig_on()
	 delay_us(15); //大于10us脉冲
	 Trig2_off(); //F_Right_Trig_off()
	 Trig3_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig3_off(); //F_Right_Trig_off()
	 Trig4_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig4_off(); //F_Right_Trig_off()
   Trig5_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig5_off(); //F_Right_Trig_off()
   Trig6_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig6_off(); //F_Right_Trig_off()
	 Trig7_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
	 Trig7_off(); //F_Right_Trig_off()
	 Trig8_on(); //F_Right_Trig_on()
	 delay_us(15);  //大于10us脉冲
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
		OLED_Refresh();
		delay_ms(10);

}



float  center_L(int a, int b)
{
	return (a+b)/2;
}
