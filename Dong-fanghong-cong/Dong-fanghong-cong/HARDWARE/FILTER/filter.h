/*
******************************************************************************************************************
*Filename      :xxx.h
*Programmer(s) :Nrush
*Description   :xxx function
******************************************************************************************************************
*/

#ifndef __FILTER_H
#define __FILTER_H

/*
******************************************************************************************************************
*                                            CONSTANTS & MACROS
*Description : 变量及宏定义声明
*Notes       : none
******************************************************************************************************************
*/


//#define MedNumSum  5       //中值滤波的数据总数
//#define MedNumGet  2       //最后取哪一个数据输出
//#define MeanSum   20       
//#define EnhanceMeanSum 10  // 增强型均值滤波算法
//#define FilterNum  5        //滑动加权滤波缓存数组大小


typedef struct
{
	short int data[25];
	unsigned char storeNum;
}FilterStruct;

typedef struct
{
	float data[100];
	unsigned char storeNum;
}FilterStruct1;

typedef struct
{
	unsigned char 			Complete;
	unsigned char      Count;
	short int  				Cache;	        //计算中间缓存值
	short int 					Last_Data;	    //上一次捕获值
	short int  				Data_His[15];		//数据历史值
 }Filter_Struct;
/*
******************************************************************************************************************
*                                            FUNCTION PROTOTYPES
******************************************************************************************************************
*/

extern float medFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MedNumSum,unsigned char MedNumGet);
extern int   meanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MeanSum);
extern float meanFilter1(FilterStruct1 *filterStruct,float newDatas,unsigned char MeanSum);
extern float enhanceMeanFilter(FilterStruct *filterStruct,short int newDatas,unsigned char EnhanceMeanSum);
void Average_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out);
void Enaverage_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out);

#endif


