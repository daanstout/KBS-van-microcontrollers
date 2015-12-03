#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"

MI0283QT9 lcd;

int zbutton = 0;
int grooteSpeler = 20;
int up = 98; //y waarde
int in_air = 0;
int playerHeight = (160 - grooteSpeler); //32=hoogte van een obstakel 10= ruimte tussen obstakel en speler tijdens sprong
int positionX = 32;
int positionY = 160;

void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}
void obstakel(int hoogte) {

  lcd.fillRect(64, 160 - 32, 32, 32 * hoogte, RGB(255, 0, 0)); //blokje genereren met de opgegeven hoogte

}
void speler() {

  lcd.fillRect(positionX, positionY - grooteSpeler, grooteSpeler, grooteSpeler, RGB(0, 0, 0));

}
void jump() {

}

int main() {

  init();
  lcd.begin();
  Serial.begin(9600);
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  nunchuck_setpowerpins();
  nunchuck_init();
  tekenLijn();
  speler();




  while (1) {
    //Serial.print("test main");
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();
    jump();

  }

  return 0;
}
