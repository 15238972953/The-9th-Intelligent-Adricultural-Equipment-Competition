#ifndef __UART4_H
#define __UART4_H
#define UART4_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_UART4_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
extern volatile int a,b;
void uart4_init(unsigned long baudrate);
void UART4_Put_Char(unsigned char DataToSend);
void UART4_Put_String(unsigned char *Str);
void usart4_fangliao(void);
#endif

//------------------End of File----------------------------

