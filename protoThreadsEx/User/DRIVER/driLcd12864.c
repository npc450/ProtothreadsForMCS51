#include "all.h"

unsigned char LCD_CheckBusy(void)
{
	unsigned char Busy;
	LCDData=0xff;
	RS=0;
	RW=1;
	E=1;
	_nop_(); 
	Busy=LCDData&0x80;
	E=0;
	return Busy;
}

/*********************************
向LCD写入字节数据
**********************************/
void LCD_WriteData(unsigned char Data)
{  
	while(LCD_CheckBusy());
	RS=1;
	RW=0;
	E=0;
	_nop_();  
	_nop_(); 
	LCDData=Data;
	E=1;
	_nop_();
	_nop_();
	E=0;
}

/***********************************
从LCD中读出数据
************************************/
unsigned char LCD_ReadData(void)
{
	unsigned char Temp;
	while(LCD_CheckBusy());
 	LCDData=0xff;
 	RS=1;
	RW=1;
	E=1;
	_nop_();
	Temp=LCDData;
	E=0;
	return Temp;
}

/*************************************
想LCD中写入指令代码
**************************************/
void LCD_WriteCmd(unsigned char CmdCode)
{  
	while(LCD_CheckBusy());
   	RS=0;
   	RW=0;
   	E=0;
   	_nop_();  
	  _nop_();
   	LCDData=CmdCode;
   	_nop_(); 
	  _nop_();
   	E=1;
   	_nop_();  
	  _nop_();
   	E=0;
}

/*************************************
向LCD指定起始位置写入一个字符串
*************************************/
void LCD_WriteStr(unsigned char x,unsigned char y,unsigned char *str)
{
	if((y>3)||(x>7))
		return;//如果指定位置不在显示区域内，则不做任何写入直接返回
	//EA=0;
	switch(y)
	{
		case 0:
				LCD_WriteCmd(0x80+x);
				break;
		case 1:
				LCD_WriteCmd(0x90+x);
				break;				
		case 2:
				LCD_WriteCmd(0x88+x);
				break;
		case 3:
				LCD_WriteCmd(0x98+x);
				break;
	}
	while(*str>0)
	{  
		LCD_WriteData(*str);
  	str++;     
	}
}
/****************************************
        LCD初始化
*****************************************/
void LCD_InitStr(void)
{  
	LCD_WriteCmd(0x30);       //选择基本指令集
	LCD_WriteCmd(0x0c);       //开显示(无游标、不反白)
	LCD_WriteCmd(0x01);       //清除显示，并且设定地址指针为00H
	LCD_WriteCmd(0x06);       //指定在资料的读取及写入时，设定游标的移动方向及指定显示的移位
}
/*****************************************
清除LCD全屏，如果清除模式Mode为0，则为全屏清除为颜色0（无任何显示）
否则为全屏清除为颜色1(全屏填充显示)
******************************************/
void LCD_InitPic(unsigned char mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	LCD_WriteCmd(0x36);//扩充指令 绘图显示
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				//EA=0;
				LCD_WriteCmd(y+0x80);        //行地址
				LCD_WriteCmd(x+0x80+ii);     //列地址     
				LCD_WriteData(Temp); //写数据 D15－D8 
				LCD_WriteData(Temp); //写数据 D7－D0 
				//EA=1;
			}
	LCD_WriteCmd(0x30);
}
/*******************************************
向LCD写图片
********************************************/
void LCD_WriteBmp(unsigned char *puts)
{
	   unsigned int x1=0,x2;
     unsigned char i,j;
     LCD_WriteCmd(0x34);      //8Bit扩充指令集,即使是36H也要写两次 使RE位=1
     LCD_WriteCmd(0x36);      //绘图ON,基本指令集里面36H不能开绘图	绘图显示
     for(i=0;i<32;i++)        //12864实际为256x32
     {
       LCD_WriteCmd(0x80|i);  //行位置
       LCD_WriteCmd(0x80);    //列位置
       x2=x1;                 //记住
       for(j=0;j<32;j++)      //256/8=32 byte
       {                      //列位置每行自动增加
            if(j/16)          //写128列开始到255列
    		{
             LCD_WriteData(puts[x2+512]);
        	 x2++;	 
    		}
            else            //写0列开始到127列
    		{
             LCD_WriteData(puts[x1]);
             x1++;	
    		}
       }    
    		
     }
}
/**************************************
为加速逻辑运算而设置的掩码表，这是以牺牲空间而换取时间的办法
***************************************/
unsigned int code LCDMaskTab[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
							                  0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
/***************************************
向LCD指定坐标写入一个象素,象素颜色有两种，
0代表白（无显示），1代表黑（有显示）
****************************************/
void LCD_PutPixel(unsigned char x,unsigned char y,unsigned char color)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)	 //128行64列
		return;
	color=color%2;
	w=15-x%16;           //确定对这个字的第多少位进行操作
	x=x/16;              //确定为一行上的第几字

	if(y<32)             //如果为上页
		z=0x80;
	else                 //否则如果为下页
		z=0x88;

	y=y%32;
	LCD_WriteCmd(0x36);
	LCD_WriteCmd(y+0x80); //行地址
	LCD_WriteCmd(x+z);    //列地址 
	Temp=LCD_ReadData();//先空读一次
	Temp=(unsigned int)LCD_ReadData()<<8;//再读出高8位
	Temp|=(unsigned int)LCD_ReadData();//再读出低8位
	if(color==1) //如果写入颜色为1
		Temp|=LCDMaskTab[w];//在此处查表实现加速
	else         //如果写入颜色为0
		Temp&=~LCDMaskTab[w];//在此处查表实现加速
	LCD_WriteCmd(y+0x80);   //行地址
	LCD_WriteCmd(x+z);     //列地址
  LCD_WriteData(Temp>>8);//先写入高8位，再写入低8位
  LCD_WriteData(Temp&0x00ff);
	LCD_WriteCmd(0x30);
}

