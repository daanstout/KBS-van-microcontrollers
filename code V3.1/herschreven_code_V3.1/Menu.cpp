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
#include "Menu.h"


void Menu::compare() {
  if (score > score2) {
    multiplayerWinner = 2;
  } else if (score < score2) {
    multiplayerWinner = 1;
  } else if (score == score2) {
    multiplayerWinner = 3;
  }
}
void Menu::drawScores(MI0283QT9 lcd) {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

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

//het game over scherm:
void Menu::inputScore(MI0283QT9 lcd) {
  lcd.fillScreen(RGB(111, 111, 111));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 50, 0));
  //achtergrond van het scherm

  lcd.drawText(82, 20, "GAME OVER", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  lcd.drawInteger(200, 50, score, DEC, RGB(0, 0, 0), RGB(111, 111, 111), 2);
  //schrijft de tekst op het game over scherm als je dood gaat

  if (rank == 1) {
    lcd.drawText(80, 80, "HIGHSCORE!", RGB(0, 0, 0), RGB(111, 111, 111), 2);
  } else if (rank < 6) {
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

  tekenVak1(lcd);
  tekenVak2(lcd);
  tekenVak3(lcd);

  while (!scoreSubmit) {
    checkButtonPress(lcd);
    if (charVerandering) {
      gameStart = false;

      tekenVak1(lcd);
      tekenVak2(lcd);
      tekenVak3(lcd);

      charVerandering = false;
    }
    _delay_ms(100);
  }
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

void Menu::howToNext(MI0283QT9 lcd) {
  lcd.fillRoundRect(200, 200, 100, 25, 5, RGB(0, 255, 239));
  lcd.drawRoundRect(200, 200, 100, 25, 5, RGB(0, 0, 0));
  lcd.drawRoundRect(199, 199, 102, 27, 5, RGB(0, 0, 0));
  lcd.drawText(215, 205, "NEXT", RGB(0, 0, 0), RGB(0, 255, 239), 2);
}

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

  lcd.drawText(70, 60, "Spring er in 1 keer over", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(75, 70, "om 2 punten te krijgen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

  howToNext(lcd);

  _delay_ms(500);
  howToContinue = false;
  gameStart = false;
  checkButtonPress(lcd);

  lcd.fillScreen(RGB(255, 255, 255));
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));

  lcd.fillRect(144, 128, 32, 32, RGB(255, 0, 0));
  lcd.fillRect(150, 100, 15, 15, RGB(0, 0, 0));
  lcd.drawLine(128, 95, 143, 100, RGB(0, 0, 0));
  lcd.drawLine(128, 100, 143, 105, RGB(0, 0, 0));
  lcd.drawLine(128, 105, 143, 110, RGB(0, 0, 0));
  lcd.drawLine(180, 136, 210, 136, RGB(0, 0, 0));
  lcd.drawLine(180, 144, 210, 144, RGB(0, 0, 0));
  lcd.drawLine(180, 152, 210, 152, RGB(0, 0, 0));

  lcd.drawText(100, 60, "Land erop om 1", RGB(0, 0, 0), RGB(255, 255, 255), 1);
  lcd.drawText(95, 70, "punt te krijgen!", RGB(0, 0, 0), RGB(255, 255, 255), 1);

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
      if (postGame == false) {
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
          scoresBack = true;
        }
        if (lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 175 && lcd.touchY() < 200) { //kijkt of er wordt gedrukt op how to
          buttonPressed = 5;
        }
        if (lcd.touchX() > 200 && lcd.touchX() < 300 && lcd.touchY() > 200 && lcd.touchY() < 225) {
          howToContinue = true;
        }
        //        if (lcd.touchX() > 270) {       // om naar score input scherm te gaan zonder de game te spelen
        //          buttonPressed = 10;
        //        }
      }
      if (postGame == true) {
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
        } else if (lcd.touchX() > 22 && lcd.touchX() < 192 && lcd.touchY() > 164 && lcd.touchY() < 189) {
          scoreSubmit = true;
          charVerandering = true;
        } else if (lcd.touchX() > 210 && lcd.touchX() < 290 && lcd.touchY() > 164 && lcd.touchY() < 189) {
          scoreSubmit = true;
          charVerandering = true;
        }
      }
    }
    if (buttonPressed != 0 || charVerandering || howToContinue) {  //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt de while loop
      gameStart = true;
    }
  }
}
