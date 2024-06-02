#ifndef Motor_H
#define Motor_H
#include "sys.h"

#define  AIN PBout(4)
#define  BIN PBout(5)
#define  CIN PEout(4)
#define  DIN PEout(7)

void motor_Init(void)	;
void motion(int PWMA,int PWMB,int PWMC,int PWMD);

#endif
