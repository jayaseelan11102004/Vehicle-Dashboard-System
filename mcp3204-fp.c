                        /*  file: mcp3204.c */
//#include "headerfp.h"

f32 Read_ADC_MCP3204(u8 channelNo)
{
  u8 hByte,lByte;
  u32 adcVal=0;
  CLRBIT(IOPIN0,CS);   				//select/activate chip 
															//delay_ms(100);
	SPI1(0x06);
  hByte = SPI1(channelNo<<6);
  lByte = SPI1(0x00);
															//de-select/de-activate chp
	SETBIT(IOPIN0,CS);
															//delay_ms(100);
  adcVal=((hByte&0x0f)<<8)|lByte;
  return (((adcVal*3.3)/4096)*100);
}
                     /* end of mcp3204.c */
