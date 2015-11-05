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

#include "WeatherClient.h"
#include <ESP8266WiFi.h>


void WeatherClient::updateWeatherData(String apiKey, char city[], char state[]) {
  WiFiClient client;
  const int httpPort = 80;
  client.setTimeout(500);
  if(client.connect("services.faa.gov", httpPort)) {
    Serial.println("Connected to services.faa.gov");
  }
  if (!client.connect("172.233.25.61", httpPort)) {
    Serial.println("connection failed");
    return;
  }

  // We now create a URI for the request
  String url = "/airport/status/";
  url += city;

  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: services.faa.gov\r\n" +
               "Content-Type: application/json\r\n" +
               "Accept: application/json\r\n" +
               "Connection: close\r\n\r\n");
  while(!client.available()) {
    Serial.println("Client not Available");
    delay(200);
  }

  // Read all the lines of the reply from server and print them to Serial
  String fcio = "";
  while(client.available()) {
    char c = client.read();
    fcio += c;
  }
  int startOfJson = fcio.indexOf('{');
  fcio = fcio.substring(startOfJson);
  //String fcio = client.readStringUntil('\n');
  Serial.println("Client Available: Reading response");
  // String fcio = client.read();
  fcio += '\0';
  Serial.println(fcio.length());
  //char* fcioJSON = (char *) calloc((fcio.length() + 1), sizeof(char));
  char fcioJSON[(fcio.length() + 1)];
  fcio.toCharArray(fcioJSON, (fcio.length() + 1));
  StaticJsonBuffer<550> jsonBuffer;
  //DynamicJsonBuffer jsonBuffer;
  JsonObject& fcioRoot = jsonBuffer.parseObject(fcioJSON);
  if (!fcioRoot.success()){
    Serial.println("parseObject() failed for API Response");
    return;
  }
  JsonObject& apiRoot = fcioRoot["weather"];

  currentTemp = apiRoot["temp"].asString();
  int currTempCrap = currentTemp.indexOf(".");
  currentTemp.remove(currTempCrap);
  const char* currentHumidity = apiRoot["visibility"];
  const char* currentIcon = apiRoot["weather"];
  const char* currentSummary = apiRoot["weather"];
  const char* maxTempToday = apiRoot["temp"];
  const char* minTempToday = apiRoot["temp"];
  const char* iconToday = apiRoot["weather"];
  const char* summaryToday = apiRoot["weather"];
  const char* maxTempTomorrow = apiRoot["temp"];
  const char* minTempTomorrow = apiRoot["temp"];
  const char* summaryTomorrow = apiRoot["weather"];
  Serial.println(currentTemp);
  Serial.println(currentHumidity);
  Serial.println(currentIcon);
  // currentTemp = (int) apiRoot["temp"];
  // currentHumidity = (int) apiRoot["visibility"];
  // currentTemp = (int) apiRoot["temp"];
  // currentHumidity = (int) apiRoot["visibility"];
  // const char* currentIcon = apiRoot["weather"];
  // const char* currentSummary = apiRoot["weather"];
  // maxTempToday = (int) apiRoot["temp"];
  // minTempToday = (int) apiRoot["temp"];
  // const char* iconToday = apiRoot["weather"];
  // const char* summaryToday = apiRoot["weather"];
  // maxTempTomorrow = (int) apiRoot["temp"];
  // minTempTomorrow = (int) apiRoot["temp"];
  // const char* summaryTomorrow = apiRoot["weather"];


  Serial.println();
  Serial.println("closing connection");
}

void WeatherClient::setUnits(String units) {
   myUnits = units;
}

String WeatherClient::getKey(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }
  return line.substring(0, separatorPosition);
}

String WeatherClient::getValue(String line) {
  int separatorPosition = line.indexOf("=");
  if (separatorPosition == -1) {
    return "";
  }
  return line.substring(separatorPosition + 1);
}


String WeatherClient::getCurrentTemp(void) {
  return currentTemp;
}
int WeatherClient::getCurrentHumidity(void) {
  return currentHumidity;
}
String WeatherClient::getCurrentIcon(void) {
  return currentIcon;
}
String WeatherClient::getCurrentSummary(void) {
  return currentSummary;
}
String WeatherClient::getIconToday(void) {
  return iconToday;
}
int WeatherClient::getMaxTempToday(void) {
  return maxTempToday;
}
int WeatherClient::getMinTempToday(void) {
  return minTempToday;
}
String WeatherClient::getSummaryToday(void) {
  return summaryToday;
}
int WeatherClient::getMaxTempTomorrow(void) {
  return maxTempTomorrow;
}
int WeatherClient::getMinTempTomorrow(void) {
  return minTempTomorrow;
}
String WeatherClient::getIconTomorrow(void) {
  return iconTomorrow;
}
String WeatherClient::getSummaryTomorrow(void) {
  return summaryTomorrow;
}
