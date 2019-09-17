/**********************BST-V51实验开发板例程************************
*  平台：BST-V51 + Keil U3 + STC89C52
*  名称：18B20温度传感器模块测试
*  公司：深圳市亚博软件开发有限公司
*  修改日期：2019-9
*  晶振:11.0592MHZ
*  说明：免费开源，不提供源代码分析.
*				 ZhangHJ于2019-9-11进行测试,并修改部分代码及注释
******************************************************************/
#include <reg52.h>
#define uchar unsigned char
#define uint unsigned int
sbit DS = P2^2;					//define interface
uint temp;							//variable of temperature
uchar flag1;						//sign of the result positive or negative
sbit dula = P2^6;				//定义数码管段选端口
sbit wela = P2^7;				//定义数码管位选端口
//定义数码管字符集
unsigned char code table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
//定义数码管码表
unsigned char code table1[]={0xbf,0x86,0xdb,0xcf,0xe6,0xed,0xfd,0x87,0xff,0xef};



/**********************************************************
*  函数名称：延时函数
*  修改日期：2019-9-9
*  修改人：ZhangHJ
*  说明：低精度延时函数
***********************************************************/
void delay(uint count)	//delay function
{
  uint i;
  while(count)
  {
    i=200;
    while(i>0)
    i--;
    count--;
  }
}



/**********************************************************
*  函数名称：发送复位和初始化命令函数 dsinit
*  修改日期：2019-9-9
*  修改人：ZhangHJ
*  说明：1. 对于单片机: 单片机首先发出480-960us的低电平脉冲,
*				 		释放总线为高电平(上拉电阻拉高),在随后的480us进行检测,
*				 		如果出现低电平，说明器件应答正常.B
*				 2. 对于DS18B20: 上电后就检测是否有480/960us的低电平脉冲,
*						如果有低电平,在总线释放之后，等待15-60us,
*						将电平拉低60-240us,告诉主机已经准备好.
***********************************************************/
void dsreset(void)				//send reset and initialization command
{
  uint i;
  DS = 0;								//先将端口拉低
  i=103;								//维持低电平状态480us~960us
  while(i>0)i--;
  DS = 1;								//然后释放总线(将总线拉高),若DS18B20做出反应,将会将在15us~60us后将总线拉低
  i = 4;								//15us~60us等待
  while(i>0)i--;
//	while(DS);
//	i = 0;
//	while(DS)							//在DS高电平时等待
//	{
//		i++;
//		if(i > 5000)				//等待时间大于60us,说明响应失败
//		{
//			reutrn 0;
//		}
//	}
//	return 1;
}



/**********************************************************
*  函数名称：读1bit数据函数
*  修改日期：2019-9-9
*  修改人：ZhangHJ
*  说明：1.首先单片机端口拉低 1 us (i++),
*				 2.然后释放总线,拉高总线,
*				 3.等待几位秒，是为了让 DS18B20 数据稳定,
*				 4.在15 us 内读取DS数据,
*				 5.接下来进行延时等待采样周期完成。
*				 详见DS18B20资料2.8.3.1读/写时间片
***********************************************************/
bit tmpreadbit(void)		//read a bit data
{
	uint i;
	bit dat;							//定义位数据 (dat = 0 or 1)
	DS = 0;								//先将端口拉低
	i++;									//延时 1us
	DS = 1;								//再将端口拉高
	i++;i++;							//等待DS数据稳定
	dat = DS;							//数据传输
	i=8;while(i>0)i--;		//等待数据采样周期完成
	return (dat);
}



/**********************************************************
*  函数名称：读1Byte数据函数
*  修改日期：2019-9-9
*  修改人：ZhangHJ
*  说明：1.首先初始化字节数据变量dat为 0
*				 2.循环 8 次,调用tmpreadbit函数,读 8bit 数据
*				 3.读出的数据暂存到 j ,之后进行移位和按位或操作
*				 4.效果是 j 先读入的数据,放到了dat的低位,读8次正好是1byte.
*				 5.最后返回读到的字节数据
***********************************************************/
uchar tmpread(void)					//read a byte date
{
	uchar i,j,dat;
  dat = 0;									//初始化数据变量为 0
  for(i=1;i<=8;i++)					//循环 8 次,调用tmpreadbit函数,读 8bit 数据
  {
    j = tmpreadbit();				//读出的数据暂存到 j
    dat = (j<<7)|(dat>>1);	//效果是 j 先读入的数据,放到了dat的低位,读8次正好是1byte.
  }
  return(dat);							//返回读到的字节
}



