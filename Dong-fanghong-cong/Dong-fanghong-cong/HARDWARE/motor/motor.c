#include "motor.h"
#include "stdlib.h"

void motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_2| GPIO_Pin_4| GPIO_Pin_10| GPIO_Pin_11| GPIO_Pin_14;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_UP ;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
	
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
	if(motor_scare(PWMA)>=0) 	
			{	AIN1=1;AIN2=0;TIM_SetCompare1(TIM9,PWMA);}
	else							
			{	AIN1=0;AIN2=1;TIM_SetCompare1(TIM9,value(PWMA));}
		
	if(motor_scare(PWMB)>=0) 
			{	BIN1=1;BIN2=0;TIM_SetCompare2(TIM9,PWMB);}
		else			
			{	BIN1=0;BIN2=1;TIM_SetCompare2(TIM9,value(PWMB));}
		
	if(motor_scare(PWMC)>=0) 	{	CIN1=1;CIN2=0;TIM_SetCompare1(TIM10,PWMC);}
		else			{	CIN1=0;CIN2=1;TIM_SetCompare1(TIM10,value(PWMC));}

	if(motor_scare(PWMD)>=0) 	{	DIN1=1;DIN2=0;TIM_SetCompare1(TIM11,PWMD);}
		else			{	DIN1=0;DIN2=1;TIM_SetCompare1(TIM11,value(PWMD));}

}


