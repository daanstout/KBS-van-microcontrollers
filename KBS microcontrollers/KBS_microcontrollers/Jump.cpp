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


//kijkt of er moet worden gesprongen
void Jump::checkJump() {
  if (zbutton == 1) startJump();                                                                              //kijkt of de zbutton is ingedrukt van de nunchuck, zo ja, start de jump
  if (zbutton == 0) endJump();                                                                                //kijkt of de zbutton is ingedrukt van de nunchuck, zo ja, start de jump
}

//start de jump door waardes en booleans te setten
void Jump::startJump() {
  if (!in_air) {
    velocityY = -2.5;
    in_air = true;
  }
}

//kijk of de val snelheid onder een minimum komt en zo ja, zet hem weer terug
void Jump::endJump() {
  if (in_air) {
    if (velocityY < -1.5) {
      velocityY = -1.5;
    }
  }
}

//update de waardes, kijkt ook of de speler weer op de grond staat, en zo ja, zorgt dat hij niet door de vloer valt
void Jump::updateJump() {
  last_y = positionY;
  velocityY += gravity;
  positionY += velocityY;

  if (positionY >= 160.0) {
    positionY = 160.0;
    velocityY = 0.0;
    in_air = false;
  }
}

//tekent de speler in een jump
void Jump::tekenJump(MI0283QT9 lcd) {

  if (in_air) {
    if (velocityY <= 0) {   //als de snelheid negatief is wordt er omlaag getekend
      lcd.fillRect(32, positionY - 15, 15, 15 , RGB(0, 0, 0));
      lcd.fillRect(32, positionY, 15, last_y - positionY + 1, RGB(255, 255, 255));
    } else if (velocityY > 0 || positionY == 160) {   //als de snelheid positief is wordt er omhoog getekend
      lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
      lcd.fillRect(32, last_y - 15 - 1, 15, (positionY - 15) - (last_y - 15) + 1, RGB(255, 255, 255));
    } else if (positionY > 128) {       //als de speler is geland op een obstakel zet hij hem vast
      lcd.fillRect(32, positionY - 15, 15, 15, RGB(0, 0, 0));
      lcd.fillRect(32, last_y - 15 - 1, 15, (positionY - 15) - (last_y - 15) + 1, RGB(255, 255, 255));
    }
  }
}
