#ifndef Key_H
#define Key_H
#include "sys.h"

#define KEY1 		PEin(0)   	
#define KEY2 		PEin(1)		
#define KEY3		PEin(2)		
#define KEY4	  PEin(3)		
void KEY_Init(void);
u8 KEY_Scan(u8 mode);
void keyboard(void);
void choose_servo(void);
#endif
