#ifndef __HEADER_H_
#define __HEADER_H_

#include <LPC21xx.H>
#define PORT0 0
#define PORT1 1
#define SET   1
#define CLEAR 0

#define SETBIT(WORD,BITPOS)         (WORD|=1<<BITPOS)
#define CLRBIT(WORD,BITPOS)         (WORD&=~(1<<BITPOS))
#define CPLBIT(WORD,BITPOS)         (WORD^=(1<<BITPOS))
#define WRITEBIT(WORD,BITPOS,BIT)   (BIT ? SETBIT(WORD,BITPOS): CLRBIT(WORD,BITPOS))




#define READBIT(WORD,BITPOS)        ((WORD>>BITPOS)&1)

#define CS 7

//defines for clock /baudrate

/*#define FOSC 12000000
#define CCLK (5*FOSC)
#define PCLK (CCLK/4)
#define SPCCR_VAL 60
#define SPI_RATE (u8)(PCLK/SPCCR_VAL)
*/
// SPCR Bits Setting 
#define Mode_0     0x00  // CPOL 0 CPHA 0
#define Mode_1     0x08  // CPOL 0 CPHA 1
#define Mode_2     0x10  // CPOL 1 CPHA 0
#define Mode_3     0x18  // CPOL 1 CPHA 1
#define MSTR_BIT   5     // SPI0 as Master 
#define LSBF_BIT   6     // default MSB first,if set LSB first
#define SPIE_BIT   7     //SPI Interrupt Enable Bit

// SPSR bits
#define SPIF_BIT   7    // Data Transfer Completion Flag

//SPINT bit
#define SPIINTF_BIT 0   //SPI Interrupt Flag Bit


typedef unsigned char u8;
typedef signed char   s8;
typedef unsigned short int  u16;
typedef signed short   int  s16;
typedef unsigned int  u32;
typedef signed   int  s32;
typedef float f32;
typedef double f64;

typedef struct CAN2
{
	u32 id;
	u32 rtr;
	u32 dlc;
	f32 byteA;
	u32 byteB;
}CAN2_MSG;

void can2_init(void);
void can2_tx(CAN2_MSG);
//void can2_rx(CAN2_MSG *m1);
void can2_rx(void) __irq;

void Init_SPI0(void);
u8 SPI0(u8);
f32 Read_ADC_MCP3204(u8 channelNo);

void uart0_init(u32 baud);
void uart0_tx(u8 ch);
u8 uart0_rx(void);
void uart0_string(s8 *s);
void uart0_integer(s32 n);

void lcd(unsigned char c,unsigned char d );
void dashboard(void);
void str(u8 *s);
void lcd_integer(int n);
void Float_display(f32 f_val);

#endif
