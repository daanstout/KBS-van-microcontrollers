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
Jump J;
Menu M;
Opmaak O;
Game G;

int main() {
  init();
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal();
  Serial.begin(9600);

  while (1) {

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
