//#include "headerfp.h"



void lcd(unsigned char c,unsigned char d ){
	
	int i,j;
	if(d==1)
		IOSET0=1<<8;
	IOSET0=c<<0;
	IOSET0=1<<9;
	delay_milliseconds(10);
	IOCLR0=1<<9;
	IOCLR0=(3<<8)|(0xff<<0);
	
}


void dashboard(void){
	
	int a,b;
	char t[]="VEHICLE DASHBOARD USING AUTOMOTIVE INDUSTRY STANDARD MAINLINE COMMUNICATION PROTOCAL";
	
	IODIR0|=(3<<8)|(0xff<<0);
	IOSET0|=(3<<8)|(0xff<<0);
	IOCLR0|=(3<<8)|(0xff<<0);
	
	lcd(0x0e,0);
	lcd(0x0c,0);
	lcd(0x38,0);
	lcd(0x80,0);
	
	for(a=0;a<84;a++){
		lcd(0x80,0);
		
		if(a==83){
			lcd(0x01,0);
			lcd(t[a],1);
		}
		
		else{
		for(b=0;b<16;b++)
			lcd(t[a+b],1);
		
	}
	lcd(0x01,0);
}
}



void str(u8 *s)
{
	while(*s)
		lcd(*s++,1);
}


void lcd_integer(int n)
{
	s8 arr[5],i=0;
	if(n==0){
		lcd('0',1);
	}
	else
	{
		   if(n<0)
		   {
				 lcd('-',1);
				 n=-n;
       }
	      while(n>0)
      	{
		     arr[i++]=n%10;
		     n=n/10;
        }
        for(--i;i>=0;i--)
   	      lcd(arr[i]+48,1);
  }
}
			

void Float_display(f32 f_val)
{
  u32 number;
  number = f_val;
  lcd_integer(number);
  lcd('.',1);
  number=(f_val-number)*100;
  lcd_integer(number);	
	
}

void delay_milliseconds(u32 milliseconds)
{

	T0PR = 60000-1;
	T0TCR = 0X01;
	while(T0TC<milliseconds);
	T0TCR = 0x03;
	T0TCR = 0x00;
}



			
			