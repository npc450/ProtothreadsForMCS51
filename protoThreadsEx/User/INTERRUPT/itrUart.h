#ifndef __UART_H_
#define __UART_H_

#define S2RI  0x01//S2CON.0
#define S2TI  0x02//S2CON.1
#define S2RB8 0x04//S2CON.2
#define S2TB8 0x08//S2CON.3

#define S3RI  0x01//S3CON.0
#define S3TI  0x02//S3CON.1
#define S3RB8 0x04//S3CON.2
#define S3TB8 0x08//S3CON.3

#define S4RI  0x01//S3CON.0
#define S4TI  0x02//S3CON.1
#define S4RB8 0x04//S3CON.2
#define S4TB8 0x08//S3CON.3

#define ESn 0x19//此处见手册P546
#define ES2 0x01//IE2.0
#define ES3 0x08//IE2.3
#define ES4 0x10//IE2.4
#define OPEN ES=1;IE2|=ESn;		//开启全串口接收
#define CLOSE ES=0;IE2&=~ESn;	//关闭全串口接收

//COM1:Txd:P3.1;Rxd:P3.0
//COM2:Txd:P1.1;Rxd:P1.0
//COM3:Txd:P0.1;Rxd:P0.0
//COM4:Txd:P0.3;Rxd:P0.2

void itrUartInit(void);

void uartPrintChar1(unsigned char ch);
void uartPrintStr1(char *s);
void uartPrintChar2(unsigned char ch);
void uartPrintStr2(char *s);
void uartPrintChar3(unsigned char ch);
void uartPrintStr3(char *s);
void uartPrintChar4(unsigned char ch);
void uartPrintStr4(char *s);

#define	uartStr(index, str)		uartPrintStr##index  (str)
#define	uartChar(index, ch)		uartPrintChar##index (ch)

#endif
