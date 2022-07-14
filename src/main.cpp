#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_INA260.h>
#include <EEPROM.h>


File dataFile;
const byte chipSelect = 4;
byte relayPin = 7;
byte dataSets = 3;
const byte dataPoints = 60;
byte readingIndex;

struct Readings {
  double millivolts;
  double watts;
};

#pragma region Filename Lookup Tables
const char filename0[] PROGMEM = "log0.txt";
const char filename1[] PROGMEM = "log1.txt";
const char filename2[] PROGMEM = "log2.txt";
const char filename3[] PROGMEM = "log3.txt";
const char filename4[] PROGMEM = "log4.txt";
const char filename5[] PROGMEM = "log5.txt";
const char filename6[] PROGMEM = "log6.txt";
const char filename7[] PROGMEM = "log7.txt";
const char filename8[] PROGMEM = "log8.txt";
const char filename9[] PROGMEM = "log9.txt";

const char filename10[] PROGMEM = "log10.txt";
const char filename11[] PROGMEM = "log11.txt";
const char filename12[] PROGMEM = "log12.txt";
const char filename13[] PROGMEM = "log13.txt";
const char filename14[] PROGMEM = "log14.txt";
const char filename15[] PROGMEM = "log15.txt";
const char filename16[] PROGMEM = "log16.txt";
const char filename17[] PROGMEM = "log17.txt";
const char filename18[] PROGMEM = "log18.txt";
const char filename19[] PROGMEM = "log19.txt";

const char filename20[] PROGMEM = "log20.txt";
const char filename21[] PROGMEM = "log21.txt";
const char filename22[] PROGMEM = "log22.txt";
const char filename23[] PROGMEM = "log23.txt";
const char filename24[] PROGMEM = "log24.txt";
const char filename25[] PROGMEM = "log25.txt";
const char filename26[] PROGMEM = "log26.txt";
const char filename27[] PROGMEM = "log27.txt";
const char filename28[] PROGMEM = "log28.txt";
const char filename29[] PROGMEM = "log29.txt";

const char filename30[] PROGMEM = "log30.txt";
const char filename31[] PROGMEM = "log31.txt";
const char filename32[] PROGMEM = "log32.txt";
const char filename33[] PROGMEM = "log33.txt";
const char filename34[] PROGMEM = "log34.txt";
const char filename35[] PROGMEM = "log35.txt";
const char filename36[] PROGMEM = "log36.txt";
const char filename37[] PROGMEM = "log37.txt";
const char filename38[] PROGMEM = "log38.txt";
const char filename39[] PROGMEM = "log39.txt";

const char filename40[] PROGMEM = "log40.txt";
const char filename41[] PROGMEM = "log41.txt";
const char filename42[] PROGMEM = "log42.txt";
const char filename43[] PROGMEM = "log43.txt";
const char filename44[] PROGMEM = "log44.txt";
const char filename45[] PROGMEM = "log45.txt";
const char filename46[] PROGMEM = "log46.txt";
const char filename47[] PROGMEM = "log47.txt";
const char filename48[] PROGMEM = "log48.txt";
const char filename49[] PROGMEM = "log49.txt";


const char* const filenames[] PROGMEM = {
  filename0, filename1, filename2, filename3, filename4, filename5, filename6, filename7, filename8, filename9,
  filename10, filename11, filename12, filename13, filename14, filename15, filename16, filename17, filename18, filename19,
  filename20, filename21, filename22, filename23, filename24, filename25, filename26, filename27, filename28, filename29,
  filename30, filename31, filename32, filename33, filename34, filename35, filename36, filename37, filename38, filename39,
  filename40, filename41, filename42, filename43, filename44, filename45, filename46, filename47, filename48, filename49
};
#pragma endregion Filename Lookup Tables

//String filename = "datalog";
char filename[16];

Readings readings[dataPoints];
Adafruit_INA260 ina260 = Adafruit_INA260();


void logReadings(File f)
{
  for(int i = 0; i < dataPoints; i++)
  {
    f.print(i);
    f.print(F(","));
      f.print(readings[i].watts);  
    f.print(F(","));
      f.println(readings[i].millivolts);
  }
}


void setup()
{
  Serial.begin(115200);
   while (!Serial) {
    ;
  }

  pinMode(relayPin, OUTPUT);

  Serial.print(F("Initializing SD card..."));
  pinMode(SS, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println(F("Card failed, or not present"));
    ;
  }
  Serial.println(F("card initialized."));

  if (!ina260.begin()) {
    Serial.println(F("Couldn't find INA260 chip"));
    while (1);
  }
  Serial.println(F("Found INA260 chip"));


  for (int i = 0; i < dataPoints; i++) 
  { 
    readings[i].millivolts = ina260.readPower()/ina260.readCurrent();
    readings[i].watts = ina260.readPower();
  }

  readingIndex = EEPROM.read(0);

  strcpy_P(filename, (char*)pgm_read_word(&(filenames[readingIndex])));

  dataFile = SD.open(filename, FILE_WRITE);  
  if (dataFile) { 
      for(int i = 0; i < dataPoints; i++)
      {
        dataFile.print(i);
        dataFile.print(F(","));
          dataFile.print(readings[i].watts);  
        dataFile.print(F(","));
          dataFile.println(readings[i].millivolts);
      }
      dataFile.close();

      EEPROM.write(0, readingIndex + 1);
  }
  else
  { 
    Serial.print(F("error opening "));
    Serial.println(filename);
  }

  digitalWrite(relayPin, HIGH);
}

void loop() {
  
}