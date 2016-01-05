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


void Jump::checkJump() {
Serial.begin(9600);
Serial.print("check");
  if (zbutton == 1) startJump();  
  if (zbutton == 0) endJump();
  //if (positionY > 160 && in_air == true) in_air = false;
  updateJump();
}
void Jump::startJump() {
  if (!in_air) {
    velocityY = -2.5;
    in_air = true;
    Serial.print("sjump");
  }
}

void Jump::endJump() {

  if (velocityY < -1.5) {
    velocityY = -1.5;
    in_air = false;
     Serial.print("ejump");
   
  }
}

void Jump::updateJump() {
  last_y = positionY;
  velocityY += gravity;
  positionY += velocityY;

  if (positionY >= 160.0) {
    positionY = 160.0;
    velocityY = 0.0;
    in_air = false;
  }
  Serial.print("ujump");
}
