#include <ESP8266WiFi.h>
#include <FS.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer
#include <SPIFFSEditor.h>
#include <ESP8266SSDP.h>        //Содержится в пакете. Видео с уроком http://esp8266-arduinoide.ru/step3-ssdp
#include <ArduinoJson.h>        //Ставим через менеджер библиотек

#include "AudioFileSourceHTTPStream.h"
#include "AudioGeneratorMP3.h"
#include "AudioOutputI2SNoDAC.h" //https://github.com/earlephilhower/ESP8266Audio
AudioGeneratorMP3 *mp3;
AudioFileSourceHTTPStream *file;
AudioOutputI2SNoDAC *out;

// SKETCH BEGIN
AsyncWebServer server(80);

String configSetup = "{}";
String configOptions = "{}";
String configLive = "{}";

uint8_t RELE1_PIN = 13;  // Реле
uint8_t LED_PIN = 13;    // Светодиод

boolean state0;


void setup() {
  Serial.begin(115200);
  SPIFFS.begin();
  configSetup = readFile("config.save.json", 4096);
  configSetup = jsonWrite(configSetup, "time", "00:00:00");
  configOptions = jsonWrite(configOptions, "lang", jsonRead(configSetup, "lang"));
  configOptions = jsonWrite(configOptions, "SSDP", jsonRead(configSetup, "SSDP"));
  WIFIinit();
  SSDP_init();
  WEB_init();
  AUDIO_init();

}

void loop() {
  //handleAudio();
}
