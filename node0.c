#include <LPC21xx.H>
#include "headerfp.h"
#include "dashboard.c"
#include "spi-fp.c"
#include "mcp3204-fp.c"
#include "can-driver-fp.c"


void main(){	  //node0

	u32 bB,bA;  
	f32 fly;
	CAN2_MSG m1;

	can2_init();

	IODIR0|=(3<<8)|(0xff<<0)|(1<<15);
	IOSET0|=(3<<8)|(0xff<<0)|(1<<15);
	IOCLR0|=(3<<8)|(0xff<<0);
	
	lcd(0x0e,0);
	lcd(0x0c,0);
	lcd(0x38,0);
	lcd(0x80,0);
	
	dashboard();

	PINSEL0|=2<<28;
	PINSEL1|=1<<10;	
	VICIntSelect=0;
	EXTMODE=0;
	EXTPOLAR=0;
	VICVectCntl0=(1<<5)|15;
	VICVectAddr0=(unsigned long)pwm;
	VICIntEnable=1<<15;

	lcd('a',1);

	lcd(0x80,0);
	str("FUEL : 0.0 %");
	lcd(0xc0,0);
	str("TEMP : 0.0 c");

	VPBDIV=1;
	PWMMR0=(60*20000)-1;
	PWMMR5= (1000*60)-1;
	PWMMCR=2;
	PWMPCR=1<<13;
	PWMTCR=0x9;
	PWMLER=(1<<0)|(1<<5);
	
	while(1){
		
		can2_rx();
	}
	
}