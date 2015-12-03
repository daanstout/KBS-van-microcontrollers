#include <avr/io.h>
#include <Wire.h>
#include <SPI.h>
#include <digitalWriteFast.h>
#include <GraphicsLib.h>
#include <MI0283QT9.h>
#include <Arduino.h>
#include "nunchuck_funcs.h"
#include <util/delay.h>

MI0283QT9 lcd;
int up = 45;
int zbutton = 0;
int in_air = 0;
int obstakelLocatie1 = 0;
int obstakelActief1 = 0;
int directie = 1;
int current = 159;
int jumpLoopCount = 0;
int i = 0;
int toJump = 0;
int gameStart = 0;
int buttonPressed = 0;
int firstTime = 1;
int gameIsLive = 0;
int death = 0;
int postGame = 0;
int topscore = 0;
int top5 = 1;
int scoreSubmit = 1;
char eerste = 'A';
char tweede = 'B';
char derde = 'C';
String eerste2;
String tweede2;
String derde2;
int charverandering = 0;
int eersteKeerScore;

void pixel(int x, int y, String kleur){
  lcd.drawRect(x,y,16,16,RGB(255,255,255));       //wit randje
  
  if(kleur == "groen"){               //onderste rij is groen
    lcd.fillRect(x,y,15,15,RGB(0,100,0));
    }
    else{
    lcd.fillRect(x,y,15,15,RGB(0,0,0));
  }
  
}

void vulScherm(){
  //for(int p = 0; p <= 240; p++){            //pixels verplaatsen
    for(int j = 0; j < 320; j += 16){       //pixels op alle plekken op de x-as zetten
      for(int k = 160; k < 240; k += 16){     //pixels op onderste rij y-as
        pixel(j,k, "groen");
      }
    }
    //if(p == 240){
    //  p = 0;
    //}
    //_delay_ms(500);
    //}
}

void tekenLijn(){
  lcd.fillRect(0, 160,320,32,RGB(0,100,0));
}

void obstakel(int x){
  lcd.fillRect(x, 160 - (32),1,32, RGB(255,0,0));    //eerste rij genereren 
}
void resetObstakel(int x){
  lcd.fillRect(x + 32, 160 - (32),1,32, RGB(255,255,255));    //laatste rij van obstakel resetten
}



void sidescroll(){
   int last_x;
   int x;
   
   if(obstakelActief1 == 0){
     obstakelActief1 = 1;
     obstakelLocatie1 = 320;
   }
   if(obstakelActief1 == 1){
     resetObstakel(last_x);
     obstakel(obstakelLocatie1);
     
     last_x = obstakelLocatie1;
     
     nunchuck_get_data();
     zbutton = nunchuck_zbutton();
     if(toJump == 0){
       speler();
     }
     //_delay_ms(0);
      if(obstakelLocatie1 == -32){
        
        obstakelLocatie1 = 320;
        obstakelActief1 = 0;
      }
      
      obstakelLocatie1--;
   }
   
//   for(x = 320; x >= -32 ; x--){
     resetObstakel(last_x);
//      obstakel(x);
//
//      last_x = x;
//
//
//      
//      nunchuck_get_data();
//      zbutton = nunchuck_zbutton();
//      speler();
//      
//      _delay_ms(0);
//      if(x == -32){
//        
//        x = 320;
//      }
//   }
  
}
void speler(){
  lcd.fillCircle(32, 160-16, 16, RGB(0,0,255)); //bolletje
  while(zbutton == 1){
  if (zbutton == 1 && in_air == 0) {
    Serial.print("test speler");
    toJump = 1;
    zbutton = 0;
  }
  }
}

