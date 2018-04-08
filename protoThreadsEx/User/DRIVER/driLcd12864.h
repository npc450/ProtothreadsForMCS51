#ifndef _LCD12864_H_
#define _LCD12864_H_

#include <intrins.h>

#define  LCDData P4 //数据P40-P47
sbit     RS=P5^5; 	//并行的指令/数据选择信号, H数据, L命令
sbit     RW=P5^4; 	//并行读写选择信号, H读, L写
sbit     E=P5^3;  	//并行使能端, H有效, L无效
//P07    RST      	//复位引脚  P07不能他用

//应用层-----------------
//从某处写入字符串:void ([列:0-7],[行:0-3],[字符串首地址])
void LCD_WriteStr(unsigned char x,unsigned char y,unsigned char *str);
#define driLcdPrint	LCD_WriteStr

//指定坐标画点:([列:0-127]，[行:0-63]，[颜色:0-1])
void LCD_PutPixel(unsigned char x,unsigned char y,unsigned char color);
#define driLcdPutPixel	LCD_PutPixel

//全屏绘图:void ([全屏图形数组首地址])
void LCD_WriteBmp(unsigned char *puts);

//读取指定位置点的颜色([列:0-127]，[行:0-63])
unsigned char LCD_ReadPixel(unsigned char x,unsigned char y);

//从某处作定长指定颜色的水平线:([列:0-127]，[行:0-63]，[长度]，[颜色:0-1])
void LCD_HoriLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color);

//从某处作定长指定颜色的水平线:([列:0-127]，[行:0-63]，[长度]，[颜色:0-1])
void LCD_VertLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color);

//任意两点间画线
void GUI_Line(unsigned char x0,unsigned char y0,
							unsigned char x1,unsigned char y1,
							unsigned char length,unsigned char color);
							
//指定左上角坐标和右下角坐标画一个指定颜色的矩形
void GUI_Rectangle(	unsigned char x0,unsigned char y0,
										unsigned char x1,unsigned char y1,
										unsigned char color);
										
//八分点画圆函数：([左上角[],[]],[右下角[],[]],[颜色]);
void GUI_Circle_Plot(int x,int y,int xc,int yc,unsigned char color);

//画圆：([圆心坐标[],[]]，[半径]，[颜色])
void GUI_Circle(int xc,int yc,int r,unsigned char color);

//驱动层-----------------
void LCD_InitStr(void);//字符显示初始化
#define driLcdInit	LCD_InitStr

//void ([0-1])：0:清屏；1:填充
void LCD_InitPic(unsigned char mode);
#define driLcdInitPic	LCD_InitPic

//指定光标位置
void LCD_Cursor(unsigned char X,unsigned char Y);

//开机欢迎
void LCD_Welcome(void);

//物理层-----------------
unsigned char	LCD_CheckBusy	(void);//忙检测
void					LCD_WriteData	(unsigned char Data);//写字符
unsigned char	LCD_ReadData	(void);//读字符
void 					LCD_WriteCmd	(unsigned char CmdCode);//写指令
#endif
