#include "motor.h"
#include "stdlib.h"

//无刷电机配置
void motor_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE, ENABLE);//使能GPIOF时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4| GPIO_Pin_7;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}
	

int motor_scare(int pwm)
{
	if (pwm>1000)
		return 1000;
	else if (pwm<-1000)
		return -1000;
	else return pwm;
	
}

int value(int a)
{ if(a>0)
		return a;
	else
		return -a;


}
void motion(int PWMA,int PWMB,int PWMC,int PWMD)
{
	int addPWM_AB = 40;
	if(PWMA>=0){
		PWMA = motor_scare(PWMA+addPWM_AB);
		GPIO_SetBits(GPIOB,GPIO_Pin_4);
		TIM_SetCompare1(TIM9,PWMA);
	}else{	
		PWMA = motor_scare(PWMA-addPWM_AB);
		GPIO_ResetBits(GPIOB,GPIO_Pin_4);
		TIM_SetCompare1(TIM9,value(PWMA));
	}
		
	if(PWMB>=0){	
		PWMB = motor_scare(PWMB + addPWM_AB);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		TIM_SetCompare2(TIM9,PWMB);
	}else{	
		PWMB = motor_scare(PWMB - addPWM_AB);
		GPIO_SetBits(GPIOB,GPIO_Pin_5);
		TIM_SetCompare2(TIM9,value(PWMB));
	}
		
	if(PWMC>=0){	
		PWMC = motor_scare(PWMC);
		GPIO_ResetBits(GPIOE,GPIO_Pin_4);
		TIM_SetCompare1(TIM10,PWMC);
	}else{	
		PWMC = motor_scare(PWMC);
		GPIO_SetBits(GPIOE,GPIO_Pin_4);
		TIM_SetCompare1(TIM10,value(PWMC));
	}
	
	if(PWMD>=0){  //PWMD为出发状态的左下方电机
		PWMD = motor_scare(PWMD);
		GPIO_SetBits(GPIOE,GPIO_Pin_7);
		TIM_SetCompare1(TIM11,PWMD);
	}else{
		PWMD = motor_scare(PWMD);
		GPIO_ResetBits(GPIOE,GPIO_Pin_7);
		TIM_SetCompare1(TIM11,value(PWMD));
	}
}


