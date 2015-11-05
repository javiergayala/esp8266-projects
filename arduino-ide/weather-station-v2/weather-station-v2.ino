/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See more at http://blog.squix.ch
*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <Adafruit_GFX.h>
#include <ESP_SSD1306.h>

#include <SPI.h>
#include <Wire.h>



#include <Ticker.h>
#include "icons.h"


#include "WeatherClient.h"

#define MOSI 7
#define CLK 5
#define CS 15 // 8
#define DC 2 // 2
#define RST 16 // 15

// Initialize the oled display for address 0x3c
// sda-pin=14 and sdc-pin=12
ESP_SSD1306 display(DC, RST, CS);
WeatherClient weather;
Ticker ticker;

// this array keeps function pointers to all frames
// frames are the single views that slide from right to left
void (*frameCallbacks[3])(int x, int y) = {drawFrame1, drawFrame2, drawFrame3};

// how many frames are there?
int frameCount = 3;
// on frame is currently displayed
int currentFrame = 0;

// your network SSID (name)
char ssid[] = "";
// your network password
char pass[] = "";

// Go to forecast.io and register for an API KEY
String forecastApiKey = "";

// Coordinates of the place you want
// weather information for
char city[] = "SAT";
char state[] = "TX";

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.println("Starting up....");
  // initialize display
  display.begin(SSD1306_SWITCHCAPVCC);
  Serial.println("Display initialized");
  display.display();
  delay(2000);
  display.clearDisplay();
  Serial.println("Display Cleared");
  // set the drawing functions
  // display.setFrameCallbacks(3, frameCallbacks);
  // how many ticks does a slide of frame take?
  // display.setFrameTransitionTicks(10);

  display.fillScreen(BLACK);
  display.setTextSize(2);
  display.setCursor(0,0);
  display.print("WeatherStation 2.0");
  display.display();



  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    display.clearDisplay();
    //display.fillScreen(BLACK);
    //display.fillRect(10, 10, 108, 44, WHITE);
    display.drawXBitmap(34,10, WiFi_Logo_bits, 60, 36, 1);
    // display.fillRect(10, 10, 108, 44, INVERSE);
    drawSpinner(3, counter % 3);
    display.display();
    counter++;
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60 * 10, setReadyForWeatherUpdate);
}

void loop() {
  // if (readyForWeatherUpdate && display.getFrameState() == display.FRAME_STATE_FIX) {
  if (readyForWeatherUpdate) {
    readyForWeatherUpdate = false;
    weather.updateWeatherData(forecastApiKey, city, state);
  }
  display.fillScreen(BLACK);
  display.setTextColor(WHITE);
  // display.nextFrameTick();
  drawFrame1(0, 0);
  display.display();
}

void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;
}

void drawFrame1(int x, int y) {
   display.setTextSize(1);
   display.setCursor(65 + x, 8 + y);
   display.print("Now");
   display.drawXBitmap(x+7,y+7, getIconFromString(weather.getCurrentIcon()), 50, 50, 1);
   display.setTextSize(2);
   display.setCursor(64 + x, 20 + y);
   display.print(String(weather.getCurrentTemp()) + "F");
}

const unsigned char* getIconFromString(String icon) {
   //"clear-day, clear-night, rain, snow, sleet, wind, fog, cloudy, partly-cloudy-day, or partly-cloudy-night"
  if (icon == "clear-day") {
    return clear_day_bits;
  } else if (icon == "clear-night") {
    return clear_night_bits;
  } else if (icon == "rain") {
    return rain_bits;
  } else if (icon == "snow") {
    return snow_bits;
  } else if (icon == "sleet") {
    return sleet_bits;
  } else if (icon == "wind") {
    return wind_bits;
  } else if (icon == "fog") {
    return fog_bits;
  } else if (icon == "cloudy") {
    return cloudy_bits;
  } else if (icon == "partly-cloudy-day") {
    return partly_cloudy_day_bits;
  } else if (icon == "partly-cloudy-night") {
    return partly_cloudy_night_bits;
  }
  return cloudy_bits;
}

void drawFrame2(int x, int y) {
   display.setTextSize(1);
   display.setCursor(65 + x, 0 + y);
   display.print("Today");
   display.drawXBitmap(x,y, xbmtemp, 60, 60, 1);
   display.setTextSize(2);
   display.setCursor(64 + x, 14 + y);
   display.print(String(weather.getCurrentTemp()) + "F");
   display.setTextSize(1);
   display.setCursor(66 + x, 40 + y);
   display.print(String(weather.getMinTempToday()) + "F/" + String(weather.getMaxTempToday()) + "F");

}

void drawFrame3(int x, int y) {
   display.drawXBitmap(x+7,y+7, getIconFromString(weather.getIconTomorrow()), 50, 50, 1);
   display.setTextSize(1);
   display.setCursor(65 + x, 7 + y);
   display.print("Tomorrow");
   display.setTextSize(2);
   display.setCursor(64 + x, 20 + y);
   display.print(String(weather.getMaxTempTomorrow()) + "F");
}

void drawSpinner(int count, int active) {
  for (int i = 0; i < count; i++) {
    const unsigned char *xbm;
    if (active == i) {
       xbm = active_bits;
    } else {
       xbm = inactive_bits;
    }
    display.drawXBitmap(64 - (12 * count / 2) + 12 * i,56, xbm, 8, 8, 1);
  }
}
