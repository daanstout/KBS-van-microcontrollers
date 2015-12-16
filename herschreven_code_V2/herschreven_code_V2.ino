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

MI0283QT9 lcd;

//variabelen:

//getallen:

uint8_t randomObstakels = 0, randomObstakelVorm, aantalObstakels = 0, nieuwObstakel, obstakelVorm1 = 0, obstakelVorm2 = 0;    //level generatie variabelen
uint8_t moeilijkheid = 5, rank = 0;     //game variabelen
uint8_t buttonPressed = 0;      //menu variabelen
uint8_t currentY = 160;     //draw variabelen
uint16_t obstakelLocatie1, obstakelLocatie2, vorigeObstakel1, vorigeObstakel2;      //obstakel variabelen
uint16_t score;     //game variabelen
double velocityY = 0.0, positionY = 160, last_y, gravity = 0.05;      //gravity variabelen

//namen:
//beide voor de score
char eerste = 'A', tweede = 'B', derde = 'C';
String eerste2, tweede2, derde2;

//booleans:

uint8_t zbutton, in_air = 0, keren;         //jump variabelen
uint8_t scoresBack = 0, scoreSubmit = 0, charVerandering = 0, gameStart = 0, postGame = 0;      //menu variabelen
uint8_t gameIsLive = 0, death = 0, geland = 0;      //game variabelen
uint8_t firstTime = 1, toCheckButton = 1;       //menu variabelen