/******************************************
从LCD指定坐标读取象素颜色值
*******************************************/
unsigned char LCD_ReadPixel(unsigned char x,unsigned char y)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return 0;
	w=15-x%16;//确定对这个字的第多少位进行操作
	x=x/16;//确定为一行上的第几字

	if(y<32) //如果为上页
		z=0x80;
	else     //否则如果为下页
		z=0x88;

	y=y%32;
	//EA=0;
	LCD_WriteCmd(0x36);
	LCD_WriteCmd(y+0x80);  //行地址
	LCD_WriteCmd(x+z);     //列地址 
	Temp=LCD_ReadData();   //先空读一次
	Temp=(unsigned int)LCD_ReadData()<<8;//再读出高8位
	Temp|=(unsigned int)LCD_ReadData();//再读出低8位
	if((Temp&&LCDMaskTab[w])==0)
		return 0;
	else
		return 1;
}

/***************************************
向LCD指定位置画一条长度为length的指定颜色的水平线
****************************************/
void LCD_HoriLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color)
{
	unsigned char i;
	if(length==0)
		return;
	for(i=0;i<length;i++)
	{
		LCD_PutPixel(x+i,y,color);
	}
}

/***************************************
向LCD指定位置画一条长度为length的指定颜色的垂直线
****************************************/
void LCD_VertLine(unsigned char x,unsigned char y,unsigned char length,unsigned char color)
{
	unsigned char i;
	if(length==0)
		return;
	for(i=0;i<length;i++)
	{
		LCD_PutPixel(x,y+i,color);
	}
}
/***************************************
任意两点之间画线
****************************************/

void GUI_Line(unsigned char x0,unsigned char y0,unsigned char x1,
              unsigned char y1,unsigned char length,unsigned char color)
{
  char dx;            //直线x轴差值变量
  char dy;            //直线y轴差值变量
  char dx_sym;        //x轴增长方向，为-1时减值方向，为1是增值方向
  char dy_sym;        //y轴增长方向，为-1时减值方向，为1时增值方向
  char dx_x2;         //dx*2值变量，用于加快运算速度
  char dy_x2;
  char di;            //决策变量

  if(x0==x1)          //横坐标相同画垂线
  {
     LCD_VertLine(x0,y0,length,color);
	 return;
  }
  if(y0==y1)		  //纵坐标相同画水平线
  {
     LCD_HoriLine(x0,y0,length,color);
	 return;
  }
  dx=x1-x0;
  dy=y1-y0;  //求两点之间的差值
/************判断增长方向************/
  if(dx>0) 
    dx_sym=1;                                          
  else
  {
	  dx_sym=-1; 
  }
  if(dy>0) 
    dy_sym=1;
  else
  {
	  dy_sym=-1;    
  }

  //计算dx,dy绝对值
  dx=dx_sym*dx;
  dy=dy_sym*dy;
  //dx,dy的2倍
  dx_x2=dx*2;
  dy_x2=dy*2;

  if(dx>=dy)//对于dx>=dy，则以x轴为基准
  {
	di=dy_x2-dx;
	while(x0!=x1)
	{
	   LCD_PutPixel(x0,y0,color);
	   x0+=dx_sym;
	   if(di<0)di+=dy_x2;
	   else
	   {
	     di+=dy_x2-dx_x2;
		   y0+=dy_sym;
	   }
    }
	LCD_PutPixel(x0,y0,color);    //显示最后一个点
  }
  else                             //对于dx<dy,则以y轴为基准
  {
     di=dx_x2-dy;
	 while(y0!=y1)
	 {
	   LCD_PutPixel(x0,y0,color);
		 y0+=dy_sym;
     if(di<0)
		   di+=dx_x2;
		 else
			{
	      di+=dx_x2-dy_x2;
			  x0=dx_sym;
			}
   }
  LCD_PutPixel(x0,y0,color);
  }

}

