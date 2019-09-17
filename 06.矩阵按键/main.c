/**********************实验测试程序************************
*  平台：BST-V51 + Keil U5 + STC89C52RC
*  名称：矩阵按键实验
*  日期：2019-9-8
*  姓名：ZhangHJ
*  说明：矩阵按键控制数码管
*				 效果是实现4X4矩阵的数码管显示
***********************************************************/
#include<reg52.h>
#include"bstv51.h"

unsigned char led[]={  //按键字符数组
                0x3F,  //"0"
                0x06,  //"1"
                0x5B,  //"2"
                0x4F,  //"3"
                0x66,  //"4"
                0x6D,  //"5"
                0x7D,  //"6"
                0x07,  //"7"
                0x7F,  //"8"
                0x6F,  //"9"
                0x77,  //"A"
                0x7C,  //"B"
                0x39,  //"C"
                0x5E,  //"D"
                0x79,  //"E"
                0x71,  //"F"
                0x76,  //"H"
                0x38,  //"L"
                0x37,  //"n"
                0x3E,  //"u"
                0x73,  //"P"
                0x5C,  //"o"
                0x40,  //"-"
                0x00,  //熄灭
								};

								
/**********************************************************
*  函数名称：数码管显示函数
*  日期：2019-9-8
*  姓名：ZhangHJ
*  说明：参数num表示需要显示的字符
*				 只进行了段选控制,没有进行位选控制
***********************************************************/
void display(int num)
{	
	P0 = led[num];
	DU = 1;
	DU = 0;
	FM = 0;
	delay_ms(60);
	FM = 1;
}



/**********************************************************
*  函数名称：矩阵键盘扫描函数
*  日期：2019-9-8
*  姓名：ZhangHJ
*  说明：函数分为四个部分,分别对应四行键盘的扫描
*				 
***********************************************************/
void keyscan4x4()
{
	uint temp,key;
	//////////////第一行扫描////////////////////
	P3 = 0xfe;								//1111 1110 第一行扫描写高电平
	temp = P3;								//设置临时变量存储P3寄存器数据
	temp = temp&0xf0;					//1111 1110 & 1111 0000 相当于高四位不变,低四位变0(屏蔽低四位数据)
	if(temp != 0xf0)					//数据发生改变,说明键盘可能被按下
	{
		delay_ms(10);						//延时消抖
		temp = P3;							//重新读入P3数据
		temp = temp&0xf0;
		if(temp != 0xf0)
		{
			temp = P3;						//重新读入数据,考虑低四位数据
			switch(temp){
				case 0xee:
					key = 0; break;		//1110 1110
				case 0xde:
					key = 1; break;		//1101 1110
				case 0xbe:
					key = 2; break;		//1011 1110
				case 0x7e:
					key = 3; break;		//0111 1110
			}
			while(temp != 0xf0)		//键盘被按下时,卡死在这个循环里,直到松开键盘,才执行接下来的程序
			{
				temp = P3;
				temp = temp&0xf0;
			}
			display(key);					//显示读到的数据
		}
	}
	
	//////////////第二行扫描////////////////////
	P3 = 0xfd;		//1111 1101
	temp = P3;
	temp = temp&0xf0;		//1111 1101 & 1111 0000 相当于高四位不变,低四位变0
	if(temp != 0xf0)
	{
		delay_ms(10);
		temp = P3;
		temp = temp&0xf0;
		if(temp != 0xf0)
		{
			temp = P3;
			switch(temp){
				case 0xed:
					key = 4; break;		//1110 1101
				case 0xdd:
					key = 5; break;		//1101 1101
				case 0xbd:
					key = 6; break;		//1011 1101
				case 0x7d:
					key = 7; break;		//0111 1101
			}
			while(temp != 0xf0)
			{
				temp = P3;
				temp = temp&0xf0;
			}
			display(key);
		}
	}
	
	//////////////第三行扫描////////////////////
	P3 = 0xfb;		//1111 1011
	temp = P3;
	temp = temp&0xf0;		//1111 1011 & 1111 0000 相当于高四位不变,低四位变0
	if(temp != 0xf0)
	{
		delay_ms(10);
		temp = P3;
		temp = temp&0xf0;
		if(temp != 0xf0)
		{
			temp = P3;
			switch(temp){
				case 0xeb:
					key = 8; break;		//1110 1011
				case 0xdb:
					key = 9; break;		//1101 1011
				case 0xbb:
					key = 10; break;		//1011 1011
				case 0x7b:
					key = 11; break;		//0111 1011
			}
			while(temp != 0xf0)
			{
				temp = P3;
				temp = temp&0xf0;
			}
			display(key);
		}
	}
	
	//////////////第四行扫描////////////////////
	P3 = 0xf7;		//1111 0111
	temp = P3;
	temp = temp&0xf0;		//1111 0111 & 1111 0000 相当于高四位不变,低四位变0
	if(temp != 0xf0)
	{
		delay_ms(10);
		temp = P3;
		temp = temp&0xf0;
		if(temp != 0xf0)
		{
			temp = P3;
			switch(temp){
				case 0xe7:
					key = 12; break;		//1110 0111
				case 0xd7:
					key = 13; break;		//1101 0111
				case 0xb7:
					key = 14; break;		//1011 0111
				case 0x77:
					key = 15; break;		//0111 0111
			}
			while(temp != 0xf0)
			{
				temp = P3;
				temp = temp&0xf0;
			}
			display(key);
		}
	}
}


void main()
{
	bstv51_init();	//板子初始化
	P0 = 0x00;			//位选8位,相当于四位全亮
	WE = 1;
	WE = 0;
	
	while(1)
	{
		keyscan4x4();	//进行按键扫描
	}
}
