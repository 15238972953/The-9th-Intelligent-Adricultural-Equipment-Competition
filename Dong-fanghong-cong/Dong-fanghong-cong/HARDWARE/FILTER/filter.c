/*
******************************************************************************************************************
*Filename      : filter.c
*Programmer(s) : 
*Description   : �˲�
******************************************************************************************************************
*/
#include "filter.h"
/*
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
����������    �˲��ṹ�嶨������      ����������

FilterStruct ????? = {{0},0};
Filter_Struct ????? = {0,0,0,0,{0}};

!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/
/******************************************************************************************************************
*      medFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MedNumSum,unsigned char MedNumGet)
*
*Description : ������ֵ�˲��㷨
*Returns     : ��ǰ������
*Notes       : ����������ڳ�������ɻ���
*              
							*filterStruct		ѡ���˲��Ľṹ��
							newDatas				������
							MedNumGet				�˲���������
							MedNumGet				ѡ��ڼ����������
*******************************************************************************************************************
*/
//float medFilter(FilterStruct *filterStruct,short int newDatas,unsigned char MedNumSum,unsigned char MedNumGet)
//{
//	unsigned char i,j;// ѭ������
//	short int dataFilter[MedNumSum];
//	short int dataTemp = 0;
//	//��ȡ����
//	filterStruct->data[filterStruct->storeNum++] = newDatas;
//	if(filterStruct->storeNum >= MedNumSum)    
//	{
//		filterStruct->storeNum = 0;
//	}
//	
//	for(i = 0;i < MedNumSum;i ++)   
//	{
//		dataFilter[i] = filterStruct->data[i];    //���ݸ���
//	}
//	
//	// ��ð�ݷ��������������
//	for (j = 0; j < MedNumSum - 1; j ++)
//	{
//		for (i = 0; i < MedNumSum - j - 1; i ++)
//		{
//			if (dataFilter[i] > dataFilter[i + 1])
//			{
//				// ����
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
*Description : ������ֵ�˲��㷨
*Returns     : ��ǰ������
*Notes       : ����������ڳ�������ɻ���
*              
							*filterStruct		ѡ���˲��Ľṹ��
							newDatas				������
							MeanSum					�˲���������
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
*Description : ��ǿ�ͻ�����ֵ�˲��㷨    ȥ��һ�����ֵ����Сֵ
*Returns     : ��ǰ������
*Notes       : ����������ڳ�������ɻ���
*              
							*filterStruct		ѡ���˲��Ľṹ��
							newDatas				������
							EnhanceMeanSum	�˲���������
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
	
	max = min = filterStruct->data[0];    //��ʼ��
	//�ҵ������Сֵ
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
*Description : ��ֵ�˲�
*Returns     : ��ǰ������
*Notes       : ����������ڳ�������ɻ���
*              
							*filterStruct		ѡ���˲��Ľṹ��
							newDatas				������
							FilterNum				�˲���������
							*Out						���ֵ
*******************************************************************************************************************
*/
void Average_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out)
{
	unsigned int i = 0;
	float FilterSum  = 0;
	
	filterStruct->Data_His[filterStruct->Count++] = newDatas;
	
	if( filterStruct->Complete )                //�Ѿ��ɼ��������AdcFilterNum������
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
*Description : ��ǿ�;�ֵ�˲�  ȥ��һ�����ֵ����Сֵ
*Returns     : ��ǰ������
*Notes       : ����������ڳ�������ɻ���
*              
							*filterStruct		ѡ���˲��Ľṹ��
							newDatas				������
							FilterNum				�˲���������
							*Out						���ֵ
*******************************************************************************************************************
*/
void Enaverage_Filter(Filter_Struct *filterStruct,short int newDatas,unsigned char FilterNum,short int *Out)
{
	unsigned int i = 0;
	float FilterSum  = 0;
	static unsigned short int max,min;
	
	filterStruct->Data_His[filterStruct->Count++] = newDatas;
	
	if( filterStruct->Complete )                //�Ѿ��ɼ��������AdcFilterNum������
	{
		max = min = filterStruct->Data_His[0];    //��ʼ��
	//�ҵ������Сֵ
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