/*******************************************
向LCD指定左上角坐标和右下角坐标画一个指定颜色的矩形
********************************************/
void GUI_Rectangle(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char color)
{
	unsigned char Temp;
	if(x0>x1)
	{
		Temp=x0;
		x0=x1;
		x1=Temp;
	}	
	if(y0>y1)
	{
		Temp=y0;
		y0=y1;
		y1=Temp;
	}
	LCD_VertLine(x0,y0,y1-y0+1,color);
	LCD_VertLine(x1,y0,y1-y0+1,color);
	LCD_HoriLine(x0,y0,x1-x0+1,color);
	LCD_HoriLine(x0,y1,x1-x0+1,color);	
}

/*********
* 名称：GUI_Circle_Plot(int x,int y,int xc,int yc,unsigned char color)
* 功能：八分点画圆函数
* 入口参数：x  指定线起点所在行的位置
*           y    指定线起点所在列的位置
*          color      显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：无
****************************************************************************/
void GUI_Circle_Plot(int x,int y,int xc,int yc,unsigned char color)
{
    LCD_PutPixel(xc+x,yc+y,color);
    LCD_PutPixel(xc+x,yc-y,color);
    LCD_PutPixel(xc-x,yc+y,color);
    LCD_PutPixel(xc-x,yc-y,color);
    LCD_PutPixel(xc+y,yc+x,color);
    LCD_PutPixel(xc+y,yc-x,color);
    LCD_PutPixel(xc-y,yc+x,color);
    LCD_PutPixel(xc-y,yc-x,color);
}
/****************************************************************************
* 名称：GUI_Circle(int xc,int yc,int r,unsigned char color)
* 功能：画圆
* 入口参数：xc  圆心的行坐标
*           yc    圆心的列坐标
*           r  半径
*          color      显示颜色(对于黑白色LCM，为0时灭，为1时显示)
* 出口参数：无
****************************************************************************/
void GUI_Circle(int xc,int yc,int r,unsigned char color)
{
    int x,y,d;
    y = r;
    d = 3 - (r + r);
    x = 0;
    while(x <= y)
    {
        GUI_Circle_Plot(x,y,xc,yc,color);
        if(d < 0)
            d += (x + x + x + x) + 6;
        else
        {
            d+=((x - y) + (x - y) + (x - y) + (x - y)) + 10;
            y = y - 1;
        }
        x = x + 1;
    }
}

/****************************************
光标控制:写那个地址光标就到那。通过写地址来改变光标位置
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f
*****************************************/
void LCD_Cursor(unsigned char X,unsigned char Y)
{
//	LCD_WriteCmd(0x02);	 //地址归位
//	LCD_WriteCmd(0x0f);	  //开光标
	if(Y==0)
		LCD_WriteCmd(0x80+X);
	if(Y==1)
		LCD_WriteCmd(0x90+X);
	if(Y==2)
		LCD_WriteCmd(0x88+X);
	if(Y==3)
		LCD_WriteCmd(0x98+X);


}
/****************************************
开机界面
*****************************************/
void LCD_Welcome(void)
{
	GUI_Rectangle(0,0,127,63,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(4,4,123,60,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(8,8,119,56,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(12,12,115,52,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(16,16,111,48,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();					  
	GUI_Rectangle(20,20,107,44,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(24,24,103,40,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(28,28,99,36,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();
	GUI_Rectangle(32,32,95,32,1);
	_nop_();_nop_();_nop_();_nop_();_nop_();	
}
