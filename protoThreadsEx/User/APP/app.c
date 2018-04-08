#include "all.h"

int Task0(unsigned char idx);
int Task1(unsigned char idx);
int Task2(unsigned char idx);

void appUserInit(void)
{
	driGpioInit();
	PT_TaskInit(0, Task0);
	PT_TaskInit(1, Task1);
	PT_TaskInit(2, Task2);
}

int Task0(unsigned char idx)
{
	PT_CBEGIN(idx);
	PT_DELAY(10);
	LED1 =~ LED1;
	PT_CEND(idx);
}

int Task1(unsigned char idx)
{
	PT_CBEGIN(idx);
	PT_DELAY(20);
	LED2 =~ LED2;
	PT_CEND(idx);
}

int Task2(unsigned char idx)
{
	PT_CBEGIN(idx);
	PT_DELAY(40);
	LED3 =~ LED3;
	PT_CEND(idx);
}