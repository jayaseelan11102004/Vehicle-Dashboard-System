                               /* spi.c */
//#include "headerfp.h"

// peripheral/hw spi
void Init_SPI1(void)
{
	PINSEL1 |=42<<2;
  S1SPCCR = 150;                  //S0SPCCR = SPI_RATE cfg bit clock rate 
	S1SPCR  = (1<<MSTR_BIT|Mode_3); //spi module in master mode,
                                  //CPOL =1,CPHA = 1. MSB first
	                                //SETBIT(IODIR0,CS);
	                                //SETBIT(IOPIN0,CS);
	IODIR0 |= 1<<20;                //P0.20 as OUTPUT(for CS)
	IOPIN0 |= 1<<20;                //make CS as HIGH initially
}

u8 SPI1(u8 data)
{
   u8 stat;
   stat = S1SPSR;                        //clear SPIF 
   S1SPDR = data;                        // load spi tx reg
   while(READBIT(S1SPSR,SPIF_BIT)==0);   // wait for transmission to complete
   return S1SPDR;                        // read data from SPI data reg, place into buffer 
}
				 /* end of spi.c */

