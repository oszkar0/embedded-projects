#include <REGX52.H>

sbit greenLed = P2^0;
sbit yellowLed = P2^7;
sbit button = P3^2;

void init(){
	greenLed = 1; //initial value of green led

	TMOD = 0x01; //timer 0 mode 1 (16 bit timer)
	TH0 = 0XDE; //load value
  TL0 = 0X40;
	ET0 = 1; //enable timer0 interrupt
	EA = 1; //enable gloabl interrupt
	TR0 = 1; //turn on timer 0

}

void timer0Interrupt() interrupt 1{
	TH0 = 0xDE; //load value
  TL0 = 0x40;
	greenLed = !greenLed;
}

void main(void){
	init();
	while(1){
		yellowLed = button;
	}
}