/**********************************************************
*  函数名称：写入1Byte数据函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：1.将对待写入数据dat进行位操作,将dat末位数值赋值给位数据testb
*				 2.通过判断testb得到写 0 还是写 1
*				 3.若是写 0 操作,将 DS 拉低,进行<60us的延时,再将DS拉高,进行>1us的延时
*				 4.若是写 1 操作,将 DS 拉低,进行>1us的延时,再将DS拉高,进行<60us的延时
*				 5.循环执行2、3、4操作8次,写入1字节数据
*				 详见18B20资料“2.8.3.1读/写时间片”章节
***********************************************************/
void tmpwritebyte(uchar dat)   //write a byte to ds18b20
{
  uint i;
  uchar j;
  bit testb;
  for(j=1;j<=8;j++)
  {
    testb = dat&0x01;
    dat = dat>>1;
    if(testb)     						//write 1
    {
      DS=0;
      i++;i++;
      DS=1;
      i=8;while(i>0)i--;
    }
    else
    {
      DS = 0;       					//write 0
      i=8;while(i>0)i--;
      DS=1;
      i++;i++;
    }
  }
}



/**********************************************************
*  函数名称：18B20温度转换函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：1.首先进行18B20初始化
*				 2.进行适当延时
*				 3.发送跳过光刻ROM指令
*				 4.发送RAM指令,进行温度转换
*				 详见18B20资料“2.8.3.1存储器操作命令”章节
***********************************************************/
void tmpchange(void)					//DS18B20 begin change
{
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);					//address all drivers on bus
  tmpwritebyte(0x44);					//initiates a single temperature conversion
  //delay(100);								//not wait to change finish
}



/**********************************************************
*  函数名称：18B20温度转换完整过程函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：1.首先进行18B20初始化
*				 2.进行适当延时
*				 3.发送跳过光刻ROM指令
*				 4.发送RAM指令,进行温度转换
*				 5.读取两个8位数据,放到16位寄存器 temp 中
*				 6.将读取到的二进制数据(默认为正数),转换为十进制数据
*				 7.返回温度数据
*				 详见18B20资料“2.8.3.1存储器操作命令”章节
***********************************************************/
uint tmp()										//get the temperature
{
  float tt;
  uchar a,b;
  dsreset();
  delay(1);
  tmpwritebyte(0xcc);					//读暂存寄存器
  tmpwritebyte(0xbe);
  a=tmpread();
  b=tmpread();
  temp=b;
  temp<<=8;										//two byte compose a int variable
  temp=temp|a;
  tt=temp*0.0625;
  temp=tt*10+0.5;
  return temp;
}



/**********************************************************
*  函数名称：读取ROM函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：使单片机读取DS18B20产品的序列号,暂时没有用到
*				 详见18B20资料“2.8.2.2ROM操作命令”章节
***********************************************************/
//void readrom()								//read the serial
//{
//  uchar sn1,sn2;
//  dsreset();
//  delay(1);
//  tmpwritebyte(0x33);
//  sn1=tmpread();
//  sn2=tmpread();
//}



/**********************************************************
*  函数名称：10ms延时函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：延时10ms
*				 详见18B20资料“2.8.2.2ROM操作命令”章节
***********************************************************/
//void delay10ms()							//delay 10ms
//{
//	uchar a,b;
//	for(a=10;a>0;a--)
//		for(b=60;b>0;b--);
//}



/**********************************************************
*  函数名称：数码管温度数据显示函数
*  修改日期：2019-9-11
*  修改人：ZhangHJ
*  说明：1.temp表示需要显示的温度数值(百位数值)
*				 2.A1、A2、A3分别了百位、十位、个位数值
*				 3.控制段选信号dula和位选信号wela,以使数码管显示
***********************************************************/
void display(uint temp)				//显示程序
{
   uchar A1,A2,A2t,A3,ser;
   ser=temp/10;
   SBUF=ser;
   A1=temp/100;								//A1 --> 百位
   A2t=temp%100;
   A2=A2t/10;									//A2 --> 十位
   A3=A2t%10;									//A3 --> 个位
   dula=0;
   P0=table[A1];							//显示百位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfe;
   wela=1;
   wela=0;
   delay(1);

   dula=0;
   P0=table1[A2];							//显示十位
   dula=1;
   dula=0;

   wela=0;
   P0=0xfd;
   wela=1;
   wela=0;
   delay(1);

   P0=table[A3];							//显示个位
   dula=1;
   dula=0;

   P0=0xfb;
   wela=1;
   wela=0;
   delay(1);
}



void main()
{
	uchar a;
	do
	{
		tmpchange();							//首次温度转换
		//delay(200);
		for(a=100;a>0;a--)				//延时,保持连续显示
		{
			display(tmp());					//进行温度转换和数值显示
		}
	}while(1);
}

