#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>

MI0283QT9 lcd;
int zbutton, in_air, obstakelLocatie1, obstakelActief1, jumpLoopCount, i, toJump, last_x, x, gameStart, buttonPressed, gameIsLive, death;
int up = 50;
int directie = 1;
int current = 159;
int firstTime = 1;

void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

void obstakel(int x) {
  lcd.fillRect(x, 160 - (32), 1, 32, RGB(255, 0, 0)); //eerste rij genereren
}

void resetObstakel(int x) {
  lcd.fillRect(x + 32, 160 - (32), 1, 32, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}


void sidescroll() {
  if (obstakelActief1 == 0) {
    obstakelActief1 = 1;
    obstakelLocatie1 = 320;
  }
  if (obstakelActief1 == 1) {
    
    //obstakel(obstakelLocatie1);

    last_x = obstakelLocatie1;

    if (toJump == 0) {
      checkJump();
    }
    //_delay_ms(3);
    if (obstakelLocatie1 == -32) {

      obstakelLocatie1 = 320;
      obstakelActief1 = 0;
    }

    obstakelLocatie1--;
  }
  //resetObstakel(last_x);
}

void checkJump(){
  while (zbutton == 1) {
    if (zbutton == 1 && in_air == 0) {
      Serial.print("test speler");
      toJump = 1;
      zbutton = 0;
    }
  }
}

void speler() {
  lcd.fillCircle(32, 160 - 16, 16, RGB(0, 0, 255)); //bolletje
}


void jump(){
  in_air = 1;
  if(directie == 1){
    if(jumpLoopCount == 0){
      i = 0;
    }
    lcd.fillCircle(32, (current + 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
    lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omhoog
    _delay_ms(1);
    current--;  //speler tekent bolletje steeds opnieuw vind oplossing
    i++;
    jumpLoopCount++;
    if(i == up){
      directie = 0;
      jumpLoopCount = 0;
    }
  }else if(directie == 0){
    if(jumpLoopCount == 0){
      i = 0;
    }
    lcd.fillCircle(32, (current - 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
    lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omlaag
    _delay_ms(1);
    current++;
    i++;
    jumpLoopCount++;
    if(i == up){
      directie = 1;
      jumpLoopCount = 0;
      toJump = 0;
    }
  }
  in_air = 0;
}

void drawScores(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm
  
  lcd.fillRoundRect(10, 200, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(10, 200, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(9, 199, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(28, 205, "BACK", RGB(0,0,0), RGB(0,034,255), 2);
  //back knop
}

void checkButtonPress(){
  while(gameStart == 0){
    lcd.touchRead();
    if(lcd.touchZ() > 80){
      Serial.println(lcd.touchZ());
      Serial.println(lcd.touchX());
      Serial.println(lcd.touchY());
      Serial.println(" ");
      if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 60 && lcd.touchY() < 85){
        buttonPressed = 1;
      }
      if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 90 && lcd.touchY() < 115){
        buttonPressed = 2;
      }
      if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 120 && lcd.touchY() < 170){
        buttonPressed = 3;
      }
      if(lcd.touchX() > 10 && lcd.touchX() < 110 && lcd.touchY() > 200 && lcd.touchY() < 225){   //kijkt of er wordt gedrukt op back in scores
        buttonPressed = 4;
      }
    }
    if(buttonPressed != 0){
      gameStart = 1;
    }
  }
}

void drawMenu(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm
  
  lcd.drawText(10, 10, "THE NOT SO POSSIBLE", RGB(190,0,0), RGB(111,111,111), 2);
  lcd.drawText(125, 30, "GAME!", RGB(190,0,0), RGB(111,111,111), 2);
  //tekent de menu tekst met de game naam
  
  lcd.fillRoundRect(110, 60, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(110, 60, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 59, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(122, 65, "START", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de start knop
  
  lcd.fillRoundRect(110, 90, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(110, 90, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 89, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(113, 95, "SCORES", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de scores knop
  
  lcd.fillRoundRect(110, 120, 100, 25, 5, RGB(0,034,255));
  lcd.fillRoundRect(110, 145, 100, 25, 5, RGB(0,255,255));
  lcd.fillRect(110, 125, 100, 20, RGB(0,034,255));
  lcd.fillRect(110, 145, 100, 20, RGB(0,255,255));
  lcd.drawRoundRect(110, 120, 100, 50, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 119, 102, 52, 5, RGB(0,0,0));
  lcd.drawText(122, 125, "MULTI", RGB(0,0,0), RGB(0,034,255), 2);
  lcd.drawText(112, 150, "PLAYER", RGB(0,0,0), RGB(0,255,255), 2);
  //tekent de multiplayer knop
}

void teken(){
  if(obstakelLocatie1 != last_x){
    obstakel(obstakelLocatie1);
    resetObstakel(last_x);
  }
 
  
}


void game() {
  lcd.fillScreen(RGB(255,255,255)); // scherm leeg
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();
  speler();
  
  while(gameIsLive == 1){
    
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();
    
    sidescroll();
    teken();
    if(toJump == 1){
      jump();
    }
    if(death == 1){
      gameIsLive = 0;
    }
    
  }

}

int main() {
  init();
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal(); //calibrate touchpanel

  
 while(1){
    if(firstTime == 1){           //menu tekenen
      drawMenu();
      firstTime = 0;
    }
    
    if(buttonPressed == 1){
      gameIsLive = 1;
      game();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op play wordt gedrukt
    }
  
    if(buttonPressed == 2){
      drawScores();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op scores wordt gedrukt
    }
    
    if(buttonPressed == 3){
      //multiplayer
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op multiplayer wordt gedrukt
    }
    if(buttonPressed == 4){
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op back wordt gedrukt in scores
    }
    checkButtonPress();
  }

  return 0;
}

