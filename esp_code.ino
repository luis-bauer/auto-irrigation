#include <Arduino.h>
#include <U8g2lib.h>
#include "DHT.h"
#include <Adafruit_ADS1X15.h>
#define DHTPIN 2
#define DHTTYPE DHT11

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

int grenzwert = 50;
const int trocken1 = 635;
const int nass1 = 245;
const int trocken2 = 635;
const int nass2 = 245;
const int trocken3 = 635;
const int nass3 = 245;
int pumpePin = D8;
int schalterPin = D6;
int durchschnittFeucht;
int potenz = 0;
int pumpdauer = 30000;
float t;
float h;
boolean cooldown = false;
int ctimer = 0;
int ptimer = 0;
boolean tabcooldown = false;
int tabtimer = 0;
int tab = 1;
int druck = 0;
boolean bearbeiten = false;
int bearbeitenmode;
boolean füllen = false;
int letztebewaesserung = 0;
int letztebewaesserungtimer = 0;
char feuchtigkeit[33];
char sekundenprint[33];
char minutenprint[33];
char stundenprint[33];

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/1, /* data=*/2, /* reset=*/U8X8_PIN_NONE);  // High speed I2C
DHT dht(DHTPIN, DHTTYPE);
Adafruit_ADS1115 ads;

void Giessen(String befehl) {
  if (cooldown == false) {
    if (befehl == "start") {
      ptimer = millis();
      digitalWrite(pumpePin, HIGH);
      letztebewaesserungtimer = millis();
    }
  } else {
    if (millis() >= pumpdauer * 1000 + ptimer) {  //Zeitdauer für die die Pumpe angeschaltet wird
      digitalWrite(pumpePin, LOW);
    }
  }
}

void LetzteBewaesserung() {
  letztebewaesserung = (millis() - letztebewaesserungtimer) / 1000;
}

void Cooldown() {
  if (millis() >= 300000 + ctimer && cooldown == true) {
    cooldown = false;
  }
}

void TabCooldown() {
  if (millis() >= 400 + tabtimer && tabcooldown == true) {
    tabcooldown = false;
  }
}

void FeuchtigkeitErmitteln() {
  int Humidity1 = map(ads.readADC_SingleEnded(0), nass1, trocken1, 100, 0);
  int Humidity2 = map(ads.readADC_SingleEnded(1), nass2, trocken2, 100, 0);
  int Humidity3 = map(ads.readADC_SingleEnded(2), nass3, trocken3, 100, 0);
  durchschnittFeucht = (Humidity1 + Humidity2 + Humidity3) / 3;
}

void TabImage() {
  u8g2.drawFilledEllipse(14, 57, 2, 2);
  u8g2.drawFilledEllipse(39, 57, 2, 2);
  u8g2.drawFilledEllipse(64, 57, 2, 2);
  u8g2.drawFilledEllipse(89, 57, 2, 2);
  u8g2.drawFilledEllipse(114, 57, 2, 2);
}

