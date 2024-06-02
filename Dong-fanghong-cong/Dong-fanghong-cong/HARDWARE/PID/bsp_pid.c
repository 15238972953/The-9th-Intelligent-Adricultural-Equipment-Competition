#include "bsp_pid.h"
#include "math.h"


//���붨���ͬʱ





//void PID_param_init()
//{
//	/* λ����س�ʼ������ */
//			
//	pid_location.output_val = 0.0;
//	pid_location.err = 0.0;
//	pid_location.err_last = 0.0;
//	pid_location.integral = 0.0;

//	pid_location.Kp = 0.05;
//	pid_location.Ki = 0;
//	pid_location.Kd = 0;

//	/* �ٶ���س�ʼ������ */
//			
//	pid_speed.output_val=0.0;
//	pid_speed.err=0.0;
//	pid_speed.err_last=0.0;
//	pid_speed.integral=0.0;

//	pid_speed.Kp = 80.0;
//	pid_speed.Ki = 2.0;
//	pid_speed.Kd = 100.0;
//}



#define LOC_DEAD_ZONE 60 /*λ�û�����*/
#define LOC_INTEGRAL_START_ERR 200 /*���ַ���ʱ��Ӧ����Χ*/
#define LOC_INTEGRAL_MAX_VAL 800   /*���ַ�Χ�޶�����ֹ���ֱ���*/

float location_pid_set(float Kp, float Ki, float Kd, float target_val ,float actual_val)
{
	PID pid_location;
	pid_location.Kp = Kp;
	pid_location.Ki = Ki;
	pid_location.Kd = Kd;
	pid_location.err = target_val - actual_val;
	if((pid_location.err  >= -LOC_DEAD_ZONE) && (pid_location.err  <= LOC_DEAD_ZONE))
	{
		pid_location.err  = 0;
		pid_location.integral = 0;
		pid_location.err_last = 0;
	}

	/*��������ַ��룬ƫ��ϴ�ʱȥ����������*/
	if(pid_location.err > -LOC_INTEGRAL_START_ERR && pid_location.err < LOC_INTEGRAL_START_ERR)
	{
		pid_location.integral += pid_location.err;  
        /*���ַ�Χ�޶�����ֹ���ֱ���*/
		if(pid_location.integral > LOC_INTEGRAL_MAX_VAL)
		{
			pid_location.integral = LOC_INTEGRAL_MAX_VAL;
		}
		else if(pid_location.integral < -LOC_INTEGRAL_MAX_VAL)
		{
			pid_location.integral = -LOC_INTEGRAL_MAX_VAL;
		}
	}	

	/*PID�㷨ʵ��*/
	pid_location.output_val = pid_location.Kp * pid_location.err +
	                  pid_location.Ki * pid_location.integral +
	                 pid_location.Kd * (pid_location.err - pid_location.err_last);

	/*����*/
	pid_location.err_last = pid_location.err;

	/*���ص�ǰʵ��ֵ*/
	return pid_location.output_val;
}


/**
  * @brief  �ٶ�PID�㷨ʵ��
  * @param  actual_val:ʵ��ֵ
  *	@note 	��
  * @retval ͨ��PID���������
  */
#define SPE_DEAD_ZONE 5.0f /*�ٶȻ�����*/
#define SPE_INTEGRAL_START_ERR 100 /*���ַ���ʱ��Ӧ����Χ*/
#define SPE_INTEGRAL_MAX_VAL 260   /*���ַ�Χ�޶�����ֹ���ֱ���*/
float speed_pid_set(float Kp, float Ki, float Kd, float target_val,float actual_val)
{
	PID pid_speed;
	pid_speed.Kp = Kp;
	pid_speed.Ki = Ki;
	pid_speed.Kd = Kd;
	
	/*����Ŀ��ֵ��ʵ��ֵ�����*/
	pid_speed.err = target_val - actual_val;

	/* �趨�ջ����� */
	if( (pid_speed.err>-SPE_DEAD_ZONE) && (pid_speed.err<SPE_DEAD_ZONE ) )
	{
		pid_speed.err = 0;
		pid_speed.integral = 0;
		pid_speed.err_last = 0;
	}

	if(pid_speed.err > -SPE_INTEGRAL_START_ERR && pid_speed.err < SPE_INTEGRAL_START_ERR)
	{
		pid_speed.integral += pid_speed.err;  
        /*���ַ�Χ�޶�����ֹ���ֱ���*/
		if(pid_speed.integral > SPE_INTEGRAL_MAX_VAL)
		{
			pid_speed.integral = SPE_INTEGRAL_MAX_VAL;
		}
		else if(pid_speed.integral < -SPE_INTEGRAL_MAX_VAL)
		{
			pid_speed.integral = -SPE_INTEGRAL_MAX_VAL;
		}
	}	

	

	pid_speed.output_val = pid_speed.Kp * pid_speed.err +
	                  pid_speed.Ki * pid_speed.integral +
	                  pid_speed.Kd *(pid_speed.err - pid_speed.err_last);

	/*����*/
	pid_speed.err_last = pid_speed.err;

	/*���ص�ǰʵ��ֵ*/
	return pid_speed.output_val;
}

int blance_pid(float Kp, float Ki, float Kd,  float angle )// ��ֱ
{
	PID pid_blance;
	pid_blance.Kp = Kp;
	pid_blance.Ki = Ki;
	pid_blance.Kd = Kd;
	pid_blance.err = angle;
	pid_blance.output_val = pid_blance.Kp * pid_blance.err +
	                  pid_blance.Ki * pid_blance.integral +
	                 pid_blance.Kd * (pid_blance.err - pid_blance.err_last);

	pid_blance.err_last = pid_blance.err;
	return (int)pid_blance.output_val;

}

float blance_pid_hor(float Kp, float Ki, float Kd, float angle)
{
	PID pid_blance;
	pid_blance.Kp = Kp;
	pid_blance.Ki = Ki;
	pid_blance.Kd = Kd;
	pid_blance.err = angle;
	pid_blance.output_val = pid_blance.Kp * pid_blance.err +
	                  pid_blance.Ki * pid_blance.integral +
	                 pid_blance.Kd * (pid_blance.err - pid_blance.err_last);

	pid_blance.err_last = pid_blance.err;
	 
	if (pid_blance.output_val>=150) return 150;
	else if (pid_blance.output_val<=-150) return -150;
	else return (int)pid_blance.output_val;

	
}

