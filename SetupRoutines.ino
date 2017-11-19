
void setup() {
  Serial.begin(9600);

  Wire.begin();
  lcd.begin();
  delay(1);
  lcd.setCursor(0,0);
  lcd.setDoubleSize(true); 
  lcd.setCharReverse(true);  
  lcd.print(F("   APRS    "));
  lcd.setDoubleSize(false); 
  lcd.setCharReverse(false);  
  lcd.setCursor(0,2);  
  lcd.print(F("  TRACKER v3.2NL  "));    //intro

  pinMode(bfPin, OUTPUT);
  pinMode(PTT, OUTPUT);
  digitalWrite(PTT, LOW);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  pinMode(MicPwr, OUTPUT);
  digitalWrite(MicPwr,LOW);
  pinMode(sqlPin, INPUT);

  Timer1.initialize(76);    //µs  fe=13200 hz so TE=76µs 13157.9 mesured

  if (EEPROM.read(offsetEEPROM) != storage.chkDigit || digitalRead(button) == 0){
    Serial.println(F("Writing defaults"));
    saveConfig();    
  }

  loadConfig();
  printConfig();

  Serial.println(F("Type GS to enter setup:"));
  delay(2000);  
  lcd.setCursor(0,2); lcd.print(F("Connect serial to"));
  lcd.setCursor(2,3); lcd.print(F("enter setup ...")); 
  delay(3000);
  if (Serial.available()) {
    Serial.println(F("Check for setup"));
    if (Serial.find(chkGS)) {
      Serial.println(F("Setup entered..."));
      setSettings(1);
      delay(2000);
    }
  }

  //if (detectMenu() == 1) EepromMenu();
  Serial.println("");
  pttOffValue = analogRead(0);
  Serial.print(F("PTT OffValue"));  Serial.println(pttOffValue);
  Beacon.begin(bfPin, 1200, 2200, 350);   //analog pin, led pin, freq1, freq2, shift freq
  Beacon.setCallsign(storage.call, storage.ssid);
  Beacon.setDestination(storage.dest, storage.dest_ssid);
  Beacon.setSymbol(storage.symbool);
  Beacon.setComment(storage.comment);
  Beacon.setPath1(storage.path1, storage.path1_ssid);
  Beacon.setPath2(storage.path2, storage.path2_ssid);
  Beacon.setPower(storage.power);
  Beacon.setHeight(storage.height);
  Beacon.setGain(storage.gain);
  Beacon.setDirectivity(storage.directivity);  
  Beacon.setPreamble(storage.preAmble);
  Beacon.setTail(storage.tail);
  
  Updatedelay = storage.interval * storage.multiplier;

  Beacon.printSettings();
  Serial.print(F("Free RAM:     ")); Serial.println(Beacon.freeMemory());

  gps_dra.begin(9600);
  setDra(storage.rxChannel, storage.txChannel, storage.rxTone, storage.txTone);
  gps_dra.println(F("AT+DMOSETVOLUME=8"));
  gps_dra.println(F("AT+DMOSETMIC=8,0"));
  
  Serial.println(F("Sats HDOP Latitude   Longitude   Fix  Date       Time     Date Alt    Course Speed Dir   Interval"));
  Serial.println(F("          (deg)      (deg)       Age                      Age  (m)    --- from GPS ---   (sec)   "));
  Serial.println(F("-------------------------------------------------------------------------------------------------"));
  
  lcd.clear();
  timeOutTimerMillis = (long)storage.txTimeOut*1000;
}
