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
��LCDд���ֽ�����
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
��LCD�ж�������
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
��LCD��д��ָ�����
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
��LCDָ����ʼλ��д��һ���ַ���
*************************************/
void LCD_WriteStr(unsigned char x,unsigned char y,unsigned char *str)
{
	if((y>3)||(x>7))
		return;//���ָ��λ�ò�����ʾ�����ڣ������κ�д��ֱ�ӷ���
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
        LCD��ʼ��
*****************************************/
void LCD_InitStr(void)
{  
	LCD_WriteCmd(0x30);       //ѡ�����ָ�
	LCD_WriteCmd(0x0c);       //����ʾ(���αꡢ������)
	LCD_WriteCmd(0x01);       //�����ʾ�������趨��ַָ��Ϊ00H
	LCD_WriteCmd(0x06);       //ָ�������ϵĶ�ȡ��д��ʱ���趨�α���ƶ�����ָ����ʾ����λ
}
/*****************************************
���LCDȫ����������ģʽModeΪ0����Ϊȫ�����Ϊ��ɫ0�����κ���ʾ��
����Ϊȫ�����Ϊ��ɫ1(ȫ�������ʾ)
******************************************/
void LCD_InitPic(unsigned char mode)
{
	unsigned char x,y,ii;
	unsigned char Temp;
	if(mode%2==0)
		Temp=0x00;
	else
		Temp=0xff;
	LCD_WriteCmd(0x36);//����ָ�� ��ͼ��ʾ
	for(ii=0;ii<9;ii+=8)   
		for(y=0;y<0x20;y++)     
			for(x=0;x<8;x++)
			{ 	
				//EA=0;
				LCD_WriteCmd(y+0x80);        //�е�ַ
				LCD_WriteCmd(x+0x80+ii);     //�е�ַ     
				LCD_WriteData(Temp); //д���� D15��D8 
				LCD_WriteData(Temp); //д���� D7��D0 
				//EA=1;
			}
	LCD_WriteCmd(0x30);
}
/*******************************************
��LCDдͼƬ
********************************************/
void LCD_WriteBmp(unsigned char *puts)
{
	   unsigned int x1=0,x2;
     unsigned char i,j;
     LCD_WriteCmd(0x34);      //8Bit����ָ�,��ʹ��36HҲҪд���� ʹREλ=1
     LCD_WriteCmd(0x36);      //��ͼON,����ָ�����36H���ܿ���ͼ	��ͼ��ʾ
     for(i=0;i<32;i++)        //12864ʵ��Ϊ256x32
     {
       LCD_WriteCmd(0x80|i);  //��λ��
       LCD_WriteCmd(0x80);    //��λ��
       x2=x1;                 //��ס
       for(j=0;j<32;j++)      //256/8=32 byte
       {                      //��λ��ÿ���Զ�����
            if(j/16)          //д128�п�ʼ��255��
    		{
             LCD_WriteData(puts[x2+512]);
        	 x2++;	 
    		}
            else            //д0�п�ʼ��127��
    		{
             LCD_WriteData(puts[x1]);
             x1++;	
    		}
       }    
    		
     }
}
/**************************************
Ϊ�����߼���������õ�����������������ռ����ȡʱ��İ취
***************************************/
unsigned int code LCDMaskTab[]={0x0001,0x0002,0x0004,0x0008,0x0010,0x0020,0x0040,0x0080,
							                  0x0100,0x0200,0x0400,0x0800,0x1000,0x2000,0x4000,0x8000};
/***************************************
��LCDָ������д��һ������,������ɫ�����֣�
0����ף�����ʾ����1����ڣ�����ʾ��
****************************************/
void LCD_PutPixel(unsigned char x,unsigned char y,unsigned char color)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)	 //128��64��
		return;
	color=color%2;
	w=15-x%16;           //ȷ��������ֵĵڶ���λ���в���
	x=x/16;              //ȷ��Ϊһ���ϵĵڼ���

	if(y<32)             //���Ϊ��ҳ
		z=0x80;
	else                 //�������Ϊ��ҳ
		z=0x88;

	y=y%32;
	LCD_WriteCmd(0x36);
	LCD_WriteCmd(y+0x80); //�е�ַ
	LCD_WriteCmd(x+z);    //�е�ַ 
	Temp=LCD_ReadData();//�ȿն�һ��
	Temp=(unsigned int)LCD_ReadData()<<8;//�ٶ�����8λ
	Temp|=(unsigned int)LCD_ReadData();//�ٶ�����8λ
	if(color==1) //���д����ɫΪ1
		Temp|=LCDMaskTab[w];//�ڴ˴����ʵ�ּ���
	else         //���д����ɫΪ0
		Temp&=~LCDMaskTab[w];//�ڴ˴����ʵ�ּ���
	LCD_WriteCmd(y+0x80);   //�е�ַ
	LCD_WriteCmd(x+z);     //�е�ַ
  LCD_WriteData(Temp>>8);//��д���8λ����д���8λ
  LCD_WriteData(Temp&0x00ff);
	LCD_WriteCmd(0x30);
}

