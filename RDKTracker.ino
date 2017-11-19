#include <LiquidCrystal_SSD1306.h>

#include <RDKAPRS.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <TimerOne.h>
#include <Wire.h>
#include <EEPROM.h>
#include <LiquidCrystal_SSD1306.h>

#define button  2   //button to transmit package
#define PTT     3   // PTT pin. This is active low.
#define MicPwr  4   // PTT pin. This is active low.
#define ledPin  A2  //led for GPS fix
#define bfPin   5   //afsk output must be PD5 
#define sqlPin  6   //Squelsh pin

#define rxPin   8   //rx pin into TX GPS connection
#define txPin   9   //tx pin into RX GPS connection

#define offsetEEPROM 0x0    //offset config

TinyGPSPlus gps;
SoftwareSerial gps_dra(rxPin, txPin);  // RX from GPS (blue), TX to DRA (pin 8)
#define OLED_RESET 0
LiquidCrystal_SSD1306 lcd(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, OLED_RESET);

long lastUpdate = 0;
int Updatedelay = 600; //delay between packets in seconds.
bool validGPS = 1;
bool pttPressed = 0;
bool lastPttPressed = 0;
bool isInverted = 0;
int pttOffValue = 0;

long currentMillis;
long passedMillis = 0;
long startTimeOutMillis = 0;
long currentTimeOutMillis = 0;
long timeOutTimerMillis = 0;
bool buttonPressed = 0;
char receivedString[28];
char chkGS[3] = "GS";

struct StoreStruct {
  byte chkDigit;
  byte aprsChannel;
  byte rxChannel;
  byte txChannel;
  byte rxTone;
  byte txTone;
  char dest[8];
  byte dest_ssid;
  char call[8];
  byte ssid;
  char comment[16];
  byte symbool;  // = auto.
  char path1[8];
  byte path1_ssid;
  char path2[8];
  byte path2_ssid;
  byte interval;
  byte multiplier;
  byte power;
  byte height;
  byte gain;
  byte directivity;
  byte preAmble;
  byte tail;
  byte doTX;
  byte txTimeOut;
  byte isDebug;
};

StoreStruct storage = {
  '{',
  32,
  70,
  46,
  8,
  8,
  "APZRAZ",
  0,
  "PA2RDK",
  8,
  "73 de Robert",
  '>',
  "WIDE1",
  1,
  "WIDE2",
  2,
  30,
  20,
  5,
  0,
  0,
  0,
  100,
  50,
  0,
  10,
  0
};

void loop() {
  if (digitalRead(button) == 0) {
    buttonPressed = 1;
    passedMillis = 0;
  }
  currentMillis = millis();
  if (currentMillis - passedMillis > 0) {
    passedMillis = currentMillis;

    float flat, flon;
    unsigned long age;
    unsigned long gps_speed;
    unsigned int FlexibleDelay = Updatedelay;

    printInt(gps.satellites.value(), gps.satellites.isValid(), 5, 0);
    printInt(gps.hdop.value(), gps.hdop.isValid(), 5, 0);

    flat = gps.location.lat();
    flon = gps.location.lng();
    age = gps.location.age();
    gps_speed = gps.speed.kmph();

    if (storage.isDebug == 1) {
      printStr("5204.44N", 11);
      printStr("00430.22E", 12);
    } else {
      printFloat(flat, gps.location.isValid(), 11, 6, 0);
      printFloat(flon, gps.location.isValid(), 12, 6, 0);
    }

    printInt(age, gps.location.isValid(), 5, 0);
    printDateTime(gps.date, gps.time, 0);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2, 0);
    printFloat(gps.course.deg(), gps.course.isValid(), 7, 2, 0);
    printFloat(gps_speed, gps.speed.isValid(), 6, 2, 0);
    printStr(gps.course.isValid() ? TinyGPSPlus::cardinal(gps.course.value()) : "*** ", 6);

    if ((gps_speed > 0) && (gps_speed < 20000)) {
      FlexibleDelay = Updatedelay / gps_speed ;
      if (FlexibleDelay < storage.interval) FlexibleDelay = storage.interval;
    } else {
      FlexibleDelay = Updatedelay;
    }

    Serial.print(FlexibleDelay);

    if (((!gps.location.isValid()) || (age > 3000)) && (storage.isDebug == 0)) {
      Serial.print(F(" Invalid position"));
      validGPS = 0;
    } else {
      validGPS = 1;
      if (lastPttPressed == 0)
        if ((buttonPressed == 1) || ((millis() - lastUpdate) / 1000 > FlexibleDelay) || (Updatedelay == 0))
        {
          showDisplay(flat, flon, 1);
          Serial.println("");
          Serial.print(F("Send beacon:"));
          setDra(storage.aprsChannel, storage.aprsChannel, 0, 0);
          smartDelay(100);
          locationUpdate(flat, flon);
          smartDelay(50);
          setDra(storage.rxChannel, storage.txChannel, storage.rxTone, storage.txTone);
          lastUpdate = millis();
        }
    }
    buttonPressed = 0;
    showDisplay(flat, flon, 0);

    smartDelay(1000);
    if (lastPttPressed != pttPressed) {
      Serial.print(F(" PTT Swapped"));
      lastPttPressed = pttPressed;
      if (lastPttPressed == 1) {
        startTimeOutMillis = millis();
        digitalWrite(PTT, HIGH);                           //PTT on
        digitalWrite(MicPwr, HIGH);                        //Mike on
        digitalWrite(ledPin, HIGH);
      }
      else
      {
        digitalWrite(PTT, LOW);                            //PTT off
        digitalWrite(MicPwr, LOW);                         //Mike off
        digitalWrite(ledPin, LOW);
      }
    }
    currentTimeOutMillis = millis();
    if (((currentTimeOutMillis - startTimeOutMillis) > timeOutTimerMillis) && (storage.txTimeOut > 0) && (lastPttPressed == 1)) {
      isInverted = !isInverted;
      lcd.invert(isInverted);
      digitalWrite(PTT, LOW);                            //PTT off
      digitalWrite(MicPwr, LOW);                         //Mike off
      digitalWrite(ledPin, LOW);
    } else lcd.invert(false);

    Serial.print(F(" ")); Serial.print(analogRead(0));
    if (lastPttPressed == 1) Serial.println(F(" TX")); else Serial.println(F(" RX"));
  }
}

