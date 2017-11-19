
static void smartDelay(unsigned long ms)
{
  int pttValue = 1000;
  unsigned long start = millis();
  do
  {
    while (gps_dra.available()) gps.encode(gps_dra.read());
    if (ms > 500) {
      pttValue = analogRead(0);
      pttPressed = (pttValue > pttOffValue + 250);
    }
  } while (millis() - start < ms && pttPressed == lastPttPressed);
}


static void printFloat(float val, bool valid, int len, int prec, bool toLCD)
{
  if (!valid)
  {
    while (len-- > 1) {
      if (toLCD) lcd.print(F("*")); else Serial.print(F("*"));
    }
    if (!toLCD) Serial.print(F(" "));
  }
  else
  {
    if (toLCD) lcd.print(val, prec); else Serial.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i = flen; i < len; ++i) {
      if (!toLCD) Serial.print(' ');
    }
  }
  smartDelay(0);
}

static void printInt(unsigned long val, bool valid, int len, bool toLCD)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i = strlen(sz); i < len; ++i)
    sz[i] = ' ';
  if (len > 0)
    sz[len - 1] = ' ';
  if (toLCD) lcd.print(sz); else Serial.print(sz);
  smartDelay(0);
}

static void printDateTime(TinyGPSDate &d, TinyGPSTime &t, bool toLCD)
{
  if (!toLCD) {
    if (!d.isValid())
    {
      Serial.print(F("********** "));
    }
    else
    {
      char sz[32];
      sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
      Serial.print(sz);
    }
  }

  if (!t.isValid())
  {
    if (toLCD) lcd.print(F("********")); else Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    if (toLCD) lcd.print(sz); else Serial.print(sz);
  }

  if (!toLCD) {
    printInt(d.age(), d.isValid(), 5, 0);
    smartDelay(0);
  }
}

static void printStr(const char *str, int len)
{
  int slen = strlen(str);
  for (int i = 0; i < len; ++i)
    Serial.print(i < slen ? str[i] : ' ');
  smartDelay(0);
}

// converts float to char*
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

void setDra(byte rxFreq, byte txFreq, byte rxTone, byte txTone) {
  byte tmp;
  gps_dra.print(F("AT+DMOSETGROUP=0,"));

  if (txFreq < 40) gps_dra.print(F("144.")); else gps_dra.print(F("145."));
  tmp = txFreq;
  if (tmp > 39) tmp = tmp - 40;
  gps_dra.print(tmp * 250);
  gps_dra.print(F(","));

  if (rxFreq < 40) gps_dra.print(F("144.")); else gps_dra.print(F("145."));
  tmp = rxFreq;
  if (tmp > 39) tmp = tmp - 40;
  gps_dra.print(tmp * 250);
  gps_dra.print(F(","));

  if (txTone < 10) gps_dra.print(F("000")); else gps_dra.print(F("00"));
  gps_dra.print(txTone);
  gps_dra.print(F(",1,"));

  if (rxTone < 10) gps_dra.print(F("000")); else gps_dra.print(F("00"));
  gps_dra.println(rxTone);

  Serial.print(F("AT+DMOSETGROUP=0,"));

  if (txFreq < 40) Serial.print(F("144.")); else Serial.print(F("145."));
  tmp = txFreq;
  if (tmp > 39) tmp = tmp - 40;
  Serial.print(tmp * 250);
  Serial.print(F(","));

  if (rxFreq < 40) Serial.print(F("144.")); else Serial.print(F("145."));
  tmp = rxFreq;
  if (tmp > 39) tmp = tmp - 40;
  Serial.print(tmp * 250);
  Serial.print(F(","));

  if (txTone < 10) Serial.print(F("000")); else Serial.print(F("00"));
  Serial.print(txTone);
  Serial.print(F(",1,"));

  if (rxTone < 10) Serial.print(F("000")); else Serial.print(F("00"));
  Serial.println(rxTone);

}

