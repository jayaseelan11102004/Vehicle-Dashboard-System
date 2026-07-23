
#include <LPC21xx.H>
#include "headerfp.h"
#include "dashboard.c"
#include "spi-fp.c"
#include "mcp3204-fp.c"
#include "can-driver-fp.c"


main(){	
	
	u32 bB,bA;  
	f32 fly;
	CAN2_MSG m1;
	
	IODIR0|=(3<<8)|(0xff<<0);
	IOSET0|=(3<<8)|(0xff<<0);
	IOCLR0|=(3<<8)|(0xff<<0);
	
	lcd(0x0e,0);
	lcd(0x0c,0);
	lcd(0x38,0);
	lcd(0x80,0);
	
	can2_init();
	Init_SPI1();
	
	while(1){
		
		fly= Read_ADC_MCP3204(0);
		bB = fly;
		bA= (fly-bB)*100;
		
		m1.id=0x00000104;
		m1.rtr=0;                     //data frame	
		m1.dlc=8;
		m1.byteA= bA;
		m1.byteB= bB;
		
		{
		
		lcd(0x01,0);
		lcd(0x80,0);
		
		lcd_integer(bB);
		lcd('.',1);
		lcd_integer(bA);	
			
		}
		
		can2_tx(m1);
		delay_milliseconds(800);
	}
}

