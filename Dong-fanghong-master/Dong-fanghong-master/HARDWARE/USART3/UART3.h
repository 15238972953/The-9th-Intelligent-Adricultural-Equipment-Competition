#ifndef __UART3_H
#define __UART3_H
#define USART3_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART3_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
void uart3_init(unsigned long baudrate);
void UART3_Put_Char(unsigned char DataToSend);
void UART3_Put_String(unsigned char *Str);
void usart3_fangliao(void);
#endif

//------------------End of File----------------------------

