#include "headerfp.h"
main(){	
	
	int c;
	
	VICIntSelect=0;
	VICVectCntl0=(1<<5)|27;
	VICVectAddr0=(unsigned long)can2_rx;
	VICIntEnable=1<<27;
	
	can2_init();
	dashboard();
	
	PINSEL1|=1<<10;
	PWMMR0=(15000*20)-1;
	PWMMR5=(15000*1.5)-1;;
	PWMMCR=2;
	PWMPCR=1<<13;
	PWMTCR=0x9;
	
	lcd(0x80,0);
	str("TEMP : 0.0 c");
	lcd(0xc0,0);
	str("FUEL : 0 %");
	
	while(1){
		if(IOPIN0&(1<<20)){
			PWMMR5=(15000*2)-1;
			PWMLER=1<<5;
			for(c=0;c<5000000;c++);
			PWMMR5=(15000*1)-1;
			PWMLER=1<<5;
			for(c=0;c<5000000;c++);
		}
		else{
			PWMMR5=(15000*1.5)-1;
			PWMLER=1<<5;
		}
	}
}
