#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>


MI0283QT9 lcd;

void pixel(int x, int y, String kleur){
	lcd.drawRect(x,y,8,8,RGB(255,255,255));
	
	if(kleur == "groen"){
		lcd.fillRect(x,y,7,7,RGB(0,100,0));
		}else{
		lcd.fillRect(x,y,7,7,RGB(0,0,0));
	}
	
}

void vulScherm(){
	for(int p = 0; p <= 240; p++){						//pixels verplaatsen
		for(int j = p; j < 320; j += 8){				//pixels op alle plekken op de x-as zetten
			for(int i = 0; i < 40; i += 8){				//pixels op bovenste rij y-as
				pixel(j,i, "zwart");
			}
			for(int k = 160; k < 240; k += 8){			//pixels op onderste rij y-as
				pixel(j,k, "groen");
			}
		}
		if(p == 240){
			p = 0;
		}
		//_delay_ms(500);
	}
}

int main(){
	init();
	lcd.begin();
	lcd.fillScreen(RGB(255,255,255)); // scherm leeg
	
	vulScherm();
	
	
	return 0;
}
