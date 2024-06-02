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
int Encoder_Timer3_sum=0;//定时器3编码器总数
int Encoder_Timer4_sum=0;//定时器4编码器总数
int PID_value;
int empt=0; //判断标志位，是否停车施药的标志

void TIM6_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	TIM_Cmd(TIM6,DISABLE); //初始化时先不开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn; //定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM7_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);  ///使能TIM7时钟
	
    TIM_TimeBaseInitStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	TIM_Cmd(TIM7,ENABLE); //初始化时先不开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM7_IRQn; //定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM7_IRQHandler(void)                          //得到编码器素的和位置
{
	if(TIM_GetITStatus(TIM7,TIM_IT_Update)==SET)  
    //溢出中断
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
                PID_value=blance_pid_hor(30,0.1,10,YAW);//全哥pid：100,0.1,1//跨龙pid
            else
                PID_value=blance_pid_hor(10,1,10,YAW);//25,1,10//35,1,15//竖着pid
            //OLED_ShowIntNum(50,50,PID_value,5,8,1);
			//OLED_ShowIntNum(50,0,YAW*100,5,8,1);
//				OLED_ShowIntNum(0,0,Encoder_Timer3_sum ,5,8,1);
//			OLED_ShowIntNum(0,10,Encoder_Timer4_sum ,5,8,1);
			//Ultra_Test1();
			OLED_ShowNum(40,10,flag ,2,8,1); //显示flag标志；
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


	TIM_ClearITPendingBit(TIM7,TIM_IT_Update);        //清除中断标志位
}

void TIM13_init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13,ENABLE);  ///使能TIM7时钟
	
  TIM_TimeBaseInitStructure.TIM_Period = arr;   //自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM13,&TIM_TimeBaseInitStructure);//初始化TIM7
	
	TIM_ITConfig(TIM13,TIM_IT_Update,ENABLE); //允许定时器6更新中断
	TIM_Cmd(TIM13,ENABLE); //初始化时先不开启定时器7
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM8_UP_TIM13_IRQn; //定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x02; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x02; //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//void TIM14_init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14,ENABLE);  ///使能TIM7时钟
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr;   //自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	TIM_TimeBaseInit(TIM14,&TIM_TimeBaseInitStructure);//初始化TIM7
//	
//	TIM_ITConfig(TIM14,TIM_IT_Update,ENABLE); //允许定时器6更新中断
//	TIM_Cmd(TIM14,ENABLE); //初始化时先不开启定时器7
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM8_TRG_COM_TIM14_IRQn; //定时器6中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x00; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x01; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}

void TIM8_UP_TIM13_IRQHandler() //定时器13中断 显示
{
	if(TIM_GetITStatus(TIM13,TIM_IT_Update)==SET)      //溢出中断
		{  

		Ultrasonic_Trig_Start();
		//tly();
			
	}
	TIM_ClearITPendingBit(TIM13,TIM_IT_Update);
}

//通用定时器3中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器3!
//void TIM3_Int_Init(u16 arr,u16 psc)
//{
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;
//	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  ///使能TIM3时钟
//	
//  TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
//	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
//	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
//	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
//	
//	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);//初始化TIM3
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许定时器3更新中断
//	TIM_Cmd(TIM3,DISABLE); //使能定时器3
//	
//	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; //定时器3中断
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; //子优先级3
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//}

////定时器3中断服务函数
//void TIM3_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
//	{
//		flag_kualong++;
//		
//	}
//	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
//}




void TIM6_DAC_IRQHandler(void)//显示 数字 
{
		if(TIM_GetITStatus(TIM6,TIM_IT_Update)==SET){
			flag_kualong++;
		}
		TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
}
	