void TabUpdate() {
  if (tab == 1) {
    itoa(durchschnittFeucht, feuchtigkeit, 10);
    u8g2.clearBuffer();                     // clear the internal memory
    u8g2.setFont(u8g2_font_lubR10_tr);
    u8g2.drawStr(10, 10, "Feuchtigkeit:");  // write something to the internal memory
    u8g2.setFont(u8g2_font_courB18_tf);
    u8g2.drawStr(80, 40, "%");           // write something to the internal memory
    u8g2.drawStr(48, 40, feuchtigkeit);  // write something to the internal memory
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.drawGlyph(100, 25, 219);
    u8g2.setFont(u8g2_font_8x13_t_symbols);
    u8g2.drawCircle(14, 57, 5);
    TabImage();
    u8g2.sendBuffer();
  }
  if (tab == 2) {
    u8g2.clearBuffer();  // clear the internal memory
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.drawGlyph(100, 25, 700);
    u8g2.setFont(u8g2_font_8x13_t_symbols);  // choose a suitable font
    u8g2.drawStr(48, 23, "c");
    u8g2.setCursor(0, 10);
    u8g2.print("Temperatur");
    u8g2.setFont(u8g2_font_9x15B_mf);  // choose a suitable font
    u8g2.setCursor(0, 23);
    u8g2.print(dht.readTemperature());

    //display print humidity
    u8g2.setFont(u8g2_font_8x13_t_symbols);  // choose a suitable font
    u8g2.drawStr(48, 48, "%\t");             // Tabulator
    u8g2.setCursor(0, 36);
    u8g2.print("Luftfeuchtigkeit");
    u8g2.setFont(u8g2_font_9x15B_mf);  // choose a suitable font
    u8g2.setCursor(0, 48);
    u8g2.print(dht.readHumidity());
    u8g2.setFont(u8g2_font_8x13_t_symbols);
    u8g2.drawCircle(39, 57, 5);
    TabImage();

    u8g2.sendBuffer();
    u8g2.setCursor(0, 0);
  }

  if (tab == 4) {
    int stunden = 0;
    int minuten = 0;
    int sekunden = 0;
    if (letztebewaesserung > 59) {
      for (int i = letztebewaesserung; i > 59; i = i - 60) {
        minuten++;
      }
    }
    sekunden = letztebewaesserung - (minuten * 60);
    if (minuten > 59) {
      for (int i = minuten; i > 59; i = i - 60) {
        stunden++;
      }
    }
    itoa(stunden, stundenprint, 10);
    itoa(minuten, minutenprint, 10);
    itoa(sekunden, sekundenprint, 10);
    u8g2.clearBuffer();                        // clear the internal memory
    u8g2.setFont(u8g2_font_lubR10_tr);
    u8g2.drawStr(0, 10, "Zuletzt Gegossen:");  // write something to the internal memory
    u8g2.setFont(u8g2_font_streamline_all_t);
    u8g2.drawGlyph(100, 35, 331);
    u8g2.setFont(u8g2_font_courB18_tf);
    if (stunden > 0) {
      u8g2.drawStr(5, 40, stundenprint);  // write something to the internal memory
      u8g2.drawStr(35, 40, "h");           // write something to the internal memory
      u8g2.drawStr(55, 40, minutenprint);  // write something to the internal memory
      u8g2.drawStr(85, 40, "m");           // write something to the internal memory
    } else {
      u8g2.drawStr(5, 40, minutenprint);   // write something to the internal memory
      u8g2.drawStr(35, 40, "m");            // write something to the internal memory
      u8g2.drawStr(55, 40, sekundenprint);  // write something to the internal memory
      u8g2.drawStr(85, 40, "s");            // write something to the internal memory
    }
    u8g2.setFont(u8g2_font_8x13_t_symbols);
    u8g2.drawCircle(89, 57, 5);
    TabImage();
    u8g2.sendBuffer();
  }

  if (tab == 5) {
    if (!füllen) {
      u8g2.clearBuffer();  // clear the internal memory
      u8g2.setFont(u8g2_font_lubR10_tr);
      u8g2.drawStr(5, 10, "Einstellungen");
      u8g2.setFont(u8g2_font_streamline_all_t);
      u8g2.drawGlyph(100, 25, 508);
      u8g2.setFont(u8g2_font_8x13_t_symbols);

      if (bearbeiten) {
        if (bearbeitenmode == 1) {
          u8g2.setCursor(20, 47);
          int potenzwert = map(ads.readADC_SingleEnded(3), 1024, 10, 240, 1);
          u8g2.setFont(u8g2_font_t0_22_te);
          u8g2.print(potenzwert);
          pumpdauer = potenzwert;
          u8g2.setFont(u8g2_font_8x13_t_symbols);
          u8g2.drawStr(0, 27, "Giesszeit");
        }
        if (bearbeitenmode == 2) {
          u8g2.setCursor(20, 47);
          int potenzwert = map(ads.readADC_SingleEnded(3), 1024, 10, 100, 1);
          u8g2.setFont(u8g2_font_t0_22_te);
          u8g2.print(potenzwert);
          grenzwert = potenzwert;
          u8g2.setFont(u8g2_font_8x13_t_symbols);
          u8g2.drawStr(0, 27, "Giessgrenze");
        }
      } else {
        u8g2.drawStr(17, 27, "Bearbeiten");
        u8g2.drawFrame(19, 32, 72, 12);
      }
      u8g2.setFont(u8g2_font_8x13_t_symbols);
      u8g2.drawCircle(114, 57, 5);
      TabImage();
      u8g2.sendBuffer();
    }
  }
}

