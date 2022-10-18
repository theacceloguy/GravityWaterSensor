#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#define SERVER_IP "http://fabfest.theacceloguy.com/backend/append"

#ifndef STASSID
#define STASSID "wifihost"
#define STAPSK "qwertyui"
#endif

String macAddr;

void setup() {
  macAddr = WiFi.macAddress();
  
  // Open serial communications and wait for port to open:
  Serial.begin(115200);

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
}
void loop() {
 // Serial.println(macAddr);

  // run over and over
  if (Serial.available()) {
    String message = Serial.readString();
    Serial.println(message);
    if ((WiFi.status() == WL_CONNECTED)) {

      DynamicJsonDocument doc(1024);
      String json;

      DeserializationError error = deserializeJson(doc, message);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
      }

      WiFiClient client;
      HTTPClient http;

      Serial.print("[HTTP] begin...\n");
      // configure traged server and url
      http.begin(client, SERVER_IP);  //HTTP
      http.addHeader("Content-Type", "application/json");
      Serial.print("[HTTP] POST...\n");
  
      doc["mac"] = macAddr;
      serializeJson(doc,json);
      serializeJson(doc,Serial);

      int httpCode = http.POST(json);

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        if (httpCode == HTTP_CODE_OK) {
          const String& payload = http.getString();
          Serial.println("received payload:\n<<");
          Serial.println(payload);
          Serial.println(">>");
        }
      } else {
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
    }
  }
}
