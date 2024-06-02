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
*Description : �������궨������
*Notes       : none
******************************************************************************************************************
*/


//#define MedNumSum  5       //��ֵ�˲�����������
//#define MedNumGet  2       //���ȡ��һ���������
//#define MeanSum   20       
//#define EnhanceMeanSum 10  // ��ǿ�;�ֵ�˲��㷨
//#define FilterNum  5        //������Ȩ�˲����������С


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
	short int  				Cache;	        //�����м仺��ֵ
	short int 					Last_Data;	    //��һ�β���ֵ
	short int  				Data_His[15];		//������ʷֵ
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


