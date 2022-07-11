#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_INA260.h>
#include <EEPROM.h>


const int chipSelect = 4;
int relayPin = 7;
int dataSets = 3;
const int dataPoints = 60;
int maxIndex = -1;
int readingIndex;

struct Readings {
  double millivolts;
  //double amps;
  double watts;
};

String filename = "datalog";
File dataFile;
Readings readings[dataPoints];
Adafruit_INA260 ina260 = Adafruit_INA260();


void logReadings(File f)
{
  String d = "";
  for(int i = 0; i < dataPoints; i++)
  {
    d.concat(i);
    d.concat(",");
      d.concat(readings[i].watts);  
    d.concat(",");
      d.concat(readings[i].millivolts);

    f.println(d);
  }
}


void setup()
{
  Serial.begin(115200);
   while (!Serial) {
    ;
  }

  pinMode(relayPin, OUTPUT);

  Serial.print("Initializing SD card...");
  pinMode(SS, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1) ;
  }
  Serial.println("card initialized.");

  if (!ina260.begin()) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
  }
  Serial.println("Found INA260 chip");


  for (int i = 0; i < dataPoints; i++) 
  { 
    readings[i].millivolts = ina260.readPower()/ina260.readCurrent();
    readings[i].watts = ina260.readPower();
  }

  readingIndex = EEPROM.read(0);
  EEPROM.write(0, readingIndex + 1);

  filename.concat(readingIndex);
  filename.concat(".txt");

  
  dataFile = SD.open(filename.c_str(), FILE_WRITE);  
  if (dataFile) {
      String d = "";
      for(int i = 0; i < dataPoints; i++)
      {
        d.concat(i);
        d.concat(",");
          d.concat(readings[i].watts);  
        d.concat(",");
          d.concat(readings[i].millivolts);

        dataFile.println(d);
      }
      dataFile.close();
  }
  else
  { 
    Serial.print("error opening ");
    Serial.println(filename);
  }

  digitalWrite(relayPin, HIGH);
}

void loop() {
  
}