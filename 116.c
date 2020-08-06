#include "reg52.h"
#include "hd7279.h"
int s=0,a=1;
int score1=0,score2=0,change=0;
unsigned char i=20;
void Delay_ms(unsigned int Ms)//延时函数
{
	int us,usn;
	while(Ms!=0)
	{ 
		usn = 4;
		while(usn!=0)
		{
			us=0xfa;
			while (us!=0){us--;};
			usn--;
		}
		Ms--;
	}
}
void time(void)//时间函数
{
	TMOD=0x01;
	TH0=0x3c;
	TL0=0xb0;
	EA=1;
}
void display(int s)//显示函数
{
			write_hd7279(HD7279_DECODE0,(s%60)%10);	 //显示时间
			write_hd7279(HD7279_DECODE0+1,(s%60)/10);
			write_hd7279(HD7279_DECODE0+2,(s/60)%10);
			write_hd7279(HD7279_DECODE0+3,(s/60)/10);
			write_hd7279(HD7279_DECODE0+5,score2/10);//显示乙的比分
			write_hd7279(HD7279_DECODE0+4,score2%10);
			write_hd7279(HD7279_DECODE0+7,score1/10);//显示甲的比分
			write_hd7279(HD7279_DECODE0+6,score1%10);
}
void T0_int(void) interrupt 1//中断服务子程序
{
	TH0=0x3c;
	TL0=0xb0;
	i--;
	display(s);
	if(i<=0)
	{
		i=20;
		if(a==0&&s>0)
			s=s-1;
		else if (a==0&&s==0)
			send_byte(HD7279_FLASH );
		else 
			s=s;
	}
}
void main()
{
  	unsigned char j;
	send_byte(HD7279_RESET);
	Delay_ms(50);
	TR0=1,ET0=1;
	j=0xff;	
	HD7279A_KEY=1;
	time();
	while(1)
	{
		while(!HD7279A_KEY)
		{
			j=read_hd7279(HD7279_READ);
			switch(j)
			{
				case 0x3f:score1=score1+1;break;//甲+1分
				case 0x3e:score1=score1+2;break;//甲+2分
				case 0x3d:score1=score1+3;break;//甲+3分
				case 0x3c:score1=score1-1;break;//甲-1分
				case 0x37:score2=score2+1;break;//乙+1分
				case 0x36:score2=score2+2;break;//乙+2分
				case 0x35:score2=score2+3;break;//乙+3分
				case 0x34:score2=score2-1;break;//乙-1分
				case 0x2f:s=s+60;break;//设置时间(+1min)
				case 0x2e:s=s+30;break;//设置时间(+30s)
				case 0x2d:a=0;break;//比赛开始
				case 0x2c:a=1;break;//比赛暂停
				case 0x27:				   //交换分数
				{
					change=score1;
					score1=score2;
					score2=change;;
				}
				break;	
				case 0x26:send_byte(HD7279_RESET);break;	
			}
				while(!HD7279A_KEY);
		}
			
	}
}

