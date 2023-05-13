#include <REGX52.H>
#define KEY_NOT_PRESSED P3_2 == 1
#define ON 0
#define OFF 1
#define DIODE P0_0

unsigned char code patternArray[] = { 0xEF, 0xDF, 0xBF, 0x7F};
unsigned char data index = 0;
unsigned char bdata key;

sbit key_4 = key^4; //row 0
sbit key_5 = key^5;	//row 1
sbit key_6 = key^6;	//row 2
sbit key_7 = key^7;	//row 3

//400 ms delay
void delay(){
	unsigned char i;
	unsigned char j;
	
	for(i = 0; i < 150; i++){
		for(j = 0; j < 100; j++){
			;
		}
	}
}

void init(){
		//interrupts setup
		TMOD = 0x22;          //timer 1 configure                     
    TH1  = 0x00;          //initial value
    TL1  = 0x00;          
    TCON = 0x10;          //timer 1 configure
    ET1  = 1;             //enable timer 1 interrupt         
	  
    EX0 = 1;              //enable int0 interrupt
    IT0 = 1;              //enable trailling edge
    
		TR1 = 1; 							//turn on timer 1
    EA = 1;    	          //enable all interrupts

		//diode
		DIODE = OFF;
	
		//keyboard
		P2 = 0xFF;
}

void putZeroNextRow() interrupt 3{
	P2 = patternArray[index];
	if(KEY_NOT_PRESSED){
		if(index < 3){
			index++;
		}else{
			index = 0;
		} 
	}
}

int getPressedKeyRow(){
	if(key_4 == 0){
		return 1;
	} else if (key_5 == 0) {
		return 2;
	}	else if (key_6 == 0) {
		return 3;
	}	else if	(key_7 == 0) {
		return 4;
	}
	return 0;
}

void lightDiode(){
	DIODE = ON;
	delay();
	DIODE = OFF;
}

void decodePressedKey() interrupt 0{
	int row, i;
	
	TR1 = 0; 
	EX0 = 0;
	
	key = P2;
	row = getPressedKeyRow();	
	for(i = 0; i < row; i++){
		lightDiode();
		delay();
	}
	
	delay();
	delay();
	
	lightDiode();
	
	TR1 = 1; 
	EX0 = 1;
}

void main(void){
	init();
	while(1);
}