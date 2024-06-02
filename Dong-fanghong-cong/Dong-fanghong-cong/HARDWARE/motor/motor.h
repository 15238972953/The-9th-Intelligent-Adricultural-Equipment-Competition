#ifndef Motor_H
#define Motor_H
#include "sys.h"

#define  AIN1 PDout(14)
#define  AIN2 PDout(4)
#define  BIN1 PDout(2)
#define  BIN2 PDout(0)
#define  CIN1 PEout(4)
#define  CIN2 PEout(7)
#define  DIN1 PDout(10)
#define  DIN2 PDout(11)

void motor_Init(void)	;
void motion(int PWMA,int PWMB,int PWMC,int PWMD);

#endif
