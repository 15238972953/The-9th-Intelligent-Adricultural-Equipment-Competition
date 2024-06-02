#include "key.h"
#include "delay.h"
#include "variable.h"
void KEY_Init(void)
{
	
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
} 

//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//0，没有任何按键按下
//1，KEY0按下
//2，KEY1按下
//3，KEY2按下 
//4，WKUP按下 WK_UP
//注意此函数有响应优先级,KEY0>KEY1>KEY2>WK_UP!!
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))//有按键按下
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		if(KEY2==0)return 2;
		if(KEY3==0)return 3;
		if(KEY4==0)return 4;
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}

u8 KEY_Scan_2(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))//有按键按下
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		if(KEY2==0)return 2;
		if(KEY3==0)return 3;
		if(KEY4==0)return 4;
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}

u8 KEY_Scan_3(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY1==0||KEY2==0||KEY3==0||KEY4==0))//有按键按下
	{
		delay_ms(10);//去抖动 
		key_up=0;
		if(KEY1==0)return 1;
		if(KEY2==0)return 2;
		if(KEY3==0)return 3;
		if(KEY4==0)return 4;
	}
	else if(KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1)key_up=1; 	    
 	return 0;// 无按键按下
}


void choose_servo(void)
{
	static int key=0;
	static int key_2=0;	
	static int key_3=0;
	static int key_4=100;
	static int increase=0;
	static int reduce=0;
	OLED_ShowString(98,0,"S",8,1);
	OLED_ShowString(108,0,":",8,1);
	OLED_ShowNum(116,0,key_2 ,1,8,1);
	OLED_ShowNum(96,10,key_3 ,3,8,1);
	OLED_ShowNum(96,20,key_4 ,3,8,1);
	OLED_ShowIntNum(96,30,increase ,4,8,1);
	OLED_ShowIntNum(96,40,reduce ,4,8,1);
		if(KEY_Scan(0)==2)
		{
		key_2++;
		if(key_2>8)
			key_2=0;
		OLED_ShowNum(116,0,key_2 ,1,8,1);
		}
			
		switch(key_2)
		{
			case 1:
//				key_3=0;
//				key_4=100;
				if(KEY_Scan_2(0)==3)
				{
					Set_angle(35+key_3,143,133,40);
					OLED_ShowNum(96,10,(u32)(35+key_3) ,3,8,1);
				//	OLED_Refresh();
					key_3++;
				}
				if(KEY_Scan_3(0)==4)
				{
					Set_angle(35-(100-key_4),143,133,40);
					OLED_ShowNum(96,20,(u32)(35-(100-key_4)) ,3,8,1);
					//OLED_Refresh();
					key_4--;
				}
				break;
				
			case 2:
				if(KEY_Scan_2(0)==3)
				{
					Set_angle(35,143+key_3,133,40);
					OLED_ShowNum(108,10,(143+key_3) ,3,8,1);
					key_3++;
				}
				if(KEY_Scan_3(0)==4)
				{
					Set_angle(35,143-(100-key_4),133,40);
					OLED_ShowNum(108,10,143-(100-key_4) ,3,8,1);
					key_4--;
				}
				break;
				
			case 3:
				if(KEY_Scan_2(0)==3)
				{
					Set_angle(35,143,133+key_3,40);
					OLED_ShowNum(108,10,133+key_3 ,3,8,1);
					key_3++;
				}
				if(KEY_Scan_3(0)==4)
				{
					Set_angle(35,143,133-(100-key_4),40);
					OLED_ShowNum(108,10,133-(100-key_4) ,3,8,1);
					key_4--;
				}
				break;
				
			case 4:
				if(KEY_Scan_2(0)==3)
				{
					Set_angle(35,143,133,40+key_3);
					OLED_ShowNum(108,10,40+key_3 ,3,8,1);
					key_3++;
				}
				if(KEY_Scan_3(0)==4)
				{
					Set_angle(35,143,133,40-(100-key_4));
					OLED_ShowNum(108,10,40-(100-key_4) ,3,8,1);
					key_4--;
				}
				break;
				
			case 5:
				if(KEY_Scan_2(0)==3)
				{
					motion(0+increase,0,0,0);
					OLED_ShowIntNum(96,10,increase ,4,8,1);
					increase+=50;
				}
				if(KEY_Scan_3(0)==4)
				{
					motion(0-reduce ,0,0,0);
					OLED_ShowIntNum(108,10,reduce ,4,8,1);
					reduce-=50;
				}
				break;	
		}
}
