#include <SoftwareSerial.h>

#include <Adafruit_FONA.h>
#include <TimeLib.h>
#include <Time.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SharpMem.h>

#define SPEAKER 5

#define FONA_RST 2

// any pins can be used
#define SCK 13
#define MOSI 11
#define SS 10

#define BLACK 0
#define WHITE 1

int showColon = 1;

Adafruit_SharpMem display(SCK, MOSI, SS);

Adafruit_FONA fona(FONA_RST);
HardwareSerial *fonaSerial = &Serial1;
uint8_t fonaType;

char replybuffer[255];

void setup() {
  // put your setup code here, to run once:
  biosBeep();
  setupSharpMem();
  setupTime();
  setupFONA();
}

void loop() {
  // put your main code here, to run repeatedly:
  updateTime();
  delay(500);
}

time_t getTeensyTime() {
  return Teensy3Clock.get();
}

void updateTime()
{
  display.setCursor(4,1);
  display.setTextSize(3);
  display.setTextColor(WHITE, BLACK);
  char timeString[5];
  sprintf(timeString, "%2d%s%s%d", hour(), (showColon = !showColon) ? ":" : " ", minute() < 10 ? "0" : "", minute());
  display.print(timeString);
  display.refresh();
}

void setupTime()
{
  return setSyncProvider(getTeensyTime);
}

void setupSharpMem() {
  display.begin();
  display.fillScreen(BLACK);
  display.refresh();
  display.setTextSize(1);
  display.setCursor(4,4);
  display.setTextColor(WHITE, BLACK);
  display.println("Starting FONA");
  display.refresh();
}

void setupFONA() {
  fonaSerial->begin(115200);
  if (! fona.begin(*fonaSerial)){
    
  }
  fonaType = fona.type();
  display.setCursor(4, 40);
  switch (fonaType) {
    case FONA800L:
      display.println(F("FONA 800L")); break;
    case FONA800H:
      display.println(F("FONA 800H")); break;
    case FONA808_V1:
      display.println(F("FONA 808 (v1)")); break;
    case FONA808_V2:
      display.println(F("FONA 808 (v2)")); break;
    case FONA3G_A:
      display.println(F("FONA 3G (American)")); break;
    case FONA3G_E:
      display.println(F("FONA 3G (European)")); break;
    default: 
      display.println(F("???")); break;
  }
}

void ring () {
  for (int i=0; i<3; i++){
    tone(5, 440);
    delay(100);
    tone(5, 720);
    delay(100);
  }
  noTone(5);
}

void slide () {
  for (int i=100; i<1000; i++){
    tone(5, i);
    delay(5);
  }
  noTone(5);
}

void biosBeep () 
{
  pinMode(SPEAKER, OUTPUT);
  tone(SPEAKER, 600, 100);
}


