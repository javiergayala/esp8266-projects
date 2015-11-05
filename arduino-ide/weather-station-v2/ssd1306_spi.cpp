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

Credits for parts of this code go to Mike Rankin. Thank you so much for sharing!
*/
#include <Arduino.h>
#include <Wire.h>
#include <ESP_SSD1306.h>
#include "ssd1306_spi.h"

SSD1306::SSD1306(int oled_mosi, int oled_clk, int oled_dc, int oled_reset, int oled_cs)
{
  spi_mosi = oled_mosi;
  spi_clk = oled_clk;
  spi_dc = oled_dc;
  spi_reset = oled_reset;
  spi_cs = oled_cs;
}

void SSD1306::init() {
  /* Uncomment this for Software SPI
  Adafruit_SSD1306 display(spi_mosi, spi_clk, spi_dc, spi_reset, spi_cs);
  */
  ESP_SSD1306 display(spi_dc, spi_reset, spi_cs);
}
