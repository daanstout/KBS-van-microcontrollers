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
#include "Jump.h"
#include "Menu.h"
#include "Opmaak.h"
#include "Game.h"

volatile uint8_t teller = 0;



//aanmaken van de classen:
MI0283QT9 lcd;
Jump J;
Menu M;
Opmaak O;
Game G;


ISR(TIMER2_OVF_vect) {
    if(G.gameIsLive){
      teller++;
      if(teller > 3){
        O.sidescroll(lcd, &M, G.moeilijkheid);                                                                  //scrolled de game opzij                                                                                   
        if (!G.geland) {                                                                          //kijkt of de speler niet is geland, en zo niet, update hij de jump waardes
          J.updateJump();
        }
       teller = 0;
       G.veranderd = true; 
    }
  }
}

int main() {
  //initialiseren van de lcd scherm, we hardcoden de calibratie zodat we dat niet elke keer weer hoeven te doen
  init();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  lcd.begin();
  lcd.tp_matrix.a = 79800;
  lcd.tp_matrix.b = 4294966596;
  lcd.tp_matrix.c = 4250933096;
  lcd.tp_matrix.d = 2700;
  lcd.tp_matrix.e = 57800;
  lcd.tp_matrix.f = 4291263596;
  lcd.tp_matrix.div = 109865;


  TCCR2B |= (1 << CS20);
  TIMSK2 |= (1 << TOIE0);
  TCNT2 = 0;
  sei();

  Serial.begin(9600);

  //de infinite while loop
  while (1) {
    //elke keer als de loop voor de eerste keer het scherm tekent komt hij in deze if statement die het menu tekent.
    if (M.firstTime) {
      M.drawMenu(lcd);
      M.firstTime = false;
      M.scherm = 0;
    }
    //als de integer buttonPressed 1 is, gaat hij de game spelen. door verschillende variabelen en integers te zetten kan de game worden gestart.
    if (M.buttonPressed == 1) {
      M.multiplayerMode = false;    //zegt of multiplayer actief is, true = wel multiplayer, false = geen multiplayer
      G.gameIsLive = true;          //zegt dat de game live is, true = game moet runnen, false = game is false
      G.death = false;              //zegt of de speler af is, true = speler is dood, false = speler leeft nog
      M.setterScore(0);             //zet de score op 0
      G.moeilijkheid = 255;         //zet de moeilijkheid op 255
      G.currentY = 160;
      G.game(lcd, &M, &O, &J);       //start de game totdat de speler dood gaat
      M.firstTime = true;           //zet firstTime op true zodat het menu weer wordt getekend de volgende keer dat de loop loopt
      M.buttonPressed = 10;         //zet buttonPressed op 10 zodat je je nickname kan invullen
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.toCheckButton = false;      //zet de variabel toCheckButton op false zodat deze loop niet wordt gechecked of er op een knop is gedrukt
      M.scherm = 10;
    }
    //als de integer buttonPressed 2 is, gaat hij de highscore weergeven.
    if (M.buttonPressed == 2) {
      M.buttonPressed = 0;          //zet buttonPressed op 0 zodat de game weet dat hij op het menu moet gaan staan
      M.scherm = 1;
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.drawScores(lcd);            //tekent de highscores op het scherm
      M.firstTime = true;           //zet firstTime op true zodat het menu weer wordt getekend de volgende keer dat de loop loopt
      M.toCheckButton = false;      //zet de variabel toCheckButton op false zodat deze loop niet wordt gechecked of er op een knop is gedrukt
      M.scherm = 0;
      M.buttonPressed = 0;
      M.gameStart = false;
    }
    //als de integer buttonPressed 3 is, gaat hij de game 2 keer spelen voor multiplayer.
    if (M.buttonPressed == 3) {
      M.buttonPressed = 0;
      M.multiplayerMode = true;     //zet multiplayerMode op true zodat de game weet dat multiplayer actief is
      G.gameIsLive = true;          //zegt dat de game live is, true = game moet runnen, false = game is false
      G.death = false;              //zegt of de speler af is, true = speler is dood, false = speler leeft nog
      M.setterScore(0);             //zet de score op 0
      G.moeilijkheid = 255;         //zet de moeilijkheid op 255
      M.gameStart = false;
      O.tekenMultiplayerScherm(lcd, &M, 1);
      M.gameStart = false;
      G.game(lcd, &M, &O, &J);      //start de game totdat de speler dood gaat
      M.score2 = M.getterScore();   //nadat de eerste speler dood is gegaan wordt de score opgeslagen zodat de tweede speler kan spelen
      M.score = 0;            //zet de score op 0
      G.death = false;              //zegt of de speler af is, true = speler is dood, false = speler leeft nog
      //_delay_ms(5000);              //een delay zodat de spelers de controller kunnen doorgeven
      O.tekenMultiplayerScherm(lcd, &M, 2);
      M.gameStart = false;
      G.currentY = 160;
      O.aantalObstakels = 0;
      O.obstakelLocatie1 = 320;
      G.gameIsLive = true;          //zegt dat de game live is, true = game moet runnen, false = game is false
      G.game(lcd, &M, &O, &J);       //start de game totdat de speler dood gaat
      M.compare();                  //vergelijkt de 2 scores zodat de game weet wie de winnaar is
      M.firstTime = true;           //zet firstTime op true zodat het menu weer wordt getekend de volgende keer dat de loop loopt
      M.buttonPressed = 10;         //zet buttonPressed op 10 zodat je je nickname kan invullen
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.toCheckButton = false;      //zet de variabel toCheckButton op false zodat deze loop niet wordt gechecked of er op een knop is gedrukt
      M.scherm = 10;
    }
    //als de integer buttonPressed 5 is, gaat hij de How To weergeven.
    if (M.buttonPressed == 5) {
      M.firstTime = true;           //zet firstTime op true zodat het menu weer wordt getekend de volgende keer dat de loop loopt
      M.scherm = 3;
      M.buttonPressed = 0;          //zet buttonPressed op 0 zodat de game weet dat hij op het menu moet gaan staan
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.toCheckButton = false;      //zet de variabel toCheckButton op false zodat deze loop niet wordt gechecked of er op een knop is gedrukt
      M.howTo(lcd);                 //tekent de How To
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.howToContinue = false;      //zorgt ervoor dat de How To stopt
      M.scherm = 0;
      M.buttonPressed = 0;
      M.gameStart = false;
    }
    //als de integer buttonPressed 5 is, gaat hij de How To weergeven.
    if (M.buttonPressed == 10) {
      M.scherm = 10;
      M.buttonPressed = 0;          //zet buttonPressed op 0 zodat de game weet dat hij op het menu moet gaan staan
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.inputScore(lcd);            //laat het score input scherm zien
      M.firstTime = true;           //zet firstTime op true zodat het menu weer wordt getekend de volgende keer dat de loop loopt
      M.buttonPressed = 0;          //zet buttonPressed op 0 zodat de game weet dat hij op het menu moet gaan staan
      M.gameStart = false;          //zet gameStart op false zodat er bij de functie checkButtonPress weer kan worden gelooped
      M.toCheckButton = false;      //zet de variabel toCheckButton op false zodat deze loop niet wordt gechecked of er op een knop is gedrukt
      M.scoreSubmit = false;        //zet scoreSubmit op false zodat de game de volgende keer weet dat de score nog niet is gesubmit
      M.scherm = 0;
      M.multiplayerMode = false;
    }
    //kijkt of er moet worden gekeken of er op een knop is gedrukt
    if (M.toCheckButton) {
      M.checkButtonPress(lcd);
    }
    M.toCheckButton = true;         //zorgt ervor dat de loop de volgende keer checkt of er op een knop is gedrukt
  }

  return 0;                         //returned 0, komt hij toch nooit
}