void TabChange(int currenttab, boolean gedrueckt) {
  if (tabcooldown == false) {
    if (gedrueckt == true) {
      if (currenttab == 5) {
        
        if (!bearbeiten) {
          füllen = true;
          druck = druck + 2;
          u8g2.drawBox(20, 33, druck, 10);
          u8g2.sendBuffer();

          if (druck > 70) {
            druck = 68;
          }
        } else {
          if (bearbeitenmode == 1) {
            bearbeitenmode = 2;
            tabtimer = millis();
            tabcooldown = true;
          } else {
            bearbeiten = false;
            bearbeitenmode = 1;
            tab = 1;
            TabUpdate();
            tabtimer = millis();
            tabcooldown = true;
          }
        }
      }
      if (currenttab == 1) {
        tab = 2;
        TabUpdate();
        tabtimer = millis();
        tabcooldown = true;
      }
      if (currenttab == 2) {
        tab = 3;
        u8g2.clearBuffer();                       // clear the internal memory
        u8g2.setFont(u8g2_font_lubR10_tr);
        u8g2.drawStr(0, 10, "Pumpe Anschalten");  // write something to the internal memory
        u8g2.setFont(u8g2_font_streamline_all_t);
        u8g2.drawGlyph(100, 35, 196);
        u8g2.setFont(u8g2_font_8x13_t_symbols);
        u8g2.drawCircle(64, 30, 10);
        u8g2.drawCircle(64, 57, 5);
        TabImage();
        u8g2.sendBuffer();
        tabtimer = millis();
        tabcooldown = true;
      }
      if (currenttab == 3) {
        druck = druck + 1;
        if (druck == 1) {
          u8g2.drawCircle(64, 30, 13, U8G2_DRAW_UPPER_RIGHT);
          u8g2.sendBuffer();
        }
        if (druck == 1250) {
          u8g2.drawCircle(64, 30, 13, U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_UPPER_RIGHT);
          u8g2.sendBuffer();
        }
        if (druck == 2500) {
          u8g2.drawCircle(64, 30, 13, U8G2_DRAW_LOWER_LEFT | U8G2_DRAW_LOWER_RIGHT | U8G2_DRAW_UPPER_RIGHT);
          u8g2.sendBuffer();
        }
        if (druck == 3750) {
          u8g2.drawCircle(64, 30, 13, U8G2_DRAW_ALL);
          u8g2.sendBuffer();
        }
        if (druck > 5000) {
          cooldown = false;
          Giessen("start");
          ctimer = millis();
          cooldown = true;
        }
      }
      if (currenttab == 4) {
        tab = 5;
        tabtimer = millis();
        tabcooldown = true;
      }
    } else {
      if (currenttab == 3) {
        if (druck > 0) {
          tab = 4;
          tabtimer = millis();
          tabcooldown = true;
        }
        druck = 0;
      }
      if (currenttab == 5) {
        if (druck > 67) {
          bearbeiten = true;
          bearbeitenmode = 1;
        } else {
          if (druck > 0) {
            tab = 1;
            TabUpdate();
            tabtimer = millis();
            tabcooldown = true;
          }
        }
        füllen = false;
        druck = 0;
      }
    }
  }
}


void setup() {
  pinMode(pumpePin, OUTPUT);
  pinMode(schalterPin, INPUT);
  pinMode(A0, INPUT);
  dht.begin();
  u8g2.begin();
  u8g2.setFont(u8g2_font_8x13_t_symbols);
}

void loop() {
  potenz = analogRead(A0);
  h = dht.readHumidity();
  t = dht.readTemperature();


  LetzteBewaesserung();
  FeuchtigkeitErmitteln();

  if (durchschnittFeucht < grenzwert && cooldown == false) {
    Giessen("start");
    ctimer = millis();
    cooldown = true;
  }

  Cooldown();
  Giessen("");
  TabCooldown();

  if (digitalRead(schalterPin) == 1) {
    TabChange(tab, true);
  } else {
    TabChange(tab, false);
  }

  TabUpdate();
}