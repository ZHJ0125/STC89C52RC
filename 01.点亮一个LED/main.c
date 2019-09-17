#include<reg52.h>

sbit led0 = P1^0;			//定义8个特殊位变量，分别表示8个LED
sbit led1 = P1^1;
sbit led2 = P1^2;
sbit led3 = P1^3;
sbit led4 = P1^4;
sbit led5 = P1^5;
sbit led6 = P1^6;
sbit led7 = P1^7;

int main()
{
		while(1)
		{
				P1 = 0xe6;	//位操作，同时改变八位输出 1110 0110  LED0、LED3、LED4亮
				led6 = 0;		//LED6亮
				led7 = 0;		//LED7亮
		}
}