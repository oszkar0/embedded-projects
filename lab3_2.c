#include <REGX52.H>

#define ON 0
#define OFF 1

unsigned char code patternArray[] = { 0xEF, 0xDF, 0xBF, 0x7F};
unsigned char bdata key;
sbit key_1 = key^1;
sbit key_2 = key^2;
sbit key_3 = key^3;
sbit key_4 = key^4;
sbit key_5 = key^5;
sbit key_6 = key^6;
sbit key_7 = key^7;

//0.25 s
void delayShort(void){
	unsigned char i;
	unsigned char j;
	
	for(i = 0; i < 94; i++){
		for(j = 0; j < 100; j++){
			;
		}
	}
}


void lightDiode(int row, int column){
	int i;
	for(i = 0; i < row; i++){
		P0_0 = ON;
		delayShort();
		P0_0 = OFF;
		delayShort();
	}
	//podwojne wywolanie delayShort = 0.5 s wraz z ostatnim wywolaniem
	//delayShort w ostatniej iteracji w powyzszym forze dalej 0.75 s
	delayShort();
	delayShort();
	
	for(i = 0; i < column; i++){
		P0_0 = ON;
		delayShort();
		P0_0 = OFF;
		delayShort();
	}
}

void decode(void){
	unsigned char column = 0;
	unsigned char row = 0;
	
	if (key_4 == 0){
		row = 1;
	} else if (key_5 == 0){
		row = 2;
	} else if (key_6 == 0){
		row = 3;
	} else if (key_7 == 0){
		row = 4;
	}
	
	if (key_1 == 0){
		column = 3;
	} else if (key_2 == 0) {
		column = 2;
	} else if (key_3 == 0) {
		column = 1;
	}
	
	lightDiode(row, column);
}

void main(void){
	unsigned char data index = 0;
	bit decodeEnable = 1;
	
	while(1){
		P2 = patternArray[index];
		key = P2;
		if((key_1 && key_2 && key_3) == 0){
			if(decodeEnable){
				decode();
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