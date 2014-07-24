
/*
 *	Yuanzhang Hao
 *	July 18, 2014
 *	Ref: ISEE main05.c
 */

#include <reg52.h>
#include <STC12C.h>
#include <intrins.h>              
#include <stdio.h>

//变量类型标识的宏定义
#define Uchar unsigned char
#define Uint unsigned int
#define uchar unsigned char
#define uint unsigned int

#define DataPort P2		// 数据端口
#define Busy    0x80

uint	Timer_Count;	//定时计数器
uchar	Timer_Count_2;	//定时计数器2

uchar	Dsp_Count;		//显示计数器

uchar	Key_value;		//键值

uchar	Timer_s,Timer_m,Timer_h = 0;

uint adc_result[8] = {0,0,0,0,0,0,0,0};
uchar	Adc_Ch=0;

bit	Key1_press_flag;	//Key1按下标志
bit	Key2_press_flag;	//Key2按下标志
bit	Key3_press_flag;	//Key3按下标志

bit	Key1_back;		//按键的上一状态
bit	Key2_back;
bit	Key3_back;

bit	Second_flag;
bit Pwm_flag = 0;


//接口定义
sbit	Led1 = P1^0;
sbit	Led2 = P1^1;
sbit	Led3 = P1^2;

sbit	Key1 = P1^5;
sbit	Key2 = P1^4;
sbit	Key3 = P1^3;

// 控制引脚定义，不同的连接必须修改的部分
sbit RS=P3^2;
sbit RW=P3^3; 
sbit EN=P1^7;  

sbit P3_4=P3^4;
sbit P3_5=P3^5;

#define rs RS
#define	rw RW
#define en EN

uchar PULSE_W0, PULSE_W1, PULSE_W2, PULSE_T0, PULSE_T1, PULSE_T2;

uchar	Dsp_buf[16]="LCD1602 check ok";
uchar	Dsp_buf2[16]="   study up     ";

/*Define ADC operation const for ADC_CONTR*/
#define ADC_POWER   0x80            //ADC power control bit
#define ADC_FLAG    0x10            //ADC complete flag
#define ADC_START   0x08            //ADC start control bit
#define ADC_SPEEDLL 0x00            //420 clocks

//sbit	Ad_in	= P1^7;

sfr PCAPWM0     =   0xF2;
sfr PCAPWM1     =   0xF3;
sfr PCAPWM2     =   0xF4;
sfr PCAPWM3     =   0xF5;

unsigned long v_fix_point;
unsigned long freq_fix_point;
unsigned long  phi_fix_point;
unsigned long temp;
unsigned long delta;
/******************系统初始化**************************/
void system_init(void)
{
	//定义端口输入输出,p1^0-P1^2, P1^7为推挽输出，p1^3-P1^6为输入
	//设置PxM0，PxM1，
	P1M0 = 0x78;		//#0111 1000b
	P1M1 = 0x87;		//#1000 0111b
	
	//T0,模式设定
    TR0 = 0;        //停止计数
    ET0 = 1;        //允许中断
    PT0 = 1;        //高优先级中断
    TMOD = 0x11;  	//#0001,0001,16位定时模式

    TH0 = 0;
    TL0 = 0;
    TR0 = 1;    	//开始运行
	//AUXR |=  0x80;  //1T,默认为12T

	P1 &= ~0x07;	//清除LED
}

void LcdWriteData( char dataW );
void LcdWriteCommand( Uchar CMD );
void Lcd_init( void );
void Delay500us(void);
void Delay(uint y);

	/********************延时函数************************/
	//延时y ms
void	Delay(uint y)
{
	uint	x;
	while (y--)
	{
		x = 1000;
		while (x--);
	}
}

/*=======================================================
初始化程序, 必须按照产品资料介绍的初始化过程进行
=======================================================*/
void Lcd_init( void ) 
{
    LcdWriteCommand(0x38);            // 显示模式设置(以后均检测忙信号)
    LcdWriteCommand(0x0c);            // 开显示屏，管光标，光标不闪烁
    LcdWriteCommand(0x06);            // 显示地址递增
    LcdWriteCommand(0x01);            // 显示清屏
}

