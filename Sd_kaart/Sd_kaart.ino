#include <SPI.h>
#include <SD.h>

File myFile;

int main(){
  init();
  SD.begin(4);
  Serial.begin(9600);
  
  myFile = SD.open("test.txt", FILE_WRITE);
  myFile.println("testing 1, 2, 3.");
  myFile.close();
  
  myFile = SD.open("test.txt");
  
  while (myFile.available()) {
    Serial.write(myFile.read());

  }
  
  myFile.close();
  return 0;
}

