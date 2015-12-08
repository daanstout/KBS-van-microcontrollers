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
//getallen:
uint16_t obstakelLocatie1, jumpLoopCount, i, last_x, x, topscore, obstakelBovenkant = 128, spelerRechterZijde = 52, current = 139;
uint8_t up = 70, score;


//namen:
char eerste = 'A', tweede = 'B', derde = 'C';
String eerste2, tweede2, derde2;

//booleans:
uint8_t firstTime = 1, top5 = 1, directie = 1, scoreSubmit = 1, eersteKeerScore = 1, toCheckButton = 1;
uint8_t zbutton, in_air, obstakelActief1, toJump, gameStart, buttonPressed, gameIsLive, death, postGame,charverandering, scoresBack;



void tekenLijn() {
  lcd.fillRect(0, 160, 320, 32, RGB(0, 100, 0));// tekent de grond waarop je rent
}

void obstakel(int x) {
  lcd.fillRect(x, 160 - (32), 1, 32, RGB(255, 0, 0)); //eerste rij genereren
}

void resetObstakel(int x) {
  lcd.fillRect(x + 32, 160 - (32), 1, 32, RGB(255, 255, 255)); //laatste rij van obstakel resetten
}


void sidescroll() {
  if (obstakelActief1 == 0) {
    obstakelActief1 = 1;
    obstakelLocatie1 = 320;
  }
  //genereert een obstakel
  if (obstakelActief1 == 1) {     //als een obstakel actief is, scrol de game
    
    //obstakel(obstakelLocatie1);

    last_x = obstakelLocatie1;

    if (toJump == 0) {
      checkJump();
    }
    //als er moet worden gesprongen, srpint
    //_delay_ms(3);
    if (obstakelLocatie1 == -32) {
      score++;
      obstakelLocatie1 = 320;
      obstakelActief1 = 0;
    }
    //als de obstakel uit het scherm is, haal hem weg en geef de speler 1 punt
    obstakelLocatie1--;
  }
  //resetObstakel(last_x);
}

void checkJump(){
  while (zbutton == 1) {
    if (zbutton == 1 && in_air == 0) {
      Serial.print("test speler");
      toJump = 1;
      zbutton = 0;
    }
  }
}
//kijkt of er moet worden gesprongen
void speler() {
  lcd.fillRect(32, 140, 20, 20, RGB(0, 0, 255)); //vierkant
}
// tekent de speler

