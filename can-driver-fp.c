/*#include <LPC21xx.H>
#include "headerfp.h"
#include "dashboard.c"
#include "spi-fp.c"
#include "mcp3204-fp.c"*/

void can2_init(void){
	
	PINSEL1|=5<<14;       //P0.23-->RD2 & P0.24-->TD2
	VPBDIV=1;             //PCLK=60MHz
	C2MOD=0x1;            //CAN2 into Reset  Mode 
	AFMR=0x2;             //accept all receiving messages(data/remote)
	C2BTR=0x001C001D;     //B125Kbps @ PLCK=60MHz(BRP=Pclk/bit rate*16)
	C2MOD=0x0;            //CAN1 into Normal Mode
	C2IER=1;
}

///*
void can2_tx(CAN2_MSG m1)
{

  C2TID1=m1.id;
	C2TFI1=(m1.dlc<<16);	
	C2TFI1&=~(1<<30);					//RTR=0
	C2TDA1=m1.byteA; 					//lower 4bytes of data
	C2TDB1=m1.byteB; 					//upper 4bytes of data
	C2CMR=(1<<0)|(1<<5);			//Start Xmission & select Tx Buf1
	while((C2GSR&(1<<3))==0);	//wait for data Xmission.
}
//*/


///*
void can2_rx(void){

	CAN2_MSG m1;
	while((C2GSR & 1)==0);
	
	m1.id=C2RID;
	m1.dlc=(C2RFS>>16)&0xF;
	m1.rtr=(C2RFS>>30)&0x1;
	m1.byteA=C2RDA;
	m1.byteB=C2RDB;
	C2CMR=(1<<2);             //free receiver buffer(imp)
	if(m1.id==0x00000123){
		
		lcd(0x87,0);
		lcd_integer(m1.byteB);
		lcd('.',1);
		lcd_integer(m1.byteA);
		lcd(' ',1);
		lcd('%',1);
	}
	else{
		
		lcd(0xc7,0);
		lcd_integer(m1.byteB);
		lcd('.',1);
		lcd_integer(m1.byteA);
		lcd(' ',1);
		lcd('C',1);
	}

}
	
	
void pwm(void) __irq{	
	
	int c=(1000*60)-1;
	IOCLR0=(1<<15);
	EXTINT=2;

	while(1){			
		PWMMR5 = c;
    	PWMLER = 1<<5;
				
		delay_milliseconds(100);
		c=c+(56*60);
			
		if(c>((2200*60)-1)){
			
			c=(1000*60)-1;
			delay_milliseconds(3000);
			break;
		}
	}
				
	IOSET0=(1<<15);
	VICVectAddr=0;
}
