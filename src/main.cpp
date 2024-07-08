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

}

int j = 0;

void loop() {

  if (j % 2 == 0) {
    canvas.fillScreen(ST77XX_BLACK);
    canvas.setCursor(0, 17);
    canvas.setTextColor(ST77XX_BLUE);
    canvas.print("Planter ");
    canvas.setTextColor(ST77XX_GREEN);
    canvas.println("ONE");

    canvas.setTextColor(ST77XX_ORANGE); 
    canvas.print("Temperature: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println("25.5 C");  // TODO: Read from sensor

    canvas.setTextColor(ST77XX_ORANGE); 
    canvas.print("Humidity: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.println("50%");  // TODO: Read from sensor

    uint16_t analog_read = analogRead(A0);
    float soilMoisture = map(analog_read, 1000, 3000, 100., 0.);  // TODO: Fix this

    canvas.setTextColor(ST77XX_GREEN);
    canvas.print("Soil Moisture: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.print(soilMoisture);
    canvas.println("%");

    canvas.setTextColor(ST77XX_CYAN);
    canvas.print("Battery: ");
    canvas.setTextColor(ST77XX_WHITE);
    canvas.print(lipo.cellVoltage(), 1);
    canvas.print(" V  /  ");
    canvas.print(lipo.cellPercent(), 0);
    canvas.println("%");

    display.drawRGBBitmap(0, 0, canvas.getBuffer(), 240, 135);
    pinMode(TFT_BACKLITE, OUTPUT);
    digitalWrite(TFT_BACKLITE, HIGH);
  }
  
  delay(100);
  return;
}