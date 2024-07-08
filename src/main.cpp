// SPDX-FileCopyrightText: 2022 Limor Fried for Adafruit Industries
//
// SPDX-License-Identifier: MIT

#include <Arduino.h>
#include "Adafruit_MAX1704X.h"
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ST7789.h> 
#include <Fonts/FreeSans12pt7b.h>

Adafruit_MAX17048 lipo;
Adafruit_ST7789 display = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

GFXcanvas16 canvas(240, 135);

void setup() {
  Serial.begin(115200);
  //while (! Serial) delay(10);
  delay(100);

  display.init(135, 240);           // Init ST7789 240x135
  display.setRotation(3);
  canvas.setFont(&FreeSans12pt7b);
  canvas.setTextColor(ST77XX_WHITE); 

  if (!lipo.begin()) {
    Serial.println(F("Couldnt find Adafruit MAX17048?\nMake sure a battery is plugged in!"));
    while (1) delay(10);
  }
    
  Serial.print(F("Found MAX17048"));
  Serial.print(F(" with Chip ID: 0x")); 
  Serial.println(lipo.getChipID(), HEX);

  pinMode(0, INPUT_PULLUP);
  pinMode(1, INPUT_PULLDOWN);
  pinMode(2, INPUT_PULLDOWN);

  // Configure Anolog input A0

}

int j = 0;

void loop() {
  // Serial.println("**********************");

  if (j % 2 == 0) {
    canvas.fillScreen(ST77XX_BLACK);
    canvas.setCursor(0, 17);
    canvas.setTextColor(ST77XX_RED);
    canvas.println("Adafruit Feather");
    canvas.setTextColor(ST77XX_GREEN); 
    canvas.print("Battery: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.print(lipo.cellVoltage(), 1);
    canvas.print(" V  /  ");
    canvas.print(lipo.cellPercent(), 0);
    canvas.println("%");
    canvas.setTextColor(ST77XX_BLUE); 
    canvas.print("I2C: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.print("Buttons: ");
    // Serial.println(digitalRead(0));
    // Serial.println(digitalRead(1));
    // Serial.println(digitalRead(2));
    if (!digitalRead(0)) {
      canvas.print("D0, ");
    }
    if (digitalRead(1)) {
      canvas.print("D1, ");
    }
    if (digitalRead(2)) {
      canvas.print("D2, ");
    }
    canvas.println();
    canvas.setTextColor(ST77XX_CYAN);
    canvas.print("Time: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println(millis() / 1000);

    // Print ADC value of A0 on canvas
    canvas.setTextColor(ST77XX_MAGENTA);
    canvas.print("A0: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println(analogRead(A0));

    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
  }
  
  delay(100);
  return;
}