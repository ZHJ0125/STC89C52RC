/**********************实验测试程序************************
*  平台：BST-V51 + Keil U5 + STC89C52RC
*  名称：串口通信实验
*  日期：2019-9-8
*  姓名：ZhangHJ
*  说明：串口返回接收到的数据
***********************************************************/
#include<reg52.h>
#include"bstv51.h"



/**********************************************************
*  函数名称：串口通信初始化函数
*  日期：2019-9-8
*  姓名：ZhangHJ
*  说明：进行串口通信的初始化配置,包括
* 			 计时器模式寄存器(TMOD)配置
* 			 串行口控制寄存器(SCON)配置
*				 波特率与电源管理寄存器(PCON)配置
*				 中断允许寄存器(IE)配置
*				 计时器控制寄存器(TCON)配置
***********************************************************/
void UartConfig()
{
	TMOD = 0x20;			//设置计时器1的工作模式为2
	TH1 = 0xfd;				//设置计时器高八位数值
	TL1 = 0xfd;				//设置计时器低八位数值
	PCON = 0x00;			//设置波特率0x00为不加倍;0x80加倍
	SCON = 0x50;			//设置串行口控制寄存器工作模式为1,ERN=1表示允许接收数据
	EA = 1;						//开启整体中断
	ES = 1;						//允许串行中断
	TR1 = 1;					//T1计时器开始运行
}

int main()
{
	bstv51_init();
	UartConfig();			//串口通信初始化
	while(1);
}

void uart_test() interrupt 4
{
	uchar uartdata;		//数据缓存
	uartdata = 	SBUF;	//SBUF是接收到的串口数据
	while(!RI);				//等待接受完成
	RI = 0;						//接受标志位清零
	SBUF = uartdata;	//将接收到的数据发送出去
	while(!TI);				//等待发送数据完成
	TI = 0;						//发送标志位清零
}