//tekenen van de grond waar de speler op loopt
void tekenLijn(){
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

//obstakels:

//vierkant:
void drawVierkant(int x){
  lcd.drawLine(x, 160, x, 128, RGB(255, 0, 0)); //eerste rij genereren
}

void resetVierkant(int x) {
  lcd.drawLine(x + 32, 160, x + 32, 128, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}

//driehoek:
void drawDriehoek(int x) {
  lcd.drawLine(x, 160, x + 16, 128, RGB(200, 0, 0));
}

void resetDriehoek(int x) {
  lcd.drawLine(x + 16, 128, x + 32, 160, RGB(255, 255, 255));
}

//randomlevel generatie:
void randomLevel(){
  if(aantalObstakels == 0){
    randomObstakelVorm = (random(0, moeilijkheid)) + 1;
    aantalObstakels++;
    if(randomObstakelVorm == 1){
      obstakelVorm1 = 1;
    }else{
      obstakelVorm1 = 2;
    }
    obstakelLocatie1 = 320;
  }
  if(obstakelLocatie1 < 160 && aantalObstakels < 2){
    nieuwObstakel = (random(0, 3)) + 1;
    if(nieuwObstakel == 1){
      randomObstakelVorm = (random(0, moeilijkheid)) + 1;
      aantalObstakels++;
      if(randomObstakelVorm == 1){
        obstakelVorm2 = 1;
      }else{
        obstakelVorm2 = 2;
      }
      obstakelLocatie2 = 320;
    }
  }
}

//scrollen van het scherm:
void sidescroll(){
  if(aantalObstakels > 0){
    vorigeObstakel1 = obstakelLocatie1;
    vorigeObstakel2 = obstakelLocatie2;

    checkJump();
    
    if(obstakelLocatie1 == -32){
      if(geland == 1){
        score++;
      }else if(geland == 0){
        score += 2;
      }
      
      lcd.fillRect(105, 210, 20, 20, RGB(255, 255, 255));
      lcd.drawInteger(105, 210, score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);

      aantalObstakels--;
      
      if(aantalObstakels == 1){
        obstakelLocatie1 = obstakelLocatie2;
        obstakelVorm1 = obstakelVorm2;
        obstakelVorm2 = 0;
      }else if(aantalObstakels == 0){
        obstakelVorm1 = 0;
      }
    }
    obstakelLocatie1--;
    obstakelLocatie2--;
  }
}

//gravity en jump:
void checkJump(){
  if(zbutton == 1) StartJump();
  if(zbutton == 0) EndJump();

  Update();
}

void StartJump(){
  if(in_air == 0){
    velocityY = -2.5;
    in_air = 1;
  }
}

void EndJump(){
  if(velocityY < -1.5){
    velocityY = -1.5;
  }
}

void Update(){
  last_y = positionY;
  velocityY += gravity;
  positionY += velocityY;

  if(positionY > 160){
    positionY = 160;
    velocityY = 0;
    in_air = 0;
  }
}

void speler(){
  lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
}

//menu:

//scoreboard:

void drawScores(){
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.fillRoundRect(10, 200, 100, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(10, 200, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(9, 199, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(28, 205, "BACK", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //back knop
  while (scoresBack == 0) {
    checkButtonPress();
  }
  scoresBack = 0;
}

//de char inputs in het game over menu:

void tekenVak1(){
  eerste2 = (String)eerste;
  lcd.fillRoundRect(90, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(90, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(89, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(98, 120, eerste2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(97, 110, 112, 110, 104, 102, RGB(0, 0, 0));
  lcd.fillTriangle(97, 145, 112, 145, 104, 153, RGB(0, 0, 0));
  //tekent het eerste vak om je initialen in te vullen
}

void tekenVak2() {
  tweede2 = (String)tweede;
  lcd.fillRoundRect(140, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(140, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(139, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(148, 120, tweede2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(147, 110, 162, 110, 154, 102, RGB(0, 0, 0));
  lcd.fillTriangle(147, 145, 162, 145, 154, 153, RGB(0, 0, 0));
  //tekent het tweede vak om je initialen in te vullen
}

void tekenVak3() {
  derde2 = (String)derde;
  lcd.fillRoundRect(190, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(190, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(189, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(198, 120, derde2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(197, 110, 212, 110, 204, 102, RGB(0, 0, 0));
  lcd.fillTriangle(197, 145, 212, 145, 204, 153, RGB(0, 0, 0));
  //tekent het derde vak om je initialen in te vullen
}

//het game over scherm:
void inputScore(){
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.drawText(82, 20, "GAME OVER", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 50, score, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  //schrijft de tekst op het game over scherm als je dood gaat

  if(rank == 1) {
    lcd.drawText(80, 80, "HIGHSCORE!", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  } else if(rank < 6) {
    lcd.drawText(110, 80, "TOP 5!", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  }
  //meldt of je de highscore hebt of dat je in de top 5 bent gekomen

  lcd.fillRoundRect(22, 164, 170, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(22, 164, 170, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(21, 163, 172, 27, 5, RGB(0, 0, 0));
  lcd.drawText(27, 170, "SAVE SCORE", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //tekent de save score knop

  lcd.fillRoundRect(210, 164, 80, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(210, 164, 80, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(209, 163, 82, 27, 5, RGB(0, 0, 0));
  lcd.drawText(220, 170, "QUIT", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //tekent de quit knop

  tekenVak1();
  tekenVak2();
  tekenVak3();

  while (scoreSubmit == 0) {
    checkButtonPress();
    if (charVerandering == 1) {
      gameStart = 0;
  
      tekenVak1();
      tekenVak2();
      tekenVak3();
  
      charVerandering = 0;
    }
    _delay_ms(100);
  }
}

//tekent het main menu:
void drawMenu(){
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.drawText(10, 10, "THE NOT SO POSSIBLE", RGB(190, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawText(125, 30, "GAME!", RGB(190, 0, 0), RGB(111, 111, 111), 2);
  //tekent de menu tekst met de game naam

  lcd.fillRoundRect(110, 60, 100, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(110, 60, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(109, 59, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(122, 65, "START", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //tekent de start knop

  lcd.fillRoundRect(110, 90, 100, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(110, 90, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(109, 89, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(113, 95, "SCORES", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //tekent de scores knop

  lcd.fillRoundRect(110, 120, 100, 25, 5, RGB(0, 034, 255));
  lcd.fillRoundRect(110, 145, 100, 25, 5, RGB(0, 255, 255));
  lcd.fillRect(110, 125, 100, 20, RGB(0, 034, 255));
  lcd.fillRect(110, 145, 100, 20, RGB(0, 255, 255));
  lcd.drawRoundRect(110, 120, 100, 50, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(109, 119, 102, 52, 5, RGB(0, 0, 0));
  lcd.drawText(122, 125, "MULTI", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  lcd.drawText(112, 150, "PLAYER", RGB(0, 0, 0), RGB(0, 255, 255), 2);
  //tekent de multiplayer knop
}

//kijkt of er op een knop wordt gedrukt:
void checkButtonPress(){
  while (gameStart == 0) {      //loopt zolang er niet op een knop is gedrukt
    lcd.touchRead();
    if (lcd.touchZ() > 80) {    //de minimum drukkracht op het scherm nodig om een druk te registreren
      if (postGame == 0) {
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 60 && lcd.touchY() < 85) {  //kijkt of er wordt gedrukt op start
          buttonPressed = 1;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 90 && lcd.touchY() < 115) { //kijkt of er wordt gedrukt op scores
          buttonPressed = 2;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 120 && lcd.touchY() < 170) { //kijkt of er wordt gedrukt op multiplayer
          buttonPressed = 3;
        }
        if (lcd.touchX() > 10 && lcd.touchX() < 110 && lcd.touchY() > 200 && lcd.touchY() < 225) { //kijkt of er wordt gedrukt op back in scores
          buttonPressed = 4;
          scoresBack = 1;
        }
      }
      if (postGame == 1) {
        if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (eerste == 'Z') {
            eerste = 'A';
          } else {
            eerste++;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (eerste == 'A') {
            eerste = 'Z';
          } else {
            eerste--;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (tweede == 'Z') {
            tweede = 'A';
          } else {
            tweede++;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (tweede == 'A') {
            tweede = 'Z';
          } else {
            tweede--;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (derde == 'Z') {
            derde = 'A';
          } else {
            derde++;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (derde == 'A') {
            derde = 'Z';
          } else {
            derde--;
          }
          charVerandering = 1;
        } else if (lcd.touchX() > 22 && lcd.touchX() < 192 && lcd.touchY() > 164 && lcd.touchY() < 189) {
          scoreSubmit = 0;
          charVerandering = 1;
        } else if (lcd.touchX() > 210 && lcd.touchX() < 290 && lcd.touchY() > 164 && lcd.touchY() < 189) {
          scoreSubmit = 1;
          charVerandering = 1;
          Serial.println("test");
        }
      }
    }
    if (buttonPressed != 0 || charVerandering == 1) {  //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt de while loop
      gameStart = 1;
      break;
    }
  }
}

//het hertekenen van verschillende dingen:
void teken(){
  if(obstakelLocatie1 != vorigeObstakel1){
    if(obstakelVorm1 == 2){
      drawVierkant(obstakelLocatie1);
      resetVierkant(vorigeObstakel1);
    }
    if(obstakelVorm1 == 1){
      drawDriehoek(obstakelLocatie1);
      resetDriehoek(vorigeObstakel1);
    }
  }
  if(aantalObstakels == 2 && obstakelLocatie2 != vorigeObstakel2){
    if(obstakelVorm2 == 2){
      drawVierkant(obstakelLocatie2);
      resetVierkant(vorigeObstakel2);
    }
    if(obstakelVorm2 == 1){
      drawDriehoek(obstakelLocatie2);
      resetDriehoek(vorigeObstakel2);
    }
  }
  if (in_air == 1) {
    if (velocityY <= 0) {
      Serial.println("test1");
      lcd.fillRect(32, positionY - 15, 15, 15 , RGB(0, 0, 0));
      lcd.fillRect(32, positionY, 15, last_y - positionY + 1, RGB(255, 255, 255));
    } else if (velocityY > 0 || positionY == 160) {
      Serial.println("test2");
      lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
      lcd.fillRect(32, last_y - 15, 15, (positionY - 15) - (last_y - 15) , RGB(255, 255, 255));
    }
    keren = 0;
  } else if (keren != 1) {
    speler();
    keren = 1;
  }
  _delay_ms(2);
}

void game(){
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();
  speler();

  while(gameIsLive == 1){
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();
    
    //checkJump();
    sidescroll();
    randomLevel();
    teken();
    hitbox();

    if(death == 1){
      gameIsLive = 0;
    }
  }

  positionY = 160;
  obstakelLocatie1 = 0;
  obstakelLocatie2 = 0;
  aantalObstakels = 0;
  in_air = 0;
}

void hitbox(){
  if(obstakelVorm1 == 2){
    if(47 > obstakelLocatie1){
      if(positionY > 121 && positionY < 129){
        velocityY = 0;
        in_air = 0;
        positionY = 128;
        geland = 1;
      }
      if(positionY > 128){
        death++;
      }
    }
    if(32 < obstakelLocatie1 && geland == 1){
      in_air = 1;
    }
  }
  if(obstakelVorm1 == 1){
    if(47 > obstakelLocatie1){
      if(positionY > currentY){
        death++;
      }
      currentY -= 2;
    }
  }
  if(currentY == 128){
    currentY = 160;
  }
}

int main(){
  init();
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal();
  Serial.begin(9600);

  while(1){
    if(firstTime == 1){
      drawMenu();
      firstTime = 0;
    }

    if(buttonPressed == 1){
      gameIsLive = 1;
      death = 0;
      score = 0;
      in_air = 0;
      game();
      firstTime = 1;
      buttonPressed = 10;
      gameStart = 0;
      toCheckButton = 0;
    }
    if(buttonPressed == 2){
      buttonPressed = 0;
      gameStart = 0;
      drawScores();
      firstTime = 1;
      toCheckButton = 0;
    }
    if(buttonPressed == 3){
      //multiplayer
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
    }
    if(buttonPressed == 4){
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      toCheckButton = 0;
    }
    if(buttonPressed == 10){
      postGame = 1;
      buttonPressed = 0;
      gameStart = 0;
      inputScore();
      firstTime = 1;
      Serial.println("einde");
      buttonPressed = 0;
      gameStart = 0;
      postGame = 0;
      toCheckButton = 0;
      scoreSubmit = 0;
    }

    if(toCheckButton == 1){
      checkButtonPress();
    }
    toCheckButton = 1;
  }

  return 0;
}