void jump(){
  in_air = 1;
  if(directie == 1){
    if(jumpLoopCount == 0){
      i = 0;
    }
    lcd.fillCircle(32, (current + 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
    lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omhoog
    _delay_ms(1);
    current--;  //speler tekent bolletje steeds opnieuw vind oplossing
    i++;
    jumpLoopCount++;
    if(i == up){
      directie = 0;
      jumpLoopCount = 0;
    }
  }else if(directie == 0){
    if(jumpLoopCount == 0){
      i = 0;
    }
    lcd.fillCircle(32, (current - 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
    lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omlaag
    _delay_ms(1);
    current++;
    i++;
    jumpLoopCount++;
    if(i == up){
      directie = 1;
      jumpLoopCount = 0;
      toJump = 0;
    }
  }
//  for(int i = 0; i <= up; i++){
//   lcd.fillCircle(32, (current + 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
//  lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omhoog
//  _delay_ms(1);
//  current--;  //speler tekent bolletje steeds opnieuw vind oplossing
//  }
//  
//  for(int i = 0; i <= up; i++){
//   lcd.fillCircle(32, (current - 1)-16, 16, RGB(255,255,255)); //verwijder vorige ball
//   lcd.fillCircle(32, current-16, 16, RGB(0,0,255));  //ball omlaag
//  _delay_ms(1);
//  current++;
//  }

  //omgekeerd naar benede
  in_air = 0;

  }
  
  
void drawMenu(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm
  
  lcd.drawText(10, 10, "THE NOT SO POSSIBLE", RGB(190,0,0), RGB(111,111,111), 2);
  lcd.drawText(125, 30, "GAME!", RGB(190,0,0), RGB(111,111,111), 2);
  //tekent de menu tekst met de game naam
  
  lcd.fillRoundRect(110, 60, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(110, 60, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 59, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(122, 65, "START", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de start knop
  
  lcd.fillRoundRect(110, 90, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(110, 90, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 89, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(113, 95, "SCORES", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de scores knop
  
  lcd.fillRoundRect(110, 120, 100, 25, 5, RGB(0,034,255));
  lcd.fillRoundRect(110, 145, 100, 25, 5, RGB(0,255,255));
  lcd.fillRect(110, 125, 100, 20, RGB(0,034,255));
  lcd.fillRect(110, 145, 100, 20, RGB(0,255,255));
  lcd.drawRoundRect(110, 120, 100, 50, 5, RGB(0,0,0));
  lcd.drawRoundRect(109, 119, 102, 52, 5, RGB(0,0,0));
  lcd.drawText(122, 125, "MULTI", RGB(0,0,0), RGB(0,034,255), 2);
  lcd.drawText(112, 150, "PLAYER", RGB(0,0,0), RGB(0,255,255), 2);
  //tekent de multiplayer knop
}

void game(){
  lcd.fillScreen(RGB(255,255,255)); // scherm leeg
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();

  while(gameIsLive == 1){
    zbutton = nunchuck_zbutton();
    nunchuck_get_data();
    sidescroll();
    if(toJump == 1){
      jump();
    }
    if(death == 1){
      gameIsLive = 0;
    }
  }
}

void checkButtonPress(){
  while(gameStart == 0){        //loopt zolang er niet op een knop is gedrukt
    lcd.touchRead();
    if(lcd.touchZ() > 80){      //de minimum drukkracht op het scherm nodig om een druk te registreren
      if(postGame == 0){
        if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 60 && lcd.touchY() < 85){    //kijkt of er wordt gedrukt op start
          buttonPressed = 1;
        }
        if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 90 && lcd.touchY() < 115){   //kijkt of er wordt gedrukt op scores
          buttonPressed = 2;
        }
        if(lcd.touchX() > 110 && lcd.touchX() < 210 && lcd.touchY() > 120 && lcd.touchY() < 170){  //kijkt of er wordt gedrukt op multiplayer
          buttonPressed = 3;
        }
        if(lcd.touchX() > 10 && lcd.touchX() < 110 && lcd.touchY() > 200 && lcd.touchY() < 225){   //kijkt of er wordt gedrukt op back in scores
          buttonPressed = 4;
        }
        if(lcd.touchX() > 280){
          buttonPressed = 10;
        }
      }
      if(postGame == 1){
        if(lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 70 && lcd.touchY() < 122){
          if(eerste == 'Z'){
            eerste = 'A';
          }else{
            eerste++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 122 && lcd.touchY() < 175){
          if(eerste == 'A'){
            eerste = 'Z';
          }else{
            eerste--;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 70 && lcd.touchY() < 122){
          if(tweede == 'Z'){
            tweede = 'A';
          }else{
            tweede++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 122 && lcd.touchY() < 175){
          if(tweede == 'A'){
            tweede = 'Z';
          }else{
            tweede--;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 70 && lcd.touchY() < 122){
          if(derde == 'Z'){
            derde = 'A';
          }else{
            derde++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 122 && lcd.touchY() < 175){
          if(derde == 'A'){
            derde = 'Z';
          }else{
            derde--;
          }
          charverandering = 1;
        }
      }
    }
    if(buttonPressed != 0 || charverandering == 1){    //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt
      gameStart = 1;
    }
  }
}

void drawScores(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm

  lcd.fillRoundRect(10, 200, 100, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(10, 200, 100, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(9, 199, 102, 27, 5, RGB(0,0,0));
  lcd.drawText(28, 205, "BACK", RGB(0,0,0), RGB(0,034,255), 2);
  //back knop
}

void inputScore(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm
  
  lcd.drawText(82, 20, "GAME OVER", RGB(0,0,0), RGB(111,111,111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0,0,0), RGB(111,111,111), 2);
  lcd.drawInteger(200, 50, 10, DEC, RGB(0,0,0), RGB(111,111,111), 2);
  
  if(topscore == 1){
    lcd.drawText(80, 80, "HIGHSCORE!", RGB(0,0,0), RGB(111,111,111), 2);
  }else if(top5 == 1){
    lcd.drawText(110, 80, "TOP 5!", RGB(0,0,0), RGB(111,111,111), 2);
  }
  eerste2 = (String)eerste;
  lcd.fillRoundRect(70, 110, 50, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(70, 110, 50, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(69, 109, 52, 27, 5, RGB(0,0,0));
  lcd.drawText(75, 115, eerste2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.drawLine(95, 110, 95, 135, RGB(0,0,0));
  lcd.drawLine(95, 122, 120, 122, RGB(0,0,0));
  lcd.fillTriangle(100, 120, 115, 120, 107, 112, RGB(0,0,0));
  lcd.fillTriangle(100, 125, 115, 125, 107, 133, RGB(0,0,0));
  //tekent het eerste vak om je initialen in te vullen
  
  tweede2 = (String)tweede;
  lcd.fillRoundRect(130, 110, 50, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(130, 110, 50, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(129, 109, 52, 27, 5, RGB(0,0,0));
  lcd.drawText(135, 115, tweede2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.drawLine(155, 110, 155, 135, RGB(0,0,0));
  lcd.drawLine(155, 122, 180, 122, RGB(0,0,0));
  lcd.fillTriangle(160, 120, 175, 120, 167, 112, RGB(0,0,0));
  lcd.fillTriangle(160, 125, 175, 125, 167, 133, RGB(0,0,0));
  //tekent het tweede vak om je initialen in te vullen
  
  derde2 = (String)derde;
  lcd.fillRoundRect(190, 110, 50, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(190, 110, 50, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(189, 109, 52, 27, 5, RGB(0,0,0));
  lcd.drawText(195, 115, derde2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.drawLine(215, 110, 215, 135, RGB(0,0,0));
  lcd.drawLine(215, 122, 240, 122, RGB(0,0,0));
  lcd.fillTriangle(220, 120, 235, 120, 227, 112, RGB(0,0,0));
  lcd.fillTriangle(220, 125, 235, 125, 227, 133, RGB(0,0,0));
  
  scoreSubmit = 1;
  while(scoreSubmit){
    checkButtonPress();
    if(charverandering == 1){
      gameStart = 0;
      eerste2 = (String)eerste;
      lcd.fillRoundRect(70, 110, 50, 25, 5, RGB(255,255,255));
      lcd.drawRoundRect(70, 110, 50, 25, 5, RGB(0,0,0));
      lcd.drawRoundRect(69, 109, 52, 27, 5, RGB(0,0,0));
      lcd.drawText(75, 115, eerste2, RGB(0,0,0), RGB(255,255,255), 2);
      lcd.drawLine(95, 110, 95, 135, RGB(0,0,0));
      lcd.drawLine(95, 122, 120, 122, RGB(0,0,0));
      lcd.fillTriangle(100, 120, 115, 120, 107, 112, RGB(0,0,0));
      lcd.fillTriangle(100, 125, 115, 125, 107, 133, RGB(0,0,0));
      //tekent het eerste vak om je initialen in te vullen
      
      tweede2 = (String)tweede;
      lcd.fillRoundRect(130, 110, 50, 25, 5, RGB(255,255,255));
      lcd.drawRoundRect(130, 110, 50, 25, 5, RGB(0,0,0));
      lcd.drawRoundRect(129, 109, 52, 27, 5, RGB(0,0,0));
      lcd.drawText(135, 115, tweede2, RGB(0,0,0), RGB(255,255,255), 2);
      lcd.drawLine(155, 110, 155, 135, RGB(0,0,0));
      lcd.drawLine(155, 122, 180, 122, RGB(0,0,0));
      lcd.fillTriangle(160, 120, 175, 120, 167, 112, RGB(0,0,0));
      lcd.fillTriangle(160, 125, 175, 125, 167, 133, RGB(0,0,0));
      //tekent het tweede vak om je initialen in te vullen
      
      derde2 = (String)derde;
      lcd.fillRoundRect(190, 110, 50, 25, 5, RGB(255,255,255));
      lcd.drawRoundRect(190, 110, 50, 25, 5, RGB(0,0,0));
      lcd.drawRoundRect(189, 109, 52, 27, 5, RGB(0,0,0));
      lcd.drawText(195, 115, derde2, RGB(0,0,0), RGB(255,255,255), 2);
      lcd.drawLine(215, 110, 215, 135, RGB(0,0,0));
      lcd.drawLine(215, 122, 240, 122, RGB(0,0,0));
      lcd.fillTriangle(220, 120, 235, 120, 227, 112, RGB(0,0,0));
      lcd.fillTriangle(220, 125, 235, 125, 227, 133, RGB(0,0,0));
      charverandering = 0;
    }
    
  }
}

int main(){
  init();
  Serial.begin(9600);
  lcd.begin();
  lcd.touchRead();
  lcd.touchStartCal(); //calibrate touchpanel
  
  while(1){
    if(firstTime == 1){
      drawMenu();                //drawed het menu
      firstTime = 0;
    }
    
    if(buttonPressed == 1){
      gameIsLive = 1;
      game();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op play wordt gedrukt
    }
    if(buttonPressed == 2){
      drawScores();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op scores wordt gedrukt
    }
    if(buttonPressed == 3){
      //multiplayer
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op multiplayer wordt gedrukt
    }
    if(buttonPressed == 4){
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      //dit gebeurt er als er op back wordt gedrukt in scores
    }
    if(buttonPressed == 10){
      postGame = 1;
      buttonPressed = 0;
      gameStart = 0;
      inputScore();
      firstTime = 1;
      buttonPressed = 0;
      gameStart = 0;
      postGame = 0;
      //tijdelijk, input van je naam scherm
    }
    checkButtonPress();        //checked of er wordt gedrukt op een knop en kijkt waar dat is gebeurt
    
  }
  
 
  
  
  return 0;
}