/*=======================================================
写控制字符子程序: E=1 RS=0 RW=0
=======================================================*/
void LcdWriteCommand(Uchar CMD)
{
    rs=0;			//选命令寄存器
    rw=0;           //写
    DataPort=CMD;	//写命令
    //Delay(1);		//延时，让1602准备数据
	Delay500us();
    en=1;           //使能信号变化，写入数据
    en=0;
}

/*=======================================================
当前位置写字符子程序: E =1 RS=1 RW=0
=======================================================*/
void LcdWriteData( Uchar Data )
{
    rs=1;			//选数据寄存器
    rw=0;           //写
    DataPort=Data;	//写命令
	//    Delay(1);		//延时，让1602准备数据
	Delay500us();
    en=1;           //使能信号变化，写入数据
    en=0;
}


/********************* 键盘扫描函数************************/
void	Key_Scan()
{
	if (!Key1)			//开机键扫描，检测下降沿
	{
		if(Key1_back)
		{
			Key1_press_flag = 1;
		}
	}

	if (!Key2)			//开机键扫描
	{
		if(Key2_back)
		{
			Key2_press_flag = 1;
		}
	}

	if (!Key3)			//开机键扫描
	{
		if(Key3_back)
		{
			Key3_press_flag = 1;
		}
	}

	Key1_back = Key1;
	Key2_back = Key2;
	Key3_back = Key2;

}

#define ADC_PORT 6

// 短延时
void Delay500us(void)
{
    Uint i = 25;
    while(i--);
}

/********************* Timer0中断函数************************/
void timer0_int (void) interrupt 1
{	
	EA = 0;
	TH0 = 0xfc;
	TL0 = 0x67;			//1ms定时中断
	//TH0 = 0x4b;
	//TL0 = 0x00;			//50ms定时中断

	Key_Scan();

	//定时
	++Timer_Count;
	if ((Timer_Count & 0x3f) == 0)
	//if ((Timer_Count & 0x07) == 0)
	{
		Adc_Ch = ADC_PORT;
		ADC_CONTR = ADC_POWER | ADC_SPEEDLL | Adc_Ch | ADC_START;
		_nop_();                        //Must wait before inquiry
		_nop_();
		_nop_();
		_nop_();
		while (!(ADC_CONTR & ADC_FLAG));//Wait complete flag
		ADC_CONTR &= ~ADC_FLAG;         //Close ADC
		adc_result[Adc_Ch] = ADC_DATA*4+(ADC_LOW2 && 0x03);

	}
	
	//if (Timer_Count > 10)		//0.5s进入一次
	if (Timer_Count > 500)		//0.5s进入一次
	{
		Second_flag = !Second_flag;
		if (Second_flag)
		{
			++Timer_s;
			if (Timer_s == 60)
			{
				Timer_s = 0;
				++Timer_m;
				if (Timer_m == 60)
				{
					Timer_m = 0;
					++Timer_h;
					if(Timer_h == 24)
					{
						Timer_h =0;
					}
				}
			}
		}
		
		Timer_Count = 0;
		Led1 = !Led1;
		++Timer_Count_2;
		
		Timer_Count_2 &= 0x07;	//取低3位


	//		P1 &= ~0x07;
	//		P1 |= Timer_Count_2;
	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		//显示时间
		Dsp_buf[6] = Timer_h/10+0x30;
		Dsp_buf[7] = (Timer_h % 10)+0x30;
		Dsp_buf[8] = ':';
		Dsp_buf[9] = Timer_m/10+0x30;
		Dsp_buf[10] = (Timer_m % 10)+0x30;
		Dsp_buf[11] = ':';
		Dsp_buf[12] = Timer_s/10+0x30;
		Dsp_buf[13] = (Timer_s % 10)+0x30;
			
		if (Second_flag)
		{
			Dsp_buf[8] = ':';
			Dsp_buf[11] = ':';
		}
		else
		{
			Dsp_buf[8] = ' ';
			Dsp_buf[11] = ' ';
		}

		//T1,模式设定
		TR1 = 0;        //停止计数
		ET1 = 0;        //bu允许中断
		PT0 = 0;        //高优先级中断
		TMOD = 0x11;  	//#0001,0001,16位定时模式
		TH1 = 0;
		TL1 = 0;
		while (P3_4 == 0);	//wait rising edge
		_nop_();                        
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();                        
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		if (P3_4 == 1)
		{
			TR1 = 1;  	// -4
			while (P3_4 == 1);
		}
		//while (P3_4 == 1);
		//while (P3_4  == 0);
		temp = (TH1 * (1 << 8) + TL1);
		//while (P3_5	== 0);
		TR1 = 0;

		//temp = 16;
		//temp *= 16;
		//temp *= 16;
		//temp *= 9;
		//temp /= (TH1 * (2 << 8) + TL1);
		//freq_fix_point = (TH1 * (2 << 8) + TL1);
		//temp_f = 11059.2 / 12.0 / (TH1 * (2 << 8) + TL1) / 2.0;
		//sprintf(Dsp_buf2, "%d-", (int)(TH1 * (2 << 8) + TL1));
		//freq_fix_point = temp / 2 / 12;
		freq_fix_point = 4608 / temp;//110592 / (TH1 * (1 << 8) + TL1) / 12 / 2;

		 Dsp_buf2[0] = freq_fix_point / 10 + '0';
		 Dsp_buf2[1] = '.';
		 Dsp_buf2[2] = freq_fix_point % 10 + '0';
		 Dsp_buf2[3] = '0';
		 Dsp_buf2[4] = 'k';
		 Dsp_buf2[5] = 'H';
		 Dsp_buf2[6] = 'Z';
		 Dsp_buf2[7] = ' ';


		v_fix_point = adc_result[ADC_PORT] * 60 / ( 1 << 10); // assume 2^10 is 6.0V 2^0 is 0.0V
		Dsp_buf2[ 8] = v_fix_point / 10 + 0x30;
		Dsp_buf2[ 9] = '.';
		Dsp_buf2[10] = v_fix_point % 10 + 0x30;
		Dsp_buf2[11] = 'V';

		//Dsp_buf2[8] = (adc_result[ADC_PORT]/1000)+0x30;
		//Dsp_buf2[9] = ((adc_result[ADC_PORT]%1000)/100)+0x30;
		//Dsp_buf2[10] = ((adc_result[ADC_PORT]%100)/10)+0x30;
		//Dsp_buf2[11] = (adc_result[ADC_PORT]%10)+0x30;
		delta = (TH1 * (1 << 8) + TL1) - temp;
		temp /= 10;
		phi_fix_point = 18 * delta / temp;
		Dsp_buf[0] = 'p';
		Dsp_buf[1] = 'h';
		Dsp_buf[2] = 'i';
		Dsp_buf[3] = '0' + phi_fix_point / 100;
		Dsp_buf[4] = '0' + phi_fix_point / 10 % 10;
		Dsp_buf[5] = '0' + phi_fix_point % 10;
		Dsp_buf[6] = ' ';
		Dsp_buf[7] = ' ';
		Dsp_buf[8] = ' ';

	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//显示处理
	++Dsp_Count;
	Dsp_Count &= 0x3f;	//#0011 1111B
	if (Dsp_Count == 0x0a)
	{
		LcdWriteCommand(0x80);	//指定显示第一行
	}

	if ((Dsp_Count/16) == 0x01) //10 1f
	{
		LcdWriteData(Dsp_buf[Dsp_Count%16]);	//显示第一行内容
	}

	if (Dsp_Count == 0x2a)
	{
		LcdWriteCommand(0x80+0x40);	//指定显示第二行
	}

	if ((Dsp_Count/16) == 0x03)
	{
		LcdWriteData(Dsp_buf2[Dsp_Count%16]);	//显示第二行内容
	}
	EA = 1;
}


/******************** 按键处理 **************************/
void	Key_treat()
{
	if (Key1_press_flag)
	{
		Led1 = Led1^1;	//
		Key1_press_flag = 0;
	//		tone();
	}

	if (Key2_press_flag)
	{
		Led2 = Led2^1;	//
		Key2_press_flag = 0;
	//		tone();
	}

	if (Key3_press_flag)
	{
		Led3 = Led3^1;	//
		Key3_press_flag = 0;
	//		tone();
	}
}

/***********************display**********************/
void Display_menu()
{
	Dsp_buf[0] = 'T';
	Dsp_buf[1] = 'i';
	Dsp_buf[2] = 'm';
	Dsp_buf[3] = 'e';
	Dsp_buf[4] = 'r';
	Dsp_buf[5] = ':';
	Dsp_buf[6] = '0';
	Dsp_buf[7] = '0';
	Dsp_buf[8] = ':';
	Dsp_buf[9] = '0';
	Dsp_buf[10] = '0';
	Dsp_buf[11] = ':';
	Dsp_buf[12] = '0';
	Dsp_buf[13] = '0';
	Dsp_buf[14] = ' ';
	Dsp_buf[15] = ' ';
	
	Dsp_buf2[0] = 'V';
	Dsp_buf2[1] = 'o';
	Dsp_buf2[2] = 'l';
	Dsp_buf2[3] = 't';
	Dsp_buf2[4] = 'a';
	Dsp_buf2[5] = 'g';
	Dsp_buf2[6] = 'e';
	Dsp_buf2[7] = ':';
	Dsp_buf2[8] = '0';
	Dsp_buf2[9] = '0';
	Dsp_buf2[10] = '0';
	Dsp_buf2[11] = '0';
}

/******************* 频率读取 *************************/
void PCA_init(void)
{
	CCON = 0x00;
	CL = 0x00;  // Reset PCA counter
	CH = 0x00;	// Reset

	CMOD = 0x01;	//选择时钟源为12分频，允许计数溢出中断
	CCAPM0 = 0x31;	//允许上升沿、下降沿捕获中断
	//EPCAI = 1;		//PCA允许单独中断

	P3_4 = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();	//等待P3.4引脚输出稳定

	while (P3_4 == 1);//若P3.4引脚为高，则等待低电平
	while (P3_4 == 0);//若P3.4引脚为低，则等待上升沿

	CR = 1; //检测到P3.4引脚为上升沿,启动计数器工作
	CF = 0;  //清溢出中断标志
	CCF0 = 0; //清捕获中断标志

	_nop_();
}

void PCA_INT() interrupt 7
{
	EA = 0;		// 全局中断
	if(CF == 1)    //若计数器溢出
	{
		CF = 0; //清溢出标志CF
		if(P3_4 == 1) //P3.4引脚为高电平时
		{
			PULSE_W0++; //脉宽计数的最高字节加1
			PULSE_T0++;//周期计数的最高字节加1
		}
		else
			PULSE_T0++;//P3.4引脚为低，只对周期最高字节加1
	}	
	if(CCF0 == 1) //若捕获中断溢出
	{
		CCF0 = 0; //清捕获中断标志CCF0
		if (P3_4 == 0)  //检测到P3.4引脚为下降沿捕获时
		{
			PULSE_W2 = CCAP0L; //读出脉冲宽度
			PULSE_W1 = CCAP0H;
		}
		else   //检测到P3.4引脚为上升沿捕获时
		{
			CR = 0;
			PULSE_T2 = CCAP0L; //读出周期宽度
			PULSE_T1 = CCAP0H;
			freq_fix_point = 10;
			//freq_fix_point = 110592 / 
			//	(12 * (PULSE_T1 * (1 << 8) + PULSE_T2) );
		}
	}
	EA = 1;
}


/********************主函数**************************/
void main(void)
{
	system_init();
	Delay(100);
	Lcd_init();
	//PCA_init();

	Display_menu();
    EA = 1;

    while (1)
    {
		//Key_treat();
		//Delay(500);
		// if (Pwm_flag)
		// {
		// 	Pwm_treat();
		// 	Pwm_flag = 0;
		// }
	
		//		Led1 = !Led1;
	//	Display();
    }
}
