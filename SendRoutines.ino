
void locationUpdate(float flat, float flon) {
  gps_dra.end();

  int temp;
  char y[13];
  char z[13];

  // CONVERT to NMEA.
  if (flat < 0) {
    temp = -(int)flat;
    flat = temp * 100 - (flat + temp) * 60;
    ftoa((char*)&y, flat, 3);
    if (flat > 10000) {
      y[8] = 'S';
      y[9] = 0;
    } else {
      y[7] = 'S';
      y[8] = 0;
    }
  } else {
    temp = (int)flat;
    flat = temp * 100 + (flat - temp) * 60;
    ftoa((char*)&y, flat, 3);
    if (flat > 10000) {
      y[8] = 'N';
      y[9] = 0;
    } else {
      y[7] = 'N';
      y[8] = 0;
    }
  }

  Beacon.setLat(y);

  if (flon < 0) {
    temp = -(int)flon;
    flon = temp * 100 - (flon + temp) * 60;
    ftoa((char*)&z, flon, 3);
    if (flon > 10000) {
      z[8] = 'W';
      z[9] = 0;
    } else {
      z[7] = 'W';
      z[8] = 0;
    }
  } else {
    temp = (int)flon;
    flon = temp * 100 + (flon - temp) * 60;
    ftoa((char*)&z, flon, 3);
    if (flon > 10000) {
      z[7] = 'E';
      z[8] = 0;
    } else {
      z[6] = 'E';
      z[7] = 0;
    }
    if (flon > 10000) {
      for (int i = 7; i > 0; i--) {
        z[i] = z[i - 1];
      }
    } else {
      for (int i = 8; i > 1; i--) {
        z[i] = z[i - 2];
      }
      z[1] = '0';
    }
    z[0] = '0';
    z[9] = 0;
  }

  Beacon.setLon(z);

  if (storage.isDebug == 1) {
    Beacon.setLat("5204.44N");
    Beacon.setLon("00430.22E");
  }


  // We can optionally set power/height/gain/directivity
  // information. These functions accept ranges
  // from 0 to 10, directivity 0 to 9.
  // See this site for a calculator:
  // http://www.aprsfl.net/phgr.php
  // LibAPRS will only add PHG info if all four variables
  // are defined!
  //Beacon.setPower(2);
  //Beacon.setHeight(4);
  //Beacon.setGain(7);
  //Beacon.setDirectivity(0);

  txing(0);
  gps_dra.begin(9600);
}

void txing(bool sendTrack)
{
  Serial.println("");
  byte save_TIMSK0;
  byte save_PCICR;
  if (storage.doTX != 0) {
    lcd.invert(true);
    digitalWrite(PTT, HIGH);         //ptt on
    digitalWrite(ledPin, HIGH);
  }
  delay(500);                                       //delay before sending data
  //TCCR0B = TCCR0B & 0b11111000 | 1;                 //switch to 62500 HZ PWM frequency
  //After warnings...
  TCCR0B = (TCCR0B & 0b11111000) | 1;                 //switch to 62500 HZ PWM frequency
  save_TIMSK0 = TIMSK0;                             //save Timer 0 register
  TIMSK0 = 0;                                       //disable Timer 0
  save_PCICR = PCICR;                               //save external pin interrupt register
  PCICR = 0;                                        //disable external pin interrupt
  Timer1.attachInterrupt(sinus_irq);                //warp interrupt in library

  Beacon.printSettings();

  if (!sendTrack) Beacon.sendMessage();             //else Beacon.sendPacket(track2, 72);

  digitalWrite(PTT, LOW);                           //PTT off
  digitalWrite(ledPin, LOW);
  lcd.invert(false);
  Timer1.detachInterrupt();                         //disable timer1 interrupt
  analogWrite(bfPin, 0);                            //PWM at 0
  //TCCR0B = TCCR0B & 0b11111000 | 3;                 //register return to normal
  //After warnings...
  TCCR0B = (TCCR0B & 0b11111000) | 3;                 //register return to normal
  TIMSK0 = save_TIMSK0;
  PCICR = save_PCICR;
  Beacon.ptrStartNmea = 0;
  Serial.println("");
}

void sinus_irq()    //warp timer1 irq into DRAPRS lib
{
  Beacon.sinus();
}
