#ifndef Encode_H
#define Encode_H
#include "sys.h"

typedef struct 
{
	int TIM3_del_encode;
	int TIM3_sum_encode;
	int TIM4_del_encode;
	int TIM4_sum_encode;		

}Encode;

//���������ݽṹ��
typedef struct
{
	float cntA;
	float cntB;
	float speedA;
	float speedB;
	float disA;
	float disB;
}EncoderStruct;


//�������˲��ṹ��
typedef struct
{
	unsigned char 	Complete;
	unsigned char   Count;
	float   				Cache;	        //�����м仺��ֵ
	float 		 			Last_Data;	    //��һ�β���ֵ
	float  					Data_His[10];		//������ʷֵ
}EncoderFilter_Struct;


void TIM3_Encoder_Init(u16 arr,u16 psc)		;		
void TIM4_Encoder_Init(u16 arr,u16 psc)		;		
void Read_Encoder_cnt(void);
void Solve_EncoData(float time);
void sum_Read_Encoder_cnt(void);
#endif
