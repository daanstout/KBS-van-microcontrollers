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
#include <EEPROM.h>
#include "Menu.h"
#include "Opmaak.h"


//vergelijkt de scores van de speler, bij gelijk spel wordt dat ook gemeld
void Menu::compare() {
  if (score > score2) {
    multiplayerWinner = 2;
  } else if (score < score2) {
    multiplayerWinner = 1;
    score = score2;
  } else if (score == score2) {
    multiplayerWinner = 3;
  }
}

//tekent de scores.
void Menu::drawScores(MI0283QT9 lcd) {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  printScore(lcd);
  lcd.fillRoundRect(10, 200, 100, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(10, 200, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(9, 199, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(28, 205, "BACK", RGB(0, 0, 0), RGB(0, 034, 255), 2);
  //back knop
  while (!scoresBack) {
    checkButtonPress(lcd);
  }
  scoresBack = false;
}

//telt 1 op bij de score
void Menu::incScore() {
  score++;
}

//geeft de score
uint16_t Menu::getterScore() {
  return this->score;
}

//set de score
void Menu::setterScore(uint16_t x) {
  x = this->score;
}


//de char inputs in het game over menu:

void Menu::tekenVak1(MI0283QT9 lcd) {
  eerste2 = (String)eerste;
  lcd.fillRoundRect(90, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(90, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(89, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(98, 120, eerste2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(97, 110, 112, 110, 104, 102, RGB(0, 0, 0));
  lcd.fillTriangle(97, 145, 112, 145, 104, 153, RGB(0, 0, 0));
  //tekent het eerste vak om je initialen in te vullen
}

void Menu::tekenVak2(MI0283QT9 lcd) {
  tweede2 = (String)tweede;
  lcd.fillRoundRect(140, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(140, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(139, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(148, 120, tweede2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(147, 110, 162, 110, 154, 102, RGB(0, 0, 0));
  lcd.fillTriangle(147, 145, 162, 145, 154, 153, RGB(0, 0, 0));
  //tekent het tweede vak om je initialen in te vullen
}

void Menu::tekenVak3(MI0283QT9 lcd) {
  derde2 = (String)derde;
  lcd.fillRoundRect(190, 115, 30, 25, 5, RGB(255, 255, 255));
  lcd.drawRoundRect(190, 115, 30, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(189, 114, 32, 27, 5, RGB(0, 0, 0));
  lcd.drawText(198, 120, derde2, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillTriangle(197, 110, 212, 110, 204, 102, RGB(0, 0, 0));
  lcd.fillTriangle(197, 145, 212, 145, 204, 153, RGB(0, 0, 0));
  //tekent het derde vak om je initialen in te vullen
}


//leegt de eeprom
void Menu::emptyEEPROM() {
  Score leeg = {1 , 'A', 'B', 'C'};
  EEPROM.put(0, leeg);
  EEPROM.put(20, leeg);
  EEPROM.put(40, leeg);
  EEPROM.put(60, leeg);
  EEPROM.put(80, leeg);
}

//score opslaan op de plaats van de laagste score
void Menu::saveScore() {
  sortScore();
  EEPROM.put(0, nummer1);
  EEPROM.put(20, nummer2);
  EEPROM.put(40, nummer3);
  EEPROM.put(60, nummer4);
  EEPROM.put(80, nummer5);
  veranderd = true;
}
//scores ophalen uit de EEPROM
void Menu::getScore() {
  EEPROM.get(0, nummer1);
  EEPROM.get(20, nummer2);
  EEPROM.get(40, nummer3);
  EEPROM.get(60, nummer4);
  EEPROM.get(80, nummer5);
}

//nieuwe score invoegen en de rest een plek naar beneden zetten
void Menu::sortScore() {
  getScore();
  nummer = {getterScore() , eerste, tweede, derde};
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


//print de scores
void Menu::printScore(MI0283QT9 lcd) {
  getScore();
  lcd.drawText(90, 10, "HIGHSCORES", RGB(0, 0, 0), RGB(111, 111, 111), 2);             //HIGHSCORE schrijven
  sprintf(buf, "%c%c%c", nummer1.letter1, nummer1.letter2, nummer1.letter3);
  lcd.drawText(60, 37, "1.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 1 schrijven
  lcd.drawText(110, 37, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 37, nummer1.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer2.letter1, nummer2.letter2, nummer2.letter3);
  lcd.drawText(60, 62, "2.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 2 schrijven
  lcd.drawText(110, 62, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 62, nummer2.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer3.letter1, nummer3.letter2, nummer3.letter3);
  lcd.drawText(60, 87, "3.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                    //rank 3 schrijven
  lcd.drawText(110, 87, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 87, nummer3.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer4.letter1, nummer4.letter2, nummer4.letter3);
  lcd.drawText(60, 112, "4.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                   //rank 4 schrijven
  lcd.drawText(110, 112, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 112, nummer4.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

  sprintf(buf, "%c%c%c", nummer5.letter1, nummer5.letter2, nummer5.letter3);
  lcd.drawText(60, 137, "5.", RGB(0, 0, 0), RGB(111, 111, 111), 2);                   //rank 5 schrijven
  lcd.drawText(110, 137, buf, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 137, nummer5.punten, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);

}

//het game over scherm:
void Menu::inputScore(MI0283QT9 lcd) {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.drawText(82, 20, "GAME OVER", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 50, score , DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);
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

    if(multiplayerMode){
      if(multiplayerWinner == 1){
        lcd.drawText(25, 205, "Winnaar: player 1", RGB(0, 0, 0), RGB(111, 111, 111), 2);
      }else if(multiplayerWinner == 2){
        lcd.drawText(25, 205, "Winnaar: player 2", RGB(0, 0, 0), RGB(111, 111, 111), 2);
      }else if(multiplayerWinner == 3){
        lcd.drawText(75, 205, "Gelijk Spel", RGB(0, 0, 0), RGB(111, 111, 111), 2);
      }
    }
    
    //    while (scoreSubmit) {
    checkButtonPress(lcd);
    //      if (charVerandering == true) {
    gameStart = 0;
    charVerandering = false;
    // }
    _delay_ms(100);
    //}
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

    tekenVak1(lcd);
    tekenVak2(lcd);
    tekenVak3(lcd);

    //wacht tot de speler klaar is met zijn characters in te vullen.
    scoreSubmit = 1;
    while (scoreSubmit) {
      checkButtonPress(lcd);
      if (charVerandering == true) {
        gameStart = 0;
        tekenVak2(lcd);
        tekenVak1(lcd);
        tekenVak3(lcd);

        charVerandering = false;
      }
      _delay_ms(100);

    }
  }
  eerste = 'A', tweede = 'B', derde = 'C';
}
//tekent het main menu:
void Menu::drawMenu(MI0283QT9 lcd) {
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
  lcd.fillRoundRect(110, 175, 100, 25, 5, RGB(0, 034, 255));
  lcd.drawRoundRect(110, 175, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(109, 174, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(113, 180, "HOW TO", RGB(0, 0, 0), RGB(0, 034, 255), 2);
}

//tekent de knop tijdens de How To
void Menu::howToNext(MI0283QT9 lcd) {
  lcd.fillRoundRect(200, 200, 100, 25, 5, RGB(0, 255, 239));
  lcd.drawRoundRect(200, 200, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(199, 199, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(215, 205, "NEXT", RGB(0, 0, 0), RGB(0, 255, 239), 2);
}

//tekent de How To
void Menu::howTo(MI0283QT9 lcd) {
  lcd.fillScreen(RGB(255, 255, 255));
  lcd.drawText(10, 210, "Score:", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawInteger(105, 210, 5, DEC, RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
  lcd.fillRect(32, 145, 15, 15, RGB(0, 0, 0));
  lcd.fillRect(150, 128, 32, 32, RGB(255, 0, 0));
  for (uint16_t c = 195; c < 300; c += 25) {
    lcd.drawCircle(c, 15, 10, RGB(0, 0, 0));
  }

  lcd.drawText(18, 120, "Jij", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawText(98, 100, "Obstakels", RGB(0, 0, 0), RGB(255, 255, 255), 2);
  lcd.drawText(110, 35, "Moeilijkheid", RGB(0, 0, 0), RGB(255, 255, 255), 2);

  howToNext(lcd);

  _delay_ms(500);
  howToContinue = false;
  checkButtonPress(lcd);

  lcd.fillScreen(RGB(255, 255, 255));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
  lcd.fillRect(64, 128, 32, 32, RGB(255, 0, 0));
  lcd.fillTriangle(224, 160, 240, 128, 256, 160, RGB(255, 0, 0));

  lcd.drawText(40, 90, "Hier kun je", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(42, 105, "op landen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  lcd.drawText(180, 90, "Hier kun je niet", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(202, 105, "op landen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  lcd.drawText(70, 30, "Met de Z-knop spring je!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  howToNext(lcd);

  _delay_ms(500);
  howToContinue = false;
  gameStart = false;
  checkButtonPress(lcd);

  lcd.fillScreen(RGB(255, 255, 255));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));

  lcd.fillRect(144, 128, 32, 32, RGB(255, 0, 0));
  lcd.fillRect(150, 100, 15, 15, RGB(0, 0, 0));
  lcd.drawLine(128, 108, 143, 103, RGB(0, 0, 0));
  lcd.drawLine(128, 113, 143, 108, RGB(0, 0, 0));
  lcd.drawLine(128, 118, 143, 113, RGB(0, 0, 0));
  lcd.drawLine(180, 136, 210, 136, RGB(0, 0, 0));
  lcd.drawLine(180, 144, 210, 144, RGB(0, 0, 0));
  lcd.drawLine(180, 152, 210, 152, RGB(0, 0, 0));

  lcd.drawText(70, 60, "Spring over een vierkant", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(75, 70, "om 1 punt te krijgen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  howToNext(lcd);

  _delay_ms(500);
  howToContinue = false;
  gameStart = false;
  checkButtonPress(lcd);

  lcd.fillScreen(RGB(255, 255, 255));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));

  lcd.fillTriangle(144, 160, 160, 128, 176, 160, RGB(255, 0, 0));
  lcd.fillRect(170, 100, 15, 15, RGB(0, 0, 0));
  lcd.drawLine(148, 95, 163, 100, RGB(0, 0, 0));
  lcd.drawLine(148, 100, 163, 105, RGB(0, 0, 0));
  lcd.drawLine(148, 105, 163, 110, RGB(0, 0, 0));
  lcd.drawLine(180, 136, 210, 136, RGB(0, 0, 0));
  lcd.drawLine(180, 144, 210, 144, RGB(0, 0, 0));
  lcd.drawLine(180, 152, 210, 152, RGB(0, 0, 0));

  lcd.drawText(100, 60, "Spring over een driehoek", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(95, 70, "om 2 punten te krijgen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  howToNext(lcd);

  _delay_ms(500);
  howToContinue = false;
  gameStart = false;
  checkButtonPress(lcd);
}
//kijkt of er op een knop wordt gedrukt:
void Menu::checkButtonPress(MI0283QT9 lcd) {
  while (!gameStart) {      //loopt zolang er niet op een knop is gedrukt
    lcd.touchRead();
    if (lcd.touchZ() > 80) {    //de minimum drukkracht op het scherm nodig om een druk te registreren
      if(scherm == 0){
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 60 && lcd.touchY() < 85) {  //kijkt of er wordt gedrukt op start
          buttonPressed = 1;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 90 && lcd.touchY() < 115) { //kijkt of er wordt gedrukt op scores
          buttonPressed = 2;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 120 && lcd.touchY() < 170) { //kijkt of er wordt gedrukt op multiplayer
          buttonPressed = 3;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 175 && lcd.touchY() < 200) { //kijkt of er wordt gedrukt op how to
          buttonPressed = 5;
        }
      }
      if(scherm == 1){
        if (lcd.touchX() > 10 && lcd.touchX() < 110 && lcd.touchY() > 200 && lcd.touchY() < 225) { //kijkt of er wordt gedrukt op back in scores
          buttonPressed = 4;
          scoresBack = true;
        }
      }
      if(scherm == 3){
        if (lcd.touchX() > 200 && lcd.touchX() < 300 && lcd.touchY() > 200 && lcd.touchY() < 225) {
          howToContinue = true;
        }
      }
        //        if (lcd.touchX() > 270) {       // om naar score input scherm te gaan zonder de game te spelen
        //          buttonPressed = 10;
        //        }
      if(scherm == 4){
        if(lcd.touchX() > 116 && lcd.touchX() < 206 && lcd.touchY() > 149 && lcd.touchY() < 176){
          multiplayerBegin = true;
        }
      }
      if(scherm == 10){
        if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (eerste == 'Z') {
            eerste = 'A';
          } else {
            eerste++;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (eerste == 'A') {
            eerste = 'Z';
          } else {
            eerste--;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (tweede == 'Z') {
            tweede = 'A';
          } else {
            tweede++;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (tweede == 'A') {
            tweede = 'Z';
          } else {
            tweede--;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 70 && lcd.touchY() < 122) {
          if (derde == 'Z') {
            derde = 'A';
          } else {
            derde++;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 122 && lcd.touchY() < 175) {
          if (derde == 'A') {
            derde = 'Z';
          } else {
            derde--;
          }
          charVerandering = true;
        } else if (lcd.touchX() > 22 && lcd.touchX() < 192 && lcd.touchY() > 164 && lcd.touchY() < 189) {                     //save knop
          scoreSubmit = false;
          saveScore();
          charVerandering = true;
          score = 0;
        } else if (lcd.touchX() > 210 && lcd.touchX() < 290 && lcd.touchY() > 164 && lcd.touchY() < 189) {                    //quit knop
          scoreSubmit = false;
          veranderd = false;
          charVerandering = true;
          score = 0;
        }
      }
    }
    if (buttonPressed != 0 || charVerandering || howToContinue || multiplayerBegin) {  //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt de while loop
      gameStart = true;
    }
  }
}
