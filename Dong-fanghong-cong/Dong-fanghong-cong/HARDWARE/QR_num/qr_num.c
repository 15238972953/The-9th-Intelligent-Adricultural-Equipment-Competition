#include "qr_num.h"
#include "variable.h"
#include "UART1.h"
//int code=0;
//extern char QR_code;
//void get_QR(void)
//{
//	while(1)
//	{

//		if(	QR_code=='1' ||\
//				QR_code=='2' ||\
//				QR_code=='3' ||\
//				QR_code=='4' ||\
//				QR_code=='5' )
//		USART_Cmd(USART1, DISABLE);
//		switch(QR_code)
//		{
//			case '1' :code=1;break;
//			case '2' :code=2;break;
//			case '3' :code=3;break;
//			case '4' :code=4;break;
//			case '5' :code=5;break;
//		}
//		//printf("%d",QR_code);
//		OLED_ShowIntNum(0,10,code ,5,8,1);	
//		if(code!=0) break;
//		
//	}
//}