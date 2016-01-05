#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>
#include <stdlib.h>
#include "Jump.h"
#include "Menu.h"
#include "Opmaak.h"
#include "Game.h"

MI0283QT9 lcd;

//variabelen:

//getallen:
Jump J;
Menu M;
Opmaak O;
Game G;





<<<<<<< HEAD
uint8_t randomObstakels = 0, randomObstakelVorm, aantalObstakels = 0, nieuwObstakel, obstakelVorm1 = 0, obstakelVorm2 = 0;    //level generatie variabelen
uint8_t moeilijkheid = 255;    //game variabelen
uint8_t currentY = 160;     //draw variabelen
uint16_t obstakelLocatie1, obstakelLocatie2, vorigeObstakel1, vorigeObstakel2;      //obstakel variabelen
=======
>>>>>>> origin/Daan's-Workplace





//booleans:

     //jump variabelen      moet een int zijn omdat de nunchuk functie 1 of 0 returned


//tekenen van de grond waar de speler op loopt


//obstakels:

//vierkant:


//randomlevel generatie:
<<<<<<< HEAD
void randomLevel() {
  if (aantalObstakels == 0) {
    randomObstakelVorm = (random(0, moeilijkheid)) + 1;
    aantalObstakels++;
    if (randomObstakelVorm <= 50) {
      obstakelVorm1 = 1;
    } else {
      obstakelVorm1 = 2;
    }
    obstakelLocatie1 = 320;
  }
  if (obstakelLocatie1 < 160 && aantalObstakels < 2) {
    nieuwObstakel = (random(0, 10)) + 1;
    if (nieuwObstakel == 1) {
      randomObstakelVorm = (random(0, moeilijkheid)) + 1;
      aantalObstakels++;
      if (randomObstakelVorm <= 50) {
        obstakelVorm2 = 1;
      } else {
        obstakelVorm2 = 2;
      }
      obstakelLocatie2 = 320;
    }
  }
}

//scrollen van het scherm:
void sidescroll() {
  if (aantalObstakels > 0) {
    vorigeObstakel1 = obstakelLocatie1;
    vorigeObstakel2 = obstakelLocatie2;

    J.checkJump();

    if (obstakelLocatie1 == -32) {
      if (geland) {
        M.score++;
        if(moeilijkheid > 100){
          moeilijkheid--;
        }
      } else if (!geland) {
        M.score += 2;
        if(moeilijkheid > 101){
          moeilijkheid -= 2;
        }
      }
      
      drawMoeilijkheid();
      
      lcd.fillRect(105, 210, 20, 20, RGB(255, 255, 255));
      lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);

      aantalObstakels--;

      if (aantalObstakels == 1) {
        obstakelLocatie1 = obstakelLocatie2;
        obstakelVorm1 = obstakelVorm2;
        obstakelVorm2 = 0;
      } else if (aantalObstakels == 0) {
        obstakelVorm1 = 0;
      }
    }
    obstakelLocatie1--;
    obstakelLocatie2--;
  }
}
=======


//scrollen van het scherm:
>>>>>>> origin/Daan's-Workplace




//het hertekenen van verschillende dingen:

<<<<<<< HEAD
void drawMoeilijkheid(){
  if(moeilijkheid == 255){
    for(uint16_t c = 195; c < 300; c += 25){
      lcd.drawCircle(c, 15, 10, RGB(0, 0, 0));
    }
  }else{
    if(moeilijkheid == 130 || moeilijkheid == 129){
      lcd.fillCircle(295, 15, 10, RGB(0, 0, 0));
    }else if(moeilijkheid == 160 || moeilijkheid == 159){
      lcd.fillCircle(270, 15, 10, RGB(0, 0, 0));
    }else if(moeilijkheid == 190 || moeilijkheid == 189){
      lcd.fillCircle(245, 15, 10, RGB(0, 0, 0));
    }else if(moeilijkheid == 220 || moeilijkheid == 219){
      lcd.fillCircle(220, 15, 10, RGB(0, 0, 0));
    }else if(moeilijkheid == 250 || moeilijkheid == 249){
      lcd.fillCircle(195, 15, 10, RGB(0, 0, 0));
    }
  }
}

void game() {
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, M.score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  drawMoeilijkheid();
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();
  speler();

  while (gameIsLive) {
    nunchuck_get_data();
    J.zbutton = nunchuck_zbutton();

    //checkJump();
    sidescroll();
    randomLevel();
    hitbox();
    teken();

    if (death) {
      gameIsLive = false;
    }
  }
=======
>>>>>>> origin/Daan's-Workplace





int main() {
  init();
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal();
  Serial.begin(9600);

  while (1){
    
    if (M.firstTime) {
      M.drawMenu(lcd);
      M.firstTime = false;
    }

    if (M.buttonPressed == 1) {
      G.gameIsLive = true;
      G.death = false;
      M.score = 0;
      G.game(lcd);
      M.firstTime = true;
      M.buttonPressed = 10;
      M.gameStart = false;
      M.toCheckButton = false;
    }
    if (M.buttonPressed == 2) {
      M.buttonPressed = 0;
      M.gameStart = false;
      M.drawScores(lcd);
      M.firstTime = true;
      M.toCheckButton = false;
    }
    if (M.buttonPressed == 3) {
      //multiplayer
      M.firstTime = true;
      M.buttonPressed = 0;
      M.gameStart = false;
    }
    if (M.buttonPressed == 4) {
      M.firstTime = true;
      M.buttonPressed = 0;
      M.gameStart = false;
      M.toCheckButton = false;
    }
    if (M.buttonPressed == 10) {
      M.postGame = true;
      M.buttonPressed = 0;
      M.gameStart = false;
      M.inputScore(lcd);
      M.firstTime = true;
      M.buttonPressed = 0;
      M.gameStart = false;
      M.postGame = false;
      M.toCheckButton = false;
      M.scoreSubmit = false;
    }

    if (M.toCheckButton) {
      M.checkButtonPress(lcd);
    }
    M.toCheckButton = true;
  }

  return 0;
}



