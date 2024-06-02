#ifndef __DIC_H
#define	__DIC_H

void sendcmd(char cmd[]);
	void CopeSerial2Data(unsigned char ucData);
		void CopeSerial1Data(unsigned char ucData);
		int tly(void);
		int TLY_init(void);
//int tly(void);
#include "sys.h"
#endif
