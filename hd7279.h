#include "reg52.h"
#include <intrins.h> 
#ifndef _HD7279A_H_
#define _HD7279A_H_


sbit HD7279A_CS=P2^2;	//pin1
sbit HD7279A_CLK=P1^1;	//pin2
sbit HD7279A_DATA=P1^2;	//pin3
sbit HD7279A_KEY=P1^3;	//pin4
//command define
#define HD7279_RESET   0xa4
#define HD7279_TEST    0xbf
#define HD7279_RLC     0xa3
#define HD7279_RRC     0xa2
#define HD7279_RL    	0xa1
#define HD7279_RR      0xa0
#define HD7279_DECODE0  0x80
#define HD7279_DECODE1  0xc8
#define HD7279_UNDECODE  0x90
#define HD7279_HIDE      0x98
#define HD7279_FLASH     0x88
#define HD7279_SEGON     0xe0
#define HD7279_SEGOFF    0xc0
#define HD7279_READ      0x15

void Delay_ms(unsigned int Ms);
void Delay_us(unsigned char Us);

void send_byte(unsigned char c);
unsigned char recv_byte(void);
void write_hd7279(unsigned char cmd, unsigned char dat);
unsigned char read_hd7279(unsigned char cmd);


void send_byte(unsigned char c)
{
 	unsigned char i,j,k;
 	HD7279A_CS=0;
	HD7279A_DATA=1;	
 	for (i=0;i<30;i++)
	{
	//	_nop_();
	}
	
	for(i=0;i<8;i++)
 	{
		if(c&0x80)	
		{
			HD7279A_DATA=1;			
		}
  		else	
		{
			HD7279A_DATA=0;
		}
  		
		HD7279A_CLK=1;
  		
 		for (j=0;j<8;j++)
		{
		//	_nop_();
		}

		HD7279A_CLK=0;
		 	
 		for (k=0;k<8;k++)
		{
		//	_nop_();
		}
		
		c<<=1;
 	}
//	HD7279A_CS=1;
}
/*********************************
			读回按键值
**********************************/
unsigned char recv_byte(void)
{
 	unsigned char ret;
 	unsigned char i,j,k;
 	HD7279A_CS=0;
	HD7279A_DATA=1;				//读口线之前要先拉高P1口
 	for (i=0;i<30;i++)
	{
		_nop_();
	}
 	for(i=0;i<8;i++)
 	{
  		HD7279A_CLK=1;
  		for (j=0;j<8;j++)
		{
			_nop_();
		}
  		ret<<=1;
  		if(HD7279A_DATA)	ret|=0x01;
  		HD7279A_CLK=0;
  		for (k=0;k<8;k++)
		{
			_nop_();
		}
 	}
 	HD7279A_DATA=0;
 	HD7279A_CS=1;
	return ret;	
}

void write_hd7279(unsigned char cmd, unsigned char dat)
{
	send_byte(cmd);
 	send_byte(dat);
}

unsigned char read_hd7279(unsigned char cmd)
{
 	send_byte(cmd);
 	return recv_byte();
}

#endif