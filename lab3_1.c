#include <REGX52.H>

#define ON 0
#define OFF 1


void delay(void){
	unsigned char i;
	unsigned char j;
	
	for(i = 0; i < 188; i++){
		for(j = 0; j < 100; j++){
			;
		}
	}
}


unsigned char code patternArray[] = { 0xEF, 0xDF, 0xBF, 0x7F};
unsigned char bdata key;
sbit key_1 = key^1;
sbit key_2 = key^2;
sbit key_3 = key^3;

void main(void){
	unsigned char index = 0;
	bit decodeEnable = 1;
	
	while(1){
		P2 = patternArray[index];
		key = P2;
		if((key_1 && key_2 && key_3) == 0){
			if(decodeEnable){
				P0_0 = ON;
				delay();
				P0_0 = OFF;
				decodeEnable = 0;
			}
		} else {
			if(index < 3){
				index++;
			} else {
				index = 0;
			}
			decodeEnable = 1;
		}
	}
}