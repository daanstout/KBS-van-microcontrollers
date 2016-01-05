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


void Jump::checkJump(){
	if(zbutton == 1) startJump();
	if(zbutton == 0) endJump();
<<<<<<< HEAD
//	if(positionY > 159 && in_air == true) in_air = false;
=======
	if(positionY == 160 && in_air == true) in_air = false;
>>>>>>> origin/Daan's-Workplace
	update();
	
}
void Jump::startJump(){
//  Serial.println("startjump voor IF");
//  Serial.println(in_air);
	if(!in_air){
		velocityY = -2.5;
		in_air = true;
	}
}

void Jump::endJump(){
//  Serial.println("endjump voor IF");
//  Serial.println(in_air);
//  Serial.println(velocityY);
	if(velocityY < -1.5){
		velocityY = -1.5;
//		in_air = false;
//    Serial.println("endjump na IF");
//    Serial.println(in_air);
	}
}

void Jump::update(){
	last_y = positionY;
	velocityY += gravity;
	positionY += velocityY;
  Serial.println(velocityY);
//  Serial.println("check 1");    KOMT ER WEL
	if(positionY >= 160.0){
//    Serial.println("check 2");  KOMT ER WEL
		positionY = 160.0;
		velocityY = 0.0;
		in_air = false;
//    Serial.println(in_air);     IS NUL/FALSE
	}
}
