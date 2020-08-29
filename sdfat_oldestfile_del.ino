#include <SPI.h>
#include "SdFat.h"

// SD default chip select pin.
const uint8_t chipSelect = 4;

// file system object
SdFat SD;

SdFile dirFile;
SdFile file;
SdFile oldestFile;

dir_t dir;
//------------------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  
  // Wait for USB Serial 
  while (!Serial) {
    SysCall::yield();
  }
  
  Serial.println("Type any character to start");
  while (!Serial.available()) {
    SysCall::yield();
  }

  // Initialize at the highest speed supported by the board that is
  // not over 50 MHz. Try a lower speed if SPI errors occur.
  if (!SD.begin(chipSelect, SD_SCK_MHZ(50))) {
    SD.initErrorHalt();
  }
  //OLD file remove if free space not enough
  //delay(5000);
  
  Serial.print(F("Card Size MB:   "));
  Serial.println(0.000512*SD.card()->cardSize());  
  float clusterSize = 0.000512*SD.vol()->blocksPerCluster();
  Serial.print(F("Total space MB: "));
  Serial.println(clusterSize*SD.vol()->clusterCount());
  Serial.print(F("Free Space MB:  "));
  Serial.println(clusterSize*SD.vol()->freeClusterCount());
  //  float F_space = clusterSize*SD.vol()->freeClusterCount();
  //  Serial.println(F_space);

  //This line for check free space.
  while (float F_space = clusterSize*SD.vol()->freeClusterCount() <= 700.00){
    
    Serial.println("less than 700M");

    uint32_t oldestModified = 0xFFFFFFFF;
    uint32_t lastModified;
    char oldfile[255];
    
      if (!dirFile.open("/")) {
        SD.errorHalt("open root failed");
      }

//      while (file.openNext(&root, O_WRITE)) {
      while (file.openNext(&dirFile, O_RDONLY)) {
      // Skip directories and hidden files.
      //file.printName(&Serial);
        if (!file.isSubDir() && !file.isHidden()) {
          //file.printName(&Serial);
          file.dirEntry(&dir);
          lastModified = (uint16_t (dir.lastWriteDate) << 16 | dir.lastWriteTime);
            if (lastModified < oldestModified ) {
              oldestModified = lastModified;
              oldestFile = file;
              file.getName(oldfile, 255);

            }
        }
        file.close();
      }

    oldestFile.printName(&Serial);
    Serial.println();

    //
    if (!file.open(oldfile, O_WRONLY)) Serial.println("open error");
    oldestFile = file;
    file.close();
 
    if (!oldestFile.remove()) Serial.println("Error file.remove");
    //oldestFile.remove();
    Serial.println("removed!");
    dirFile.close();

    
    //check again space    
    Serial.print(F("Card Size MB:   "));
    Serial.println(0.000512*SD.card()->cardSize());  
    float clusterSize = 0.000512*SD.vol()->blocksPerCluster();
    Serial.print(F("Total space MB: "));
    Serial.println(clusterSize*SD.vol()->clusterCount());
    Serial.print(F("Free Space MB:  "));
    Serial.println(clusterSize*SD.vol()->freeClusterCount());
    //    float F_space = clusterSize*SD.vol()->freeClusterCount();
    //    Serial.println(F_space);
   }
   Serial.println("Enough space!!");
}
//------------------------------------------------------------------------------
void loop() {


}
