void WEB_init(void) {
  server.addHandler(new SPIFFSEditor());
  server.serveStatic("/", SPIFFS, "/").setDefaultFile("index.htm");
  server.onNotFound([](AsyncWebServerRequest * request) {
    request->send(404, "text/plain", "FileNotFound");
  });
  server.begin();
  server.on("/ssid", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("ssid")) {
      configSetup = jsonWrite(configSetup, "ssid", request->arg("ssid"));
    }
    if (request->hasArg("password")) {
      configSetup = jsonWrite(configSetup, "ssidPass", request->arg("password"));
    }
    request->send(200, "text/plain", "Ok");
    saveConfigSetup();
  });
  server.on("/ssidap", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("ssidAP")) {
      configSetup = jsonWrite(configSetup, "ssidAP", request->arg("ssidAP"));
    }
    if (request->hasArg("passwordAP")) {
      configSetup = jsonWrite(configSetup, "passwordAP", request->arg("passwordAP"));
    }
    request->send(200, "text/plain", "Ok");
    saveConfigSetup();
  });
  server.on("/restart", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("device")) {
      if (request->arg("device") == "ok") {                         // Если значение равно Ок
        request->send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
        ESP.restart();                                // перезагружаем модуль
      }
      else {                                        // иначе
        request->send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
      }
    }
  });
  server.on("/config.setup.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/json", configSetup);
  });
  server.on("/config.live.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/json", configLive);
  });
  server.on("/config.options.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    request->send(200, "text/json", configOptions);
  });
  server.on("/wifi.scan.json", HTTP_GET, [](AsyncWebServerRequest * request) {
    DynamicJsonBuffer jsonBuffer;
    JsonObject& json = jsonBuffer.createObject();
    JsonArray& networks = json.createNestedArray("networks");
    int n = WiFi.scanComplete();
    if (n == -2) {
      WiFi.scanNetworks(true);
    } else if (n) {
      for (int i = 0; i < n; i++) {
        JsonObject& data = networks.createNestedObject();
        data["ssid"] = WiFi.SSID(i);
        data["pass"] = (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*";
        data["dbm"] = WiFi.RSSI(i);
      }
      WiFi.scanDelete();
      if (WiFi.scanComplete() == -2) {
        WiFi.scanNetworks(true);
      }
    }
    String root;
    json.printTo(root);
    request->send(200, "application/json", root);
  });
}