/******************************************
��LCDָ�������ȡ������ɫֵ
*******************************************/
unsigned char LCD_ReadPixel(unsigned char x,unsigned char y)
{
	unsigned char z,w;
	unsigned int Temp;
	if(x>=128||y>=64)
		return 0;
	w=15-x%16;//ȷ��������ֵĵڶ���λ���в���
	x=x/16;//ȷ��Ϊһ���ϵĵڼ���

	if(y<32) //���Ϊ��ҳ
		z=0x80;
	else     //�������Ϊ��ҳ
		z=0x88;

	y=y%32;
	//EA=0;
	LCD_WriteCmd(0x36);
	LCD_WriteCmd(y+0x80);  //�е�ַ
	LCD_WriteCmd(x+z);     //�е�ַ 
	Temp=LCD_ReadData();   //�ȿն�һ��
	Temp=(unsigned int)LCD_ReadData()<<8;//�ٶ�����8λ
	Temp|=(unsigned int)LCD_ReadData();//�ٶ�����8λ
	if((Temp&&LCDMaskTab[w])==0)
		return 0;
	else
		return 1;
}

/***************************************
��LCDָ��λ�û�һ������Ϊlength��ָ����ɫ��ˮƽ��
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
��LCDָ��λ�û�һ������Ϊlength��ָ����ɫ�Ĵ�ֱ��
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
��������֮�仭��
****************************************/

void GUI_Line(unsigned char x0,unsigned char y0,unsigned char x1,
              unsigned char y1,unsigned char length,unsigned char color)
{
  char dx;            //ֱ��x���ֵ����
  char dy;            //ֱ��y���ֵ����
  char dx_sym;        //x����������Ϊ-1ʱ��ֵ����Ϊ1����ֵ����
  char dy_sym;        //y����������Ϊ-1ʱ��ֵ����Ϊ1ʱ��ֵ����
  char dx_x2;         //dx*2ֵ���������ڼӿ������ٶ�
  char dy_x2;
  char di;            //���߱���

  if(x0==x1)          //��������ͬ������
  {
     LCD_VertLine(x0,y0,length,color);
	 return;
  }
  if(y0==y1)		  //��������ͬ��ˮƽ��
  {
     LCD_HoriLine(x0,y0,length,color);
	 return;
  }
  dx=x1-x0;
  dy=y1-y0;  //������֮��Ĳ�ֵ
/************�ж���������************/
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

  //����dx,dy����ֵ
  dx=dx_sym*dx;
  dy=dy_sym*dy;
  //dx,dy��2��
  dx_x2=dx*2;
  dy_x2=dy*2;

  if(dx>=dy)//����dx>=dy������x��Ϊ��׼
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
	LCD_PutPixel(x0,y0,color);    //��ʾ���һ����
  }
  else                             //����dx<dy,����y��Ϊ��׼
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
��LCDָ�����Ͻ���������½����껭һ��ָ����ɫ�ľ���
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
* ���ƣ�GUI_Circle_Plot(int x,int y,int xc,int yc,unsigned char color)
* ���ܣ��˷ֵ㻭Բ����
* ��ڲ�����x  ָ������������е�λ��
*           y    ָ������������е�λ��
*          color      ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�������
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
* ���ƣ�GUI_Circle(int xc,int yc,int r,unsigned char color)
* ���ܣ���Բ
* ��ڲ�����xc  Բ�ĵ�������
*           yc    Բ�ĵ�������
*           r  �뾶
*          color      ��ʾ��ɫ(���ںڰ�ɫLCM��Ϊ0ʱ��Ϊ1ʱ��ʾ)
* ���ڲ�������
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
������:д�Ǹ���ַ���͵��ǡ�ͨ��д��ַ���ı���λ��
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97
0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f
0x98,0x99,0x9a,0x9b,0x9c,0x9d,0x9e,0x9f
*****************************************/
void LCD_Cursor(unsigned char X,unsigned char Y)
{
//	LCD_WriteCmd(0x02);	 //��ַ��λ
//	LCD_WriteCmd(0x0f);	  //�����
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
��������
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
