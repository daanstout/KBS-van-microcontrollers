#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>
#include <Time.h>
#include <stdlib.h>
#include <EEPROM.h>

MI0283QT9 lcd;
struct Score {
  uint16_t punten;
  char letter1;
  char letter2;
  char letter3;
};
//highscores
int eeAdress = 0, eeAdressKleinste;
Score nummer1, nummer2, nummer3, nummer4, nummer5, kleinste, nummer;
char buf[4];

//getallen:
uint16_t obstakelLocatie1, obstakelLocatie2, score, i, last_x, last_x2, x, topscore, obstakelBovenkant = 128, spelerRechterZijde = 47, current = 140, randomObstakel, randomAfstand, currentX, currentY = 160;
uint8_t up = 50, moeilijkheid = 5, grootteSpeler = 15, positionX = 32, aantalObstakels, aantalDriehoek = 0, aantalVierkant = 0, rank;
double positionY = 160, last_y, velocityY = 0.0, gravity = 0.05;

//namen:
char eerste = 'A', tweede = 'B', derde = 'C';
String eerste2, tweede2, derde2;

//booleans:
uint8_t firstTime = 1, top5 = 1, directie = 1, scoreSubmit = 1, eersteKeerScore = 1, toCheckButton = 1;
uint8_t zbutton, obstakelActief1, toJump, gameStart, buttonPressed, gameIsLive, death, postGame, charverandering, scoresBack, vierkant, driehoek, jumpPause, geland;
bool in_air = false;



void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

void obstakel(int x) {
  lcd.drawLine(x, 160, x,  160 - 32, RGB(255, 0, 0)); //eerste rij genereren
}

