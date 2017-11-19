void saveConfig() {
  for (unsigned int t = 0; t < sizeof(storage); t++)
    EEPROM.write(offsetEEPROM + t, *((char*)&storage + t));
}

void loadConfig() {
  if (EEPROM.read(offsetEEPROM + 0) == storage.chkDigit)
    for (unsigned int t = 0; t < sizeof(storage); t++)
      *((char*)&storage + t) = EEPROM.read(offsetEEPROM + t);
}

void printConfig() {
  if (EEPROM.read(offsetEEPROM + 0) == storage.chkDigit)
    for (unsigned int t = 0; t < sizeof(storage); t++)
      Serial.write(EEPROM.read(offsetEEPROM + t));

  Serial.println();
  setSettings(0);
}

void setSettings(bool doSet) {
  int i = 0;
  byte b;
  receivedString[0] = 'X';

  Serial.print(F("APRS Channel ("));
  Serial.print(storage.aprsChannel);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.aprsChannel = i;
  } 
  Serial.println();  

  Serial.print(F("RX Channel ("));
  Serial.print(storage.rxChannel);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.rxChannel = i;
  } 
  Serial.println(); 

   Serial.print(F("TX Channel ("));
  Serial.print(storage.txChannel);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.txChannel = i;
  } 
  Serial.println(); 

  Serial.print(F("RX Tone ("));
  Serial.print(storage.rxTone);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.rxTone = i;
  } 
  Serial.println(); 

  Serial.print(F("TX Tone ("));
  Serial.print(storage.txTone);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.txTone = i;
  } 
  Serial.println();        

  Serial.print(F("Dest. ("));
  Serial.print(storage.dest);
  Serial.print(F("):"));
  if (doSet == 1) {
    getStringValue(7);
    if (receivedString[0] != 0) {
      storage.dest[0] = 0;
      strcat(storage.dest, receivedString);
    }
  }
  Serial.println();

  Serial.print(F("Dest. SSID ("));
  Serial.print(storage.dest_ssid);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.dest_ssid = i;
  }
  Serial.println();

  Serial.print(F("Call ("));
  Serial.print(storage.call);
  Serial.print(F("):"));
  if (doSet == 1) {
    getStringValue(7);
    if (receivedString[0] != 0) {
      storage.call[0] = 0;
      strcat(storage.call, receivedString);
    }
  }
  Serial.println();

  Serial.print(F("SSID ("));
  Serial.print(storage.ssid);
  Serial.print(F("):"));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.ssid = i;
  }
  Serial.println();

  Serial.print(F("Comment ("));
  Serial.print(storage.comment);
  Serial.print(F("):"));
  if (doSet == 1) {
    getStringValue(15);
    if (receivedString[0] != 0) {
      storage.comment[0] = 0;
      strcat(storage.comment, receivedString);
    }
  }
  Serial.println();

  Serial.print(F("Symbol >Car,<Motor ("));
  Serial.print(char(storage.symbool));
  Serial.print(F("):"));
  if (doSet == 1) {
    b = getCharValue();
    if (receivedString[0] != 0) storage.symbool = b;
  }
  Serial.println();

  Serial.print(F("Path1 ("));
  Serial.print(storage.path1);
  Serial.print(F("):"));
  if (doSet == 1) {
    getStringValue(7);
    if (receivedString[0] != 0) {
      storage.path1[0] = 0;
      strcat(storage.path1, receivedString);
    }
  }
  Serial.println();

  Serial.print(F("Path1 SSID ("));
  Serial.print(storage.path1_ssid);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.path1_ssid = i;
  }
  Serial.println();

  Serial.print(F("Path2 ("));
  Serial.print(storage.path2);
  Serial.print(F("): "));
  if (doSet == 1) {
    getStringValue(7);
    if (receivedString[0] != 0) {
      storage.path2[0] = 0;
      strcat(storage.path2, receivedString);
    }
  }
  Serial.println();

  Serial.print(F("Path2 SSID ("));
  Serial.print(storage.path2_ssid);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.path2_ssid = i;
  }
  Serial.println();

  Serial.print(F("Interval ("));
  Serial.print(storage.interval);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.interval = i;
  }
  Serial.println();

  Serial.print(F("Multiplier ("));
  Serial.print(storage.multiplier);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.multiplier = i;
  }
  Serial.println();

  Serial.print(F("Power ("));
  Serial.print(storage.power);
  Serial.print(F("W): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.power = i;
  }
  Serial.println();

  Serial.print(F("Height ("));
  Serial.print(storage.height);
  Serial.print(F("M): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.height = i;
  }
  Serial.println();

  Serial.print(F("Gain ("));
  Serial.print(storage.gain);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.gain = i;
  }
  Serial.println();

  Serial.print(F("Directivity ("));
  Serial.print(storage.directivity);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.directivity = i;
  }
  Serial.println();

  Serial.print(F("PreAmble ("));
  Serial.print(storage.preAmble);
  Serial.print(F(" chars): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.preAmble = i;
  }
  Serial.println();

  Serial.print(F("Tail ("));
  Serial.print(storage.tail);
  Serial.print(F(" chars): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.tail = i;
  }
  Serial.println();

  Serial.print(F("TX enabled ("));
  Serial.print(storage.doTX);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.doTX = i;
  }
  Serial.println();

  Serial.print(F("TX Timeout ("));
  Serial.print(storage.txTimeOut);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.txTimeOut = i;
  }
  Serial.println();

  Serial.print(F("Debugmode ("));
  Serial.print(storage.isDebug);
  Serial.print(F("): "));
  if (doSet == 1) {
    i = getNumericValue();
    if (receivedString[0] != 0) storage.isDebug = i;
  }
  Serial.println();  

  if (doSet == 1) {
    saveConfig();
    loadConfig();
  }
}

void getStringValue(int length) {
  serialFlush();
  receivedString[0] = 0;
  int i = 0;
  while (receivedString[i] != 13 && i < length) {
    if (Serial.available() > 0) {
      receivedString[i] = Serial.read();
      if (receivedString[i] == 13 || receivedString[i] == 10) {
        i--;
      }
      else {
        Serial.write(receivedString[i]);
      }
      i++;
    }
  }
  receivedString[i] = 0;
  serialFlush();
}

byte getCharValue() {
  serialFlush();
  receivedString[0] = 0;
  int i = 0;
  while (receivedString[i] != 13 && i < 2) {
    if (Serial.available() > 0) {
      receivedString[i] = Serial.read();
      if (receivedString[i] == 13 || receivedString[i] == 10) {
        i--;
      }
      else {
        Serial.write(receivedString[i]);
      }
      i++;
    }
  }
  receivedString[i] = 0;
  serialFlush();
  return receivedString[i - 1];
}

byte getNumericValue() {
  serialFlush();
  byte myByte = 0;
  byte inChar = 0;
  bool isNegative = false;
  receivedString[0] = 0;

  int i = 0;
  while (inChar != 13) {
    if (Serial.available() > 0) {
      inChar = Serial.read();
      if (inChar > 47 && inChar < 58) {
        receivedString[i] = inChar;
        i++;
        Serial.write(inChar);
        myByte = (myByte * 10) + (inChar - 48);
      }
      if (inChar == 45) {
        Serial.write(inChar);
        isNegative = true;
      }
    }
  }
  receivedString[i] = 0;
  if (isNegative == true) myByte = myByte * -1;
  serialFlush();
  return myByte;
}

void serialFlush() {
  for (int i = 0; i < 10; i++)
  {
    while (Serial.available() > 0) {
      Serial.read();
    }
  }
}
