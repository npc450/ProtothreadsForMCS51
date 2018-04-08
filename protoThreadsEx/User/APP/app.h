#ifndef APP_H
#define APP_H

sbit LED1 = P5^0;
sbit LED2 = P5^1;
sbit LED3 = P5^2;
sbit KEY1 = P2^0;
sbit KEY2 = P2^1;
sbit KEY3 = P2^2;
sbit KEY4 = P2^3;

void appUserInit(void);

#endif