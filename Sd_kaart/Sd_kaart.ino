#include <EEPROM.h>

struct Score{
  uint16_t score;
  char naam[10];
  };


void saveScore(){
  Score nummer1 = {30, "Piet"};
  EEPROM.put(0, nummer1);
  Serial.println("STruct is succesvol geschreven");
}
void printScore(){
  Score nummer2 = {20, "DONO"};
  EEPROM.get(0, nummer2);
  Serial.println(nummer2.score);
  Serial.println(nummer2.naam);
}

int main(){
  init();
  Serial.begin(9600);
  saveScore();
  printScore();
  return 0;
}
