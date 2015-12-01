#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>


MI0283QT9 lcd;

void pixel(int x, int y, String kleur){
	lcd.drawRect(x,y,16,16,RGB(255,255,255));				//wit randje
	
	if(kleur == "groen"){								//onderste rij is groen
		lcd.fillRect(x,y,15,15,RGB(0,100,0));
		}
		else{
		lcd.fillRect(x,y,15,15,RGB(0,0,0));
	}
	
}

void vulScherm(){
	//for(int p = 0; p <= 240; p++){						//pixels verplaatsen
		for(int j = 0; j < 320; j += 16){				//pixels op alle plekken op de x-as zetten
			for(int k = 160; k < 240; k += 16){			//pixels op onderste rij y-as
				pixel(j,k, "groen");
			}
		}
		//if(p == 240){
		//	p = 0;
		//}
		//_delay_ms(500);
    //}
}

void tekenLijn(){
	lcd.fillRect(0, 160,320,32,RGB(0,100,0));
}

void obstakel(int hoogte){

  lcd.fillRect(319 - 32, 160 - (32*hoogte),32,32 * hoogte, RGB(255,0,0));    //blokje genereren met de opgegeven hoogte
  
}

void speler(){
  lcd.fillCircle(32, 160-16, 16, RGB(0,0,255));                     //bolletje
}

int main(){
	init();
	lcd.begin();
	lcd.fillScreen(RGB(255,255,255)); // scherm leeg
	
	tekenLijn();
	obstakel(2);
	speler();
	return 0;
}
