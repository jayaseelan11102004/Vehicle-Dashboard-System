
#include "headerfp.h"

void delay_milliseconds(u32);

main(){	
	
	CAN2_MSG m1;

	can2_init();
	
	Init_SPI0();
	
	while(1){
		
		m1.id=0x00000160;
		
		m1.rtr=0;//data frame
		
		m1.dlc=4;
		
		m1.byteA=Read_ADC_MCP3204(0);
		
		m1.byteB=0;
		
		can2_tx(m1);
		
		delay_milliseconds(750);
		
	}
}

void delay_milliseconds(u32 milliseconds)
{
	//T0CTCR=0X00;
	T0PR = 15000-1;
	T0TCR = 0X01;
	while(T0TC<milliseconds);
	T0TCR = 0x03;
	T0TCR = 0x00;
}