void resetObstakel(int x) {
  lcd.drawLine(x + 32, 160, x + 32, 160 - 32, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}

void spijker(int x) {
  uint8_t y = 160;
  lcd.drawLine(x, y, x + 16, y - 32, RGB(200, 0, 0));
}

void resetSpijker(int x) {
  uint8_t y = 160 - 32;
  lcd.drawLine(x + 16, y, x + 32, y + 32, RGB(255, 255, 255));
}

void randomLevel() {
  //  srand(time_t(NULL));
  if (aantalObstakels == 0) {
    randomObstakel = (random(0, moeilijkheid)) + 1;
    aantalObstakels++;
    if (randomObstakel == 1) {
      aantalDriehoek++;
      driehoek = 1;
    } else {
      aantalVierkant++;
      vierkant = 1;
    }
    obstakelLocatie1 = 320;
  }
  //  if (obstakelLocatie1 < 255 && aantalObstakels < 2) {
  //    uint8_t temprand = (random(0, 3)) + 1;
  //    if (temprand == 1) {
  //      randomObstakel = (random(0, moeilijkheid)) + 1;
  //      aantalObstakels++;
  //      if (randomObstakel == 1) {
  //        aantalDriehoek++;
  //      } else {
  //        aantalVierkant++;
  //      }
  //      if (aantalObstakels == 1) {
  //        obstakelLocatie2 = 320;
  //      }
  //    }
  //  }
}

void sidescroll() {
  if (aantalObstakels > 0) {

    //obstakel(obstakelLocatie1);

    last_x = obstakelLocatie1;

    checkJump();

    //_delay_ms(3);
    if (obstakelLocatie1 == -32) {
      if (geland == 1) {
        score++;
      }
      if (geland == 0) {
        score += 2;
      }
      lcd.fillRect(105, 210, 20, 20, RGB(255, 255, 255));
      lcd.drawInteger(105, 210, score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
      geland = 0;
      //      if (aantalObstakels == 2) {
      //        aantalObstakels--;
      //        obstakelLocatie1 = obstakelLocatie2;
      //        if (vierkant == 1) {
      //          aantalVierkant--;
      //        } else if (driehoek == 1) {
      //          aantalDriehoek--;
      //        }
      //        if (aantalVierkant == 1) {
      //          vierkant = 1;
      //          driehoek = 0;
      //        } else if (aantalDriehoek == 1) {
      //          vierkant = 1;
      //          driehoek = 1;
      //        }
      //      } else
      if (aantalObstakels == 1) {
        aantalObstakels--;
        if (vierkant == 1) {
          aantalVierkant--;
          vierkant = 0;
        } else if (driehoek == 1) {
          aantalDriehoek--;
          driehoek = 0;
        }
      }
    }

    obstakelLocatie1--;
  }
  //resetObstakel(last_x);
}

void checkJump() {

  if (zbutton == 1) StartJump();
  if (zbutton == 0) EndJump();

  Update();

}

void speler() {
  lcd.fillRect(positionX, positionY - grootteSpeler, grootteSpeler, grootteSpeler , RGB(0, 0, 0));
}

void StartJump() {

  if (!in_air) {
    velocityY = -2.5;
    in_air = true;
  }
}

void EndJump() {
  if (velocityY < -1.5) {
    velocityY = -1.5;

  }
}

void Update() {
  last_y = positionY;
  velocityY += gravity;
  positionY += velocityY;

  if (positionY > 160.0) {
    positionY = 160.0;
    velocityY = 0.0;
    in_air = false;
  }

}

void drawScores() {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm
  printScore();

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

void tekenVak1() {
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

void emptyEEPROM(){
  Score leeg = {1 ,'A', 'B', 'C'};
  EEPROM.put(0, leeg);
  EEPROM.put(20, leeg);
  EEPROM.put(40, leeg);
  EEPROM.put(60, leeg);
  EEPROM.put(80, leeg);
}

//score opslaan op de plaats van de laagste score
void saveScore() {
  sortScore();
  EEPROM.put(0, nummer1);
  EEPROM.put(20, nummer2);
  EEPROM.put(40, nummer3);
  EEPROM.put(60, nummer4);
  EEPROM.put(80, nummer5);
}
//scores ophalen uit de EEPROM
void getScore() {
  EEPROM.get(0, nummer1);
  EEPROM.get(20, nummer2);
  EEPROM.get(40, nummer3);
  EEPROM.get(60, nummer4);
  EEPROM.get(80, nummer5);
}

//nieuwe score invoegen en de rest een plek naar beneden zetten
void sortScore() {
  getScore();
  nummer = {score, eerste, tweede, derde};
  if (nummer.punten > nummer1.punten) {
    rank = 1;
    nummer5 = nummer4;
    nummer4 = nummer3;
    nummer3 = nummer2;
    nummer2 = nummer1;
    nummer1 = nummer;
  }
  else if (nummer.punten >= nummer2.punten && nummer.punten < nummer1.punten) {
    rank = 2;
    nummer5 = nummer4;
    nummer4 = nummer3;
    nummer3 = nummer2;
    nummer2 = nummer;
  }
  else if (nummer.punten >= nummer3.punten && nummer.punten < nummer2.punten) {
    rank = 3;
    nummer5 = nummer4;
    nummer4 = nummer3;
    nummer3 = nummer;
  }
  else if (nummer.punten >= nummer4.punten && nummer.punten < nummer3.punten) {
    rank = 4;
    nummer5 = nummer4;
    nummer4 = nummer;
  }
  else if (nummer.punten >= nummer5.punten && nummer.punten < nummer4.punten) {
    rank = 5;
    nummer5 = nummer;
  }
  else if (nummer.punten < nummer5.punten) {
    rank = 8;
  }
}

void printScore() {
  getScore();
  lcd.drawText(90, 10, "HIGHSCORES", RGB(0, 0, 0), RGB(111, 111, 111), 2);             //HIGHSCORE schrijven
  sprintf(buf, "%c%c%c", nummer1.letter1, nummer1.letter2, nummer1.letter3);
  lcd.drawText(60, 37, "1.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 1 schrijven
  lcd.drawText(110, 37, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(125, 37, nummer1.letter2, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(140, 37, nummer1.letter3, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 37, nummer1.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer2.letter1, nummer2.letter2, nummer2.letter3);
  lcd.drawText(60, 62, "2.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 2 schrijven
  lcd.drawText(110, 62, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(110, 62, nummer2.letter1, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(125, 62, nummer2.letter2, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(140, 62, nummer2.letter3, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 62, nummer2.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer3.letter1, nummer3.letter2, nummer3.letter3);
  lcd.drawText(60, 87, "3.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 3 schrijven
  lcd.drawText(110, 87, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(110, 87, nummer3.letter1, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(125, 87, nummer3.letter2, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(140, 87, nummer3.letter3, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 87, nummer3.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer4.letter1, nummer4.letter2, nummer4.letter3);
  lcd.drawText(60, 112, "4.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                   //rank 4 schrijven
  lcd.drawText(110, 112, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(110, 112, nummer4.letter1, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(125, 112, nummer4.letter2, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(140, 112, nummer4.letter3, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 112, nummer4.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  
  sprintf(buf, "%c%c%c", nummer5.letter1, nummer5.letter2, nummer5.letter3);
  lcd.drawText(60, 137, "5.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                   //rank 5 schrijven
  lcd.drawText(110, 137, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(110, 137, nummer5.letter1, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(125, 137, nummer5.letter2, RGB(0, 0, 0), RGB(111, 111, 111), 2);
//  lcd.drawText(140, 137, nummer5.letter3, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 137, nummer5.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

}

void inputScore() {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.drawText(82, 20, "GAME OVER", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 50, score, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  //schrijft de tekst op het game over scherm als je dood gaat

  sortScore();
  if (rank == 1) {
    lcd.drawText(80, 80, "HIGHSCORE!", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  } else if (rank < 6) {
    lcd.drawText(110, 80, "TOP 5!", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  } else if (rank == 8) {
    lcd.drawText(55, 80, "NO HIGHSCORE", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  }
  //meldt of je de highscore hebt of dat je in de top 5 bent gekomen

  if (rank == 8) {
    lcd.fillRoundRect(110, 164, 80, 25, 5, RGB(0, 034, 255));
    lcd.drawRoundRect(110, 164, 80, 25, 5, RGB(0, 0, 0));
    lcd.drawRoundRect(109, 163, 82, 27, 5, RGB(0, 0, 0));
    lcd.drawText(120, 170, "QUIT", RGB(0, 0, 0), RGB(0, 034, 255), 2);
    //tekent de quit knop
    scoreSubmit = 1;
    
    while (scoreSubmit) {
      checkButtonPress();
      if (charverandering == 1) {
        gameStart = 0;
        charverandering = 0;
      }
      _delay_ms(100);
    }
  } else {
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

    scoreSubmit = 1;
    while (scoreSubmit) {
      checkButtonPress();
      if (charverandering == 1) {
        gameStart = 0;
        tekenVak2();
        tekenVak1();
        tekenVak3();

        charverandering = 0;
      }
      _delay_ms(100);
    }
  }
  eerste = 'A', tweede = 'B', derde = 'C';
  Serial.println("check1");
}

void checkButtonPress() {
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
        if (lcd.touchX() > 280) {
          buttonPressed = 10;
        }
      }
      if (postGame == 1) {
        if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (eerste == 'Z') {
            eerste = 'A';
          } else {
            eerste++;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (eerste == 'A') {
            eerste = 'Z';
          } else {
            eerste--;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (tweede == 'Z') {
            tweede = 'A';
          } else {
            tweede++;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (tweede == 'A') {
            tweede = 'Z';
          } else {
            tweede--;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (derde == 'Z') {
            derde = 'A';
          } else {
            derde++;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (derde == 'A') {
            derde = 'Z';
          } else {
            derde--;
          }
          charverandering = 1;
        } else if (lcd.touchX() > 22 && lcd.touchX() < 192 && lcd.touchY() > 164 && lcd.touchY() < 189) {     // save score button
          scoreSubmit = 0;
          saveScore();
          charverandering = 1;
        } else if (lcd.touchX() > 210 && lcd.touchX() < 290 && lcd.touchY() > 164 && lcd.touchY() < 189) {
          scoreSubmit = 0;
          charverandering = 1;
        }
      }
    }
    if (buttonPressed != 0 || charverandering == 1) {  //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt
      gameStart = 1;
    }
  }
}

void drawMenu() {
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

void teken() {
  uint8_t keren;
  if (obstakelLocatie1 != last_x) {
    if (vierkant == 1) {
      obstakel(obstakelLocatie1);
      resetObstakel(last_x);
    }
    if (driehoek == 1) {
      spijker(obstakelLocatie1);
      resetSpijker(last_x);
    }
  }
  //  if (aantalObstakels == 2 && obstakelLocatie2 != last_x2) {
  //    if (vierkant == 1) {
  //      if (aantalVierkant == 2) {
  //        obstakel(obstakelLocatie2);
  //        resetObstakel(last_x2);
  //      } else if (aantalDriehoek == 1) {
  //        spijker(obstakelLocatie2);
  //        resetSpijker(last_x2);
  //      }
  //    } else if (driehoek == 1) {
  //      if (aantalVierkant == 1) {
  //        obstakel(obstakelLocatie2);
  //        resetObstakel(last_x2);
  //      } else if (aantalDriehoek == 2) {
  //        spijker(obstakelLocatie2);
  //        resetSpijker(last_x2);
  //      }
  //    }
  //  }
  if (in_air) {
    if (velocityY <= 0) {
      lcd.fillRect(positionX, positionY - grootteSpeler, grootteSpeler, grootteSpeler , RGB(0, 0, 0));
      lcd.fillRect(positionX, positionY, grootteSpeler , last_y - positionY + 1, RGB(255, 255, 255));
    } else if (velocityY > 0) {
      lcd.fillRect(positionX, positionY - grootteSpeler, grootteSpeler, grootteSpeler , RGB(0, 0, 0));
      lcd.fillRect(positionX, (last_y - grootteSpeler) - 1, grootteSpeler , ((positionY - grootteSpeler) - (last_y - grootteSpeler)) + 1 , RGB(255, 255, 255));
    }
    //_delay_ms(4);

  }
  if (!in_air) {
    speler();
    lcd.fillRect(positionX, positionY - grootteSpeler - 7, grootteSpeler , 7 , RGB(255, 255, 255));
  }
}

void game() {
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, score, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();
  speler();

  while (gameIsLive == 1) {
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();

    sidescroll();
    randomLevel();
    teken();

    hitbox();
    if (death == 1) {
      gameIsLive = 0;
    }

  }
  current = 140;
  obstakelLocatie1 = 0;
  obstakelActief1 = 0;
  toJump = 0;
  in_air = 0;
  directie = 1;
  //alles resetten
}

void hitbox() {
  if (vierkant == 1) {
    if (spelerRechterZijde > obstakelLocatie1) {
      if (positionY > 121 && positionY < 129) {
        velocityY = 0.0;
        in_air = false;
        positionY = 128;
        geland = 1;
      }
      if (positionY > obstakelBovenkant) {
        death++;
      }
    }
    if (32 < obstakelLocatie1 && geland == 1) {
      in_air = true;
    }
  }
  if (driehoek == 1) {
    if (spelerRechterZijde > obstakelLocatie1) {
      if (positionY > currentY) {
        death++;
      }
      currentY -= 4;
    }
  }
  if (currentY == 128) {
    currentY = 160;
  }
}

int main() {
  init();
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal(); //calibrate touchpanel
  Serial.begin(9600);
  emptyEEPROM();
  while (1) {
    if (firstTime == 1) {
      drawMenu();                //drawed het menu
      firstTime = 0;
    }

    if (buttonPressed == 1) {
      gameIsLive = 1;
      death = 0;
      score = 0;
      game();
      firstTime = 1;
      buttonPressed = 10;
      gameStart = 0;
      toCheckButton = 0;
      //dit gebeurt er als er op play wordt gedrukt
    }
    if (buttonPressed == 2) {
      buttonPressed = 0;
      gameStart = 0;
      drawScores();
      firstTime = 1;
      toCheckButton = 0;
      //dit gebeurt er als er op scores wordt gedrukt
    }
    if (buttonPressed == 3) {
      //multiplayer
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op multiplayer wordt gedrukt
    }
    if (buttonPressed == 4) {
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      toCheckButton = 0;
      //dit gebeurt er als er op back wordt gedrukt in scores
    }
    if (buttonPressed == 10) {
      postGame = 1;
      buttonPressed = 0;
      gameStart = 0;
      inputScore();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      postGame = 0;
      toCheckButton = 0;
      //tijdelijk, input van je naam scherm
    }

    if (toCheckButton == 1) {
      checkButtonPress();        //checked of er wordt gedrukt op een knop en kijkt waar dat is gebeurt
    }

    toCheckButton = 1;
  }

  return 0;
}