void jump(){
  in_air = 1;
  if(directie == 1){      //kijkt of de speler omhoof moet
    lcd.fillRect(32, (current + 1), 20, 20, RGB(255,255,255)); //verwijder vorige ball
    lcd.fillRect(32, current, 20, 20, RGB(0,0,255));  //ball omhoog
    if(jumpLoopCount == 0){
      i = 0;
    }//als het de eerste keer is dat je omhoog springt loopt, zet i op 0 zodat we kunnen bijhouden of we helemaal boven zijn
    _delay_ms(1);
    current--;  //speler tekent bolletje steeds opnieuw vind oplossing
    i++;
    jumpLoopCount++;
    if(i == up){
      directie = 0;
      jumpLoopCount = 0;
    }//als de speler helemaal bovenaan is, zet de directie op 0 en zeg dat hij voor de eerste keer omlaag wordt getekent
  }else if(directie == 0){
    if(jumpLoopCount == 0){
      i = 0;
    }
    lcd.fillRect(32, (current - 1),20,20 , RGB(255,255,255)); //verwijder vorige ball
    lcd.fillRect(32, current, 20, 20, RGB(0,0,255));  //ball omlaag
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
  in_air = 0;
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
  while(scoresBack == 0){
    checkButtonPress();
  }
  scoresBack = 0;
}

void tekenVak1(){
  eerste2 = (String)eerste;
  lcd.fillRoundRect(90, 115, 30, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(90, 115, 30, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(89, 114, 32, 27, 5, RGB(0,0,0));
  lcd.drawText(98, 120, eerste2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.fillTriangle(97, 110, 112, 110, 104, 102, RGB(0,0,0));
  lcd.fillTriangle(97, 145, 112, 145, 104, 153, RGB(0,0,0));
  //tekent het eerste vak om je initialen in te vullen
}

void tekenVak2(){
  tweede2 = (String)tweede;
  lcd.fillRoundRect(140, 115, 30, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(140, 115, 30, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(139, 114, 32, 27, 5, RGB(0,0,0));
  lcd.drawText(148, 120, tweede2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.fillTriangle(147, 110, 162, 110, 154, 102, RGB(0,0,0));
  lcd.fillTriangle(147, 145, 162, 145, 154, 153, RGB(0,0,0));
  //tekent het tweede vak om je initialen in te vullen
}

void tekenVak3(){
  derde2 = (String)derde;
  lcd.fillRoundRect(190, 115, 30, 25, 5, RGB(255,255,255));
  lcd.drawRoundRect(190, 115, 30, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(189, 114, 32, 27, 5, RGB(0,0,0));
  lcd.drawText(198, 120, derde2, RGB(0,0,0), RGB(255,255,255), 2);
  lcd.fillTriangle(197, 110, 212, 110, 204, 102, RGB(0,0,0));
  lcd.fillTriangle(197, 145, 212, 145, 204, 153, RGB(0,0,0));
  //tekent het derde vak om je initialen in te vullen
}

void inputScore(){
  lcd.fillScreen(RGB(111,111,111));
  lcd.fillRect(0, 160,320,32,RGB(0,50,0));
  //achtergrond van het scherm
  
  lcd.drawText(82, 20, "GAME OVER", RGB(0,0,0), RGB(111,111,111), 2);
  lcd.drawText(80, 50, "SCORE:", RGB(0,0,0), RGB(111,111,111), 2);
  lcd.drawInteger(200, 50, score, DEC, RGB(0,0,0), RGB(111,111,111), 2);
  //schrijft de tekst op het game over scherm als je dood gaat
  
  if(topscore == 1){
    lcd.drawText(80, 80, "HIGHSCORE!", RGB(0,0,0), RGB(111,111,111), 2);
  }else if(top5 == 1){
    lcd.drawText(110, 80, "TOP 5!", RGB(0,0,0), RGB(111,111,111), 2);
  }
  //meldt of je de highscore hebt of dat je in de top 5 bent gekomen
  
  lcd.fillRoundRect(22, 164, 170, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(22, 164, 170, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(21, 163, 172, 27, 5, RGB(0,0,0));
  lcd.drawText(27, 170, "SAVE SCORE", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de save score knop
  
  lcd.fillRoundRect(210, 164, 80, 25, 5, RGB(0,034,255));
  lcd.drawRoundRect(210, 164, 80, 25, 5, RGB(0,0,0));
  lcd.drawRoundRect(209, 163, 82, 27, 5, RGB(0,0,0));
  lcd.drawText(220, 170, "QUIT", RGB(0,0,0), RGB(0,034,255), 2);
  //tekent de quit knop
  
  tekenVak1();
  tekenVak2();
  tekenVak3();
  
  scoreSubmit = 1;
  while(scoreSubmit){ //loopt zolang je nog bezig bent om je score in te vullen
    checkButtonPress();
    if(charverandering == 1){ //kijkt of er een character is verandert, en zo ja, hertekent de characters
      gameStart = 0;
      
      tekenVak1();
      tekenVak2();
      tekenVak3();
      
      charverandering = 0;
    }
    _delay_ms(100);
  }
  Serial.println("check1");
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
          scoresBack = 1;
        }
        if(lcd.touchX() > 280){
          buttonPressed = 10;
        }
      }
      if(postGame == 1){
        if(lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 70 && lcd.touchY() < 122){// kijkt of eerste character omhoog moet
          if(eerste == 'Z'){
            eerste = 'A';
          }else{
            eerste++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 70 && lcd.touchX() < 120 && lcd.touchY() > 122 && lcd.touchY() < 175){// kijkt of eerste character omlaag moet
          if(eerste == 'A'){
            eerste = 'Z';
          }else{
            eerste--;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 70 && lcd.touchY() < 122){// kijkt of tweede character omhoog moet
          if(tweede == 'Z'){
            tweede = 'A';
          }else{
            tweede++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 130 && lcd.touchX() < 180 && lcd.touchY() > 122 && lcd.touchY() < 175){// kijkt of tweede character omlaag moet
          if(tweede == 'A'){
            tweede = 'Z';
          }else{
            tweede--;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 70 && lcd.touchY() < 122){// kijkt of derde character omhoog moet
          if(derde == 'Z'){
            derde = 'A';
          }else{
            derde++;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 190 && lcd.touchX() < 240 && lcd.touchY() > 122 && lcd.touchY() < 175){//kijkt of derde character omlaag moet
          if(derde == 'A'){
            derde = 'Z';
          }else{
            derde--;
          }
          charverandering = 1;
        }else if(lcd.touchX() > 22 && lcd.touchX() < 192 && lcd.touchY() > 164 && lcd.touchY() < 189){//kijkt of de score moet worden opgeslagen
          scoreSubmit = 0;
          charverandering = 1;
        }else if(lcd.touchX() > 210 && lcd.touchX() < 290 && lcd.touchY() > 164 && lcd.touchY() < 189){//kijkt of de score niet moet worden opgeslagen
          scoreSubmit = 0;
          charverandering = 1;
        }
      }
    }
    if(buttonPressed != 0 || charverandering == 1){    //kijkt of er succesvol op een knop is gedrukt en zoja, doorbreekt
      gameStart = 1;
    }
  }
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

void teken(){
  if(obstakelLocatie1 != last_x){
    obstakel(obstakelLocatie1);
    resetObstakel(last_x);
  }
 
  
}


void game() {
  lcd.fillScreen(RGB(255,255,255)); // scherm leeg
  tekenLijn();
  nunchuck_setpowerpins();
  nunchuck_init();
  speler();
  
  while(gameIsLive == 1){
    nunchuck_get_data();
    zbutton = nunchuck_zbutton();
    
    sidescroll();
    teken();
    if(toJump == 1){
      jump();
    }
    hitbox();
    if(death == 1){
      gameIsLive = 0;
    }
    
  }
  current = 139;
  obstakelLocatie1 = 0;
  obstakelActief1 = 0;
  toJump = 0;
  in_air = 0;
  directie = 1;
  jumpLoopCount = 0;
  //alles resetten
}
void hitbox(){
  if(spelerRechterZijde > obstakelLocatie1 && current+20 > obstakelBovenkant && 32 < (obstakelLocatie1 + 32)){// kijkt of de speler het obstakel aanraakt
    death++;
  }
}

int main() {
  init();
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
      death--;
      score = 0;
      game();
      firstTime = 1;
      buttonPressed = 10;
      gameStart = 0;
      toCheckButton = 0;
      //dit gebeurt er als er op play wordt gedrukt
    }
    if(buttonPressed == 2){
      buttonPressed = 0;
      gameStart = 0;
      drawScores();
      firstTime = 1;
      toCheckButton = 0;
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
      toCheckButton = 0;
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
      toCheckButton = 0;
      //tijdelijk, input van je naam scherm
    }
    
    if(toCheckButton == 1){
      checkButtonPress();        //checked of er wordt gedrukt op een knop en kijkt waar dat is gebeurt
    }
    
    toCheckButton = 1;
  }

  return 0;
}

