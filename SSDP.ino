void SSDP_init(void) {
  server.on("/ssdp", HTTP_GET, [](AsyncWebServerRequest * request) {
    if (request->hasArg("ssdp")) {
      String arg = request->arg("ssdp");
      configSetup = jsonWrite(configSetup, "SSDP", arg);
      configLive = jsonWrite(configLive, "SSDP", arg);
      Serial.println(configSetup);
    }
    request->send(200, "text/json", "Ok");
    saveConfigSetup();
  });

  server.on("/description.xml", HTTP_GET, [](AsyncWebServerRequest * request) {
    String ssdpSend = "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">";
    String  ssdpHeder = xmlNode("major", "1");
    ssdpHeder += xmlNode("minor", "0");
    ssdpHeder = xmlNode("specVersion", ssdpHeder);
    ssdpHeder += xmlNode("URLBase", "http://"+WiFi.localIP().toString());
    String  ssdpDescription = xmlNode("deviceType", "upnp:rootdevice");
    ssdpDescription += xmlNode("friendlyName", jsonRead(configSetup, "SSDP"));
    ssdpDescription += xmlNode("presentationURL", "/");
    ssdpDescription += xmlNode("serialNumber", "28757");
    ssdpDescription += xmlNode("modelName", "ESP8266EX");
    ssdpDescription += xmlNode("modelNumber", "929000226503");
    ssdpDescription += xmlNode("modelURL", "http://esp8266-arduinoide.ru/step3-ssdp/");
    ssdpDescription += xmlNode("manufacturer", "Tretyakov Sergey");
    ssdpDescription += xmlNode("manufacturerURL", "http://www.esp8266-arduinoide.ru");
    ssdpDescription += xmlNode("UDN", "uuid:38323636-4558-4dda-9188-cda0e6"+decToHex(ESP.getChipId(), 6));
    ssdpDescription = xmlNode("device", ssdpDescription);
    ssdpHeder += ssdpDescription;
    ssdpSend += ssdpHeder;
    ssdpSend += "</root>";
    request->send(200, "text/xml", ssdpSend);

  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.begin();
}


String xmlNode(String tags, String data) {
  String temp = "<" + tags + ">" + data + "</" + tags + ">";
  return temp;
}

String decToHex(uint32_t decValue, byte desiredStringLength) {
  String hexString = String(decValue, HEX);
  while (hexString.length() < desiredStringLength) hexString = "0" + hexString;
  return hexString;
}

