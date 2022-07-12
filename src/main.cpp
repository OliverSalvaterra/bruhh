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
  double watts;
};

#pragma region Filename Lookup Tables
const char filename0[] PROGMEM = "datalog0.txt";
const char filename1[] PROGMEM = "datalog1.txt";
const char filename2[] PROGMEM = "datalog2.txt";
const char filename3[] PROGMEM = "datalog3.txt";
const char filename4[] PROGMEM = "datalog4.txt";
const char filename5[] PROGMEM = "datalog5.txt";
const char filename6[] PROGMEM = "datalog6.txt";
const char filename7[] PROGMEM = "datalog7.txt";
const char filename8[] PROGMEM = "datalog8.txt";
const char filename9[] PROGMEM = "datalog9.txt";

const char filename10[] PROGMEM = "datalog10.txt";
const char filename11[] PROGMEM = "datalog11.txt";
const char filename12[] PROGMEM = "datalog12.txt";
const char filename13[] PROGMEM = "datalog13.txt";
const char filename14[] PROGMEM = "datalog14.txt";
const char filename15[] PROGMEM = "datalog15.txt";
const char filename16[] PROGMEM = "datalog16.txt";
const char filename17[] PROGMEM = "datalog17.txt";
const char filename18[] PROGMEM = "datalog18.txt";
const char filename19[] PROGMEM = "datalog19.txt";

const char filename20[] PROGMEM = "datalog20.txt";
const char filename21[] PROGMEM = "datalog21.txt";
const char filename22[] PROGMEM = "datalog22.txt";
const char filename23[] PROGMEM = "datalog23.txt";
const char filename24[] PROGMEM = "datalog24.txt";
const char filename25[] PROGMEM = "datalog25.txt";
const char filename26[] PROGMEM = "datalog26.txt";
const char filename27[] PROGMEM = "datalog27.txt";
const char filename28[] PROGMEM = "datalog28.txt";
const char filename29[] PROGMEM = "datalog29.txt";

const char filename30[] PROGMEM = "datalog30.txt";
const char filename31[] PROGMEM = "datalog31.txt";
const char filename32[] PROGMEM = "datalog32.txt";
const char filename33[] PROGMEM = "datalog33.txt";
const char filename34[] PROGMEM = "datalog34.txt";
const char filename35[] PROGMEM = "datalog35.txt";
const char filename36[] PROGMEM = "datalog36.txt";
const char filename37[] PROGMEM = "datalog37.txt";
const char filename38[] PROGMEM = "datalog38.txt";
const char filename39[] PROGMEM = "datalog39.txt";

const char filename40[] PROGMEM = "datalog40.txt";
const char filename41[] PROGMEM = "datalog41.txt";
const char filename42[] PROGMEM = "datalog42.txt";
const char filename43[] PROGMEM = "datalog43.txt";
const char filename44[] PROGMEM = "datalog44.txt";
const char filename45[] PROGMEM = "datalog45.txt";
const char filename46[] PROGMEM = "datalog46.txt";
const char filename47[] PROGMEM = "datalog47.txt";
const char filename48[] PROGMEM = "datalog48.txt";
const char filename49[] PROGMEM = "datalog49.txt";


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

File dataFile;
Readings readings[dataPoints];
Adafruit_INA260 ina260 = Adafruit_INA260();


void logReadings(File f)
{
  for(int i = 0; i < dataPoints; i++)
  {
    f.print(i);
    f.print(",");
      f.print(readings[i].watts);  
    f.print(",");
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

  // filename.concat(readingIndex);
  // filename.concat(".txt");

  strcpy_P(filename, (char*)pgm_read_word(&(filenames[readingIndex])));
  
  dataFile = SD.open(filename, FILE_WRITE);  
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