void showDisplay(float flat, float flon, bool isTX) {

  //Print GPS info to lcd

  if (lastPttPressed == 1) {
    lcd.setCursor(0, 0);
    lcd.setDoubleSize(true);
    lcd.setCharReverse(true);
    lcd.print(F("  "));
    lcd.print(storage.call);
    lcd.print(F("    "));
    lcd.setDoubleSize(false);
    lcd.setCharReverse(false);
    lcd.setCursor(0, 2);
    lcd.print(F("RX:"));
    if (storage.rxChannel < 40) lcd.print(F("144.")); else lcd.print(F("145."));
    byte tmp = storage.rxChannel;
    if (tmp > 39) tmp = tmp - 40;
    lcd.print(tmp * 25);
    lcd.print(F(" / ")); lcd.print(storage.rxTone); lcd.print(F("         "));

    lcd.setCursor(0, 3);
    lcd.print(F("TX:"));
    if (storage.txChannel < 40) lcd.print(F("144.")); else lcd.print(F("145."));
    tmp = storage.txChannel;
    if (tmp > 39) tmp = tmp - 40;
    lcd.print(tmp * 25);
    lcd.print(F(" / ")); lcd.print(storage.txTone); lcd.print(F("         "));
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.setCharReverse(1);
    lcd.print(F(" Tme:"));
    lcd.setCharReverse(0);
    printDateTime(gps.date, gps.time, 1);
    lcd.print(F("       "));
    lcd.setCursor(18, 0);
    if (isTX == 1) lcd.print(F("TX")); else lcd.print(F("--"));
    lcd.setCursor(0, 1);
    lcd.setCharReverse(1);
    lcd.print(F(" Lat:"));
    lcd.setCharReverse(0);
    if (validGPS) printFloat(flat, gps.location.isValid(), 11, 6, 1); else lcd.print(F("********"));
    lcd.print(F("       "));
    lcd.setCursor(0, 2);
    lcd.setCharReverse(1);
    lcd.print(F(" Lon:"));
    lcd.setCharReverse(0);
    if (validGPS) printFloat(flon, gps.location.isValid(), 12, 6, 1); else lcd.print(F("********"));
    lcd.print(F("       "));
    lcd.setCursor(14, 2);
    lcd.setCharReverse(1);
    lcd.print(F(" #:"));
    lcd.setCharReverse(0);
    lcd.setCursor(17, 2);
    if (validGPS) printInt(gps.satellites.value(), gps.satellites.isValid(), 5, 1); else lcd.print(F("**"));

    lcd.setCursor(0, 3);
    lcd.setCharReverse(1);
    lcd.print(F(" Alt:"));
    lcd.setCharReverse(0);
    lcd.setCursor(5, 3);
    if (validGPS) {
      lcd.setCursor(5, 3);
      printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2, 1);
    } else lcd.print(F("*****"));

    lcd.print(F("M "));
    lcd.setCursor(14, 3);
    lcd.setCharReverse(1);
    if (validGPS) lcd.print(F("GPSFix")); else lcd.print(F("No Fix"));
    lcd.setCharReverse(0);
  }
}








