#ifndef _LCD12864_H_
#define _LCD12864_H_

#include <intrins.h>

#define  LCDData P4 //����P40-P47
sbit     RS=P5^5; 	//���е�ָ��/����ѡ���ź�, H����, L����
sbit     RW=P5^4; 	//���ж�дѡ���ź�, H��, Lд
sbit     E=P5^3;  	//����ʹ�ܶ�, H��Ч, L��Ч
//P07    RST      	//��λ����  P07��������

//Ӧ�ò�-----------------
//��ĳ��д���ַ���:void ([��:0-7],[��:0-3],[�ַ����׵�ַ])
void LCD_WriteStr(unsigned char x,unsigned char y,unsigned char *str);
#define driLcdPrint	LCD_WriteStr

//ָ�����껭��:([��:0-127]��[��:0-63]��[��ɫ:0-1])
void LCD_PutPixel(unsigned char x,unsigned char y,unsigned char color);
#define driLcdPutPixel	LCD_PutPixel

//ȫ����ͼ:void ([ȫ��ͼ�������׵�ַ])
void LCD_WriteBmp(unsigned char *puts);

//��ȡָ��λ�õ����ɫ([��:0-127]��[��:0-63])
unsigned char LCD_ReadPixel(unsigned char x,unsigned char y);

//��ĳ��������ָ����ɫ��ˮƽ��:([��:0-127]��[��:0-63]��[����]��[��ɫ:0-1])
void LCD_HoriLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color);

//��ĳ��������ָ����ɫ��ˮƽ��:([��:0-127]��[��:0-63]��[����]��[��ɫ:0-1])
void LCD_VertLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color);

//��������仭��
void GUI_Line(unsigned char x0,unsigned char y0,
							unsigned char x1,unsigned char y1,
							unsigned char length,unsigned char color);
							
//ָ�����Ͻ���������½����껭һ��ָ����ɫ�ľ���
void GUI_Rectangle(	unsigned char x0,unsigned char y0,
										unsigned char x1,unsigned char y1,
										unsigned char color);
										
//�˷ֵ㻭Բ������([���Ͻ�[],[]],[���½�[],[]],[��ɫ]);
void GUI_Circle_Plot(int x,int y,int xc,int yc,unsigned char color);

//��Բ��([Բ������[],[]]��[�뾶]��[��ɫ])
void GUI_Circle(int xc,int yc,int r,unsigned char color);

//������-----------------
void LCD_InitStr(void);//�ַ���ʾ��ʼ��
#define driLcdInit	LCD_InitStr

//void ([0-1])��0:������1:���
void LCD_InitPic(unsigned char mode);
#define driLcdInitPic	LCD_InitPic

//ָ�����λ��
void LCD_Cursor(unsigned char X,unsigned char Y);

//������ӭ
void LCD_Welcome(void);

//�����-----------------
unsigned char	LCD_CheckBusy	(void);//æ���
void					LCD_WriteData	(unsigned char Data);//д�ַ�
unsigned char	LCD_ReadData	(void);//���ַ�
void 					LCD_WriteCmd	(unsigned char CmdCode);//дָ��
#endif
