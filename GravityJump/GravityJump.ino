#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"

MI0283QT9 lcd;

int zbutton = 0;
int grooteSpeler = 20;
int up = 98; //y waarde
bool in_air = false;
int playerHeight = (160 - grooteSpeler); //32=hoogte van een obstakel 10= ruimte tussen obstakel en speler tijdens sprong
double positionX = 32;
double positionY = 160;
int yLast;
double velocityY = 0.0;
//int velocityX = 4.0;
double gravity = 0.5;

//class Player {
//public:
//	void jump();
//	void update();
//};

void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));
}

void speler() {
  if(in_air){
  yLast = positionY;
  
  lcd.fillRect(positionX, positionY - grooteSpeler, grooteSpeler, grooteSpeler , RGB(0, 0, 0));
  lcd.fillRect(positionX, positionY - grooteSpeler, grooteSpeler ,grooteSpeler , RGB(255, 255, 255));
  
 // lcd.fillRect(positionX, yLast - grooteSpeler, grooteSpeler ,grooteSpeler , RGB(255, 255, 255));  
  }
  if(!in_air){
  
  lcd.fillRect(positionX, positionY - grooteSpeler, grooteSpeler, grooteSpeler, RGB(0, 0, 0));
  }
}


int main() {
  init();
  lcd.begin();
  Serial.begin(9600);
  lcd.fillScreen(RGB(255, 255, 255)); // scherm leeg
  nunchuck_setpowerpins();
  nunchuck_init();
  tekenLijn();


  while (1) {
    //Serial.print("test main");
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();
    if(zbutton == 1) StartJump();
    if(zbutton == 0) EndJump();
    
    Update();
    speler();
  
}

  return 0;
}

void StartJump(){
  
    if(!in_air){
        velocityY = -12.0;
        in_air = true; 
    }
}

void EndJump()
{
    if(velocityY < -6.0){
        velocityY = -6.0;
       
}
}

void Update(){
    velocityY += gravity;
    positionY += velocityY;
   // positionX += velocityX;
    
    if(positionY > 160.0){
        positionY = 160.0;
        velocityY = 0.0;
        in_air = false;
    }
    
   //if(positionX < 10 || positionX > 190)
     //  velocityX *= -1;
}

//void Render()
//{
//    ctx.clearRect(0, 0, 200, 200);
//    ctx.moveTo(0,175);
//    ctx.lineTo(200,175);
//    ctx.stroke();
//    ctx.beginPath();
//    ctx.moveTo(positionX - 10, positionY - 20);
//    ctx.lineTo(positionX + 10, positionY - 20);
//    ctx.lineTo(positionX + 10, positionY);
//    ctx.lineTo(positionX - 10, positionY);
//    ctx.closePath();
//    ctx.stroke(); 
//}
