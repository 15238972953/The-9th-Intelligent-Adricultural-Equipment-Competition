#ifndef __UART4_H
#define __UART4_H
#define UART4_REC_LEN  			200  	//定义最大接收字节数 200
#define EN_UART4_RX 			1		//使能（1）/禁止（0）串口1接收
extern volatile int a,b;
void uart4_init(unsigned long baudrate);
void UART4_Put_Char(unsigned char DataToSend);
void UART4_Put_String(unsigned char *Str);
void usart4_fangliao(void);
#endif

//------------------End of File----------------------------

