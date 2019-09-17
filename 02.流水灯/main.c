/**********************实验测试程序************************
*  平台：BST-V51 + Keil U5 + STC89C52RC
*  名称：流水灯实验
*  日期：2019-9-7
*  姓名：ZhangHJ
***********************************************************/
#include<reg52.h>
#include<intrins.h>
#define ON 0
#define OFF 1
#define DELAYTIME 100		//延时时间

//分别定义8位LED位
sbit led0 = P1^0;
sbit led1 = P1^1;
sbit led2 = P1^2;
sbit led3 = P1^3;
sbit led4 = P1^4;
sbit led5 = P1^5;
sbit led6 = P1^6;
sbit led7 = P1^7;


/**********************************************************
*  函数名称：低精度延时函数
*  日期：2019-9-7
*  姓名：ZhangHJ
*  说明：嵌套循环延时
***********************************************************/
void delay_ms(unsigned int mstime)
{
	int i,j;
	for(i=mstime; i>=0; i--)
		for(j=112; j>=0; j--);
}



/**********************************************************
*  函数名称：位操作流水灯函数
*  日期：2019-9-7
*  姓名：ZhangHJ
*  说明：依次分别点亮并延时8个LED
***********************************************************/
liushui_fun1(void)
{
	while(1)
	{
		led0 = ON;
		delay_ms(DELAYTIME);
		led0 = OFF;
		
		led1 = ON;
		delay_ms(DELAYTIME);
		led1 = OFF;
		
		led2 = ON;
		delay_ms(DELAYTIME);
		led2 = OFF;
		
		led3 = ON;
		delay_ms(DELAYTIME);
		led3 = OFF;

		led4 = ON;
		delay_ms(DELAYTIME);
		led4 = OFF;
		
		led5 = ON;
		delay_ms(DELAYTIME);
		led5 = OFF;

		led6 = ON;
		delay_ms(DELAYTIME);
		led6 = OFF;
		
		led7 = ON;
		delay_ms(DELAYTIME);
		led7 = OFF;
	}
}



/**********************************************************
*  函数名称：移位方式流水灯函数
*  日期：2019-9-7
*  姓名：ZhangHJ
*  说明：P1循环移位8次,然后使P1初始化并重新循环
***********************************************************/
liushui_fun2()
{
	int i = 0;
	while(1)
	{
		P1 = 0xfe;				//1111 1110 LED0亮
		for(i=0; i<8; i++)
		{
			delay_ms(DELAYTIME);
			P1 <<= 1;				//P1左移一位
			P1 = P1|0x01;		//保证P1最后一位为 1
		}
	}
}



/**********************************************************
*  函数名称：循环移位方式流水灯函数
*  日期：2019-9-7
*  姓名：ZhangHJ
*  说明：使用_crol_函数,使P1循环移位(不会存在溢出现象)
***********************************************************/
liushui_fun3()
{
	P1 = 0Xfe;							//初始化LED0亮
	while(1)
	{
		delay_ms(DELAYTIME);
		P1 = _crol_(P1,1);		//P1循环左移1位
	}
}

int main()
{
	//分别对三种方式进行测试
	//liushui_fun1();
	//liushui_fun2();
	liushui_fun3();
	return 0;
}
