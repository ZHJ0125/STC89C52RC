#include<reg52.h>

#define ON 1
#define OFF 0

sbit LED = P0^0;

int main()
{
		while(1)
		{
				LED = ON;
		}
}