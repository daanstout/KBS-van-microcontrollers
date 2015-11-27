
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"

MI0283QT9 lcd;

volatile uint16_t count = 0;
volatile uint16_t teller = 0;

ISR(TIMER2_OVF_vect) {
	teller++;
	if(teller >= 512){
		count++;
		
		teller = 0;
	}
}

void tekenAssen(){
	lcd.drawLine(lcd.getWidth()/2, 0, lcd.getWidth()/2, lcd.getHeight(), RGB(255, 0, 0)); //verticale lijn
	lcd.drawLine(0, lcd.getHeight()/2, lcd.getWidth(), lcd.getHeight()/2, RGB(255,0,0)); // horizontale lijn
}

int main(){
	init();
	
	TCCR2A |= (1 << CS02) | (1 << CS00);
	TIMSK2 |= (1 << TOIE0);
	
	
	TCNT2 = 0;
	sei();

	byte accx,accy,zbut,cbut,x,y, x1, y1;
	uint8_t tp_last_x;
	uint8_t tp_last_y;
	char c[128];
	
	lcd.begin();
	nunchuck_setpowerpins();
	nunchuck_init();
	
	//lcd.touchRead();
	//lcd.touchStartCal(); //calibreren
	lcd.fillScreen(RGB(255,255,255)); // scherm leeg
	tekenAssen();
	
	
	while(1){
		nunchuck_get_data();
		
		accx  = nunchuck_accelx(); // ranges from approx 70 - 182
		accy  = nunchuck_accely(); // ranges from approx 65 - 173
		zbut = nunchuck_zbutton();
		cbut = nunchuck_cbutton();
		x = nunchuck_joyx();
		y = nunchuck_joyy();
		
		if(x > 30 && x < 50){
			 x1 = lcd.getWidth()/4;
			 y1= lcd.getHeight()/2;
		}
		if(x > 200 && x < 255){
			 x1 =  lcd.getWidth()/4 +  lcd.getWidth()/2;
			 y1 = lcd.getHeight()/2;
		}
		if(y > 30 && y < 50){
			 y1 = lcd.getHeight()/4 + lcd.getHeight()/2;
			 x1 = lcd.getWidth()/2;
		}
		if(y > 200 && y < 255){
			 y1 = lcd.getHeight()/4;
			 x1 = lcd.getWidth()/2;
		}
		if((x > 120 && x < 140) && (y > 120 && y < 140)){
			y1 = lcd.getHeight()/2;
			x1 = lcd.getWidth()/2;
		}
		
		
		lcd.drawInteger(5, 5, count, DEC, RGB(0,0,0), RGB(255,255,255), 1);
		if(tp_last_x != x1 || tp_last_y != y1){
			lcd.fillCircle(tp_last_x, tp_last_y, 3, RGB(255,255,255));
		}
		
		sprintf(c, "X:%03i Y:%03i Z:%03i", x, y, lcd.touchZ());	//coordinaten opmaak
		lcd.drawText(100, 2, c, RGB(0,0,0), RGB(255,255,255), 1);						//coordinaten op lcd tekenen
		tekenAssen();
		lcd.fillCircle(x1, y1, 2, RGB(0,0,255));					//cirkel tekenen
		
		
		tp_last_x = x1;
		tp_last_y = y1;
		
	}

	return 0;
}




