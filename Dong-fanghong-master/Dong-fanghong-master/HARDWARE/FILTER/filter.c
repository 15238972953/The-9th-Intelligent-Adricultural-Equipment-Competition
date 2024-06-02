/*
******************************************************************************************************************
*Filename      : filter.c
*Programmer(s) : 
*Description   : 滤波
******************************************************************************************************************
*/
#include "filter.h"
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
！！！！！    滤波结构体定义事例      ！！！！！

FilterStruct ????? = {{0},0};
Filter_Struct ????? = {0,0,0,0,{0}};

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
/******************************************************************************************************************
*      medFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MedNumSum,unsigned char MedNumGet)
*
*Description : 连续中值滤波算法
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*              
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							MedNumGet				滤波样本总数
							MedNumGet				选择第几个数据输出
*******************************************************************************************************************
*/
//float medFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MedNumSum,unsigned char MedNumGet)
//{
//	unsigned char i,j;// 循环变量
//	short int dataFilter[MedNumSum];
//	short int dataTemp = 0;
//	//存取数据
//	filterStruct->data[filterStruct->storeNum++] = newDatas;
//	if(filterStruct->storeNum >= MedNumSum)    
//	{
//		filterStruct->storeNum = 0;
//	}
//	
//	for(i = 0;i < MedNumSum;i ++)   
//	{
//		dataFilter[i] = filterStruct->data[i];    //数据复制
//	}
//	
//	// 用冒泡法对数组进行排序
//	for (j = 0; j < MedNumSum - 1; j ++)
//	{
//		for (i = 0; i < MedNumSum - j - 1; i ++)
//		{
//			if (dataFilter[i] > dataFilter[i + 1])
//			{
//				// 互换
//				dataTemp = dataFilter[i];
//				dataFilter[i] = dataFilter[i + 1];
//				dataFilter[i + 1] = dataTemp;
//			}
//		}
//	}
//	return dataFilter[MedNumGet];
//}


/******************************************************************************************************************
*        			  meanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MeanSum)
*
*Description : 滑动均值滤波算法
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*              
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							MeanSum					滤波样本总数
*******************************************************************************************************************
*/
int meanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MeanSum)
{
	float dataSum = 0;
	unsigned char i = 0;
	filterStruct->data[filterStruct->storeNum++] = newDatas;  
	if(filterStruct->storeNum >= MeanSum)
	{
		filterStruct->storeNum = 0;
	}
	
	for(i = 0;i < MeanSum;i++)
	{
		dataSum += filterStruct->data[i];
	}
	return (int)(dataSum/MeanSum);
}


float meanFilter1(FilterStruct1 *filterStruct,float newDatas,unsigned char MeanSum)
{
	float dataSum = 0.0;
	unsigned char i = 0;
	filterStruct->data[filterStruct->storeNum++] = newDatas;  
	if(filterStruct->storeNum >= MeanSum)
	{
		filterStruct->storeNum = 0;
	}
	
	for(i = 0;i < MeanSum;i++)
	{
		dataSum += filterStruct->data[i];
	}
	return dataSum/MeanSum;
}


/******************************************************************************************************************
*          enhanceMeanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char EnhanceMeanSum)
*
*Description : 增强型滑动均值滤波算法    去掉一个最大值和最小值
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*              
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							EnhanceMeanSum	滤波样本总数
*******************************************************************************************************************
*/
float enhanceMeanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char EnhanceMeanSum)
{
	float dataSum = 0;
	unsigned char i = 0;
	static short int max,min;
	filterStruct->data[filterStruct->storeNum++] = newDatas;  
	if(filterStruct->storeNum >= EnhanceMeanSum)
	{
		filterStruct->storeNum = 0;
	}
	
	max = min = filterStruct->data[0];    //初始化
	//找到最大最小值
	for(i =1;i < EnhanceMeanSum; i ++)
	{
		if(filterStruct->data[i] > max)
		{
			max = filterStruct->data[i];
		}
		
		if(filterStruct->data[i] < min)
		{
			min = filterStruct->data[i];
		}
	}
	
	for(i = 0;i < EnhanceMeanSum;i ++)
	{
		dataSum += filterStruct->data[i];
	}
	dataSum -= (max + min); 
	return dataSum/(EnhanceMeanSum-2);
}


/******************************************************************************************************************
*             Average_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum)
*
*Description : 均值滤波
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*              
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							FilterNum				滤波样本总数
							*Out						输出值
*******************************************************************************************************************
*/
void Average_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out)
{
	unsigned int i = 0;
	float FilterSum  = 0;
	
	filterStruct->Data_His[filterStruct->Count++] = newDatas;
	
	if( filterStruct->Complete )                //已经采集完最初的AdcFilterNum个数据
	{
		for(i = 0; i < FilterNum; i++)
		{
			FilterSum += filterStruct->Data_His[i];
		}
		filterStruct->Cache = FilterSum / (FilterNum);
	}
	
	if(filterStruct->Complete  == 1)
	{
		if(filterStruct->Cache < 5000)
		{
			*Out 	 = filterStruct->Cache;
			filterStruct->Last_Data 	 = filterStruct->Cache;
		}
		else
			*Out = filterStruct->Last_Data;
	}
	if(filterStruct->Count == FilterNum)
	{
		filterStruct->Complete   = 1;
		filterStruct->Count = 0;
	}
}


/******************************************************************************************************************
*             Enaverage_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum)
*
*Description : 增强型均值滤波  去掉一个最大值和最小值
*Returns     : 当前的数据
*Notes       : 输入的数据在程序中完成缓存
*              
							*filterStruct		选择滤波的结构体
							newDatas				新数据
							FilterNum				滤波样本总数
							*Out						输出值
*******************************************************************************************************************
*/
void Enaverage_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out)
{
	unsigned int i = 0;
	float FilterSum  = 0;
	static unsigned short int max,min;
	
	filterStruct->Data_His[filterStruct->Count++] = newDatas;
	
	if( filterStruct->Complete )                //已经采集完最初的AdcFilterNum个数据
	{
		max = min = filterStruct->Data_His[0];    //初始化
	//找到最大最小值
		for(i =1;i < FilterNum; i ++)
		{
			if(filterStruct->Data_His[i] > max)
			{
				max = filterStruct->Data_His[i];
			}
			
			if(filterStruct->Data_His[i] < min)
			{
				min = filterStruct->Data_His[i];
			}
		}
		
		for(i = 0; i < FilterNum; i++)
		{
			FilterSum += filterStruct->Data_His[i];
		}
		FilterSum -= max + min;
		filterStruct->Cache = FilterSum / (FilterNum-2);
	}
	
	if(filterStruct->Complete  == 1)
	{
		if(filterStruct->Cache < 5000)
		{
			*Out = filterStruct->Cache;
			filterStruct->Last_Data 	 = filterStruct->Cache;
		}
		else
			*Out = filterStruct->Last_Data;
	}
	
	
	if(filterStruct->Count == FilterNum)
	{
		
		filterStruct->Complete = 1;
		filterStruct->Count = 0;
		
	}
	
	
	
}





