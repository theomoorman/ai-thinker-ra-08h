#include "RA08H.h"
#include <CayenneLPP.h>

#define debugSerial Serial
#define loraSerial Serial2
// Set your OTAA connection details here:
#define DEV_EUI "YOUR_DEV_EUI"
#define APP_EUI "YOUR_APP_EUI"
#define APP_KEY "YOUR_APP_KEY"

RA08H ra08h(debugSerial, loraSerial);
long lastMessageReceiveAttempt = millis();
CayenneLPP lpp(51);

void setup() {
  debugSerial.begin(9600);
  loraSerial.setRX(5);
  loraSerial.setTX(4);
  loraSerial.begin(9600);

  // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;

  ra08h.begin();
  debugSerial.println("getManufacturerIdentification " + ra08h.getManufacturerIdentification());
  debugSerial.println("getModelIdentification " + ra08h.getModelIdentification());
  debugSerial.println("getRevisionIdentification " + ra08h.getRevisionIdentification());
  debugSerial.println("getProductSerialNumberIdentification " + ra08h.getProductSerialNumberIdentification());
  debugSerial.println("setJoinMode " + String(ra08h.setJoinMode(OTAA) ? "true" : "false"));
  debugSerial.println("getJoinMode " + String(ra08h.getJoinMode() == OTAA ? "OTAA" : "ABP"));
  debugSerial.println("setDevEui " + String(ra08h.setDevEui(DEV_EUI)));
  debugSerial.println("getDevEui " + ra08h.getDevEui());
  debugSerial.println("setAppEui " + String(ra08h.setAppEui(APP_EUI)));
  debugSerial.println("getAppEui " + ra08h.getAppEui());
  debugSerial.println("setAppKey " + String(ra08h.setAppKey(APP_KEY)));
  debugSerial.println("getAppKey " + ra08h.getAppKey());
  debugSerial.println("joinOtaa " + String(ra08h.joinOtaa() ? "Joined" : "Not joined"));
  // Send CayenneLPP message (you also need to set this as the uplink decoder in TTN)
  lpp.reset();
  lpp.addPresence(1, 1);
  lpp.addTemperature(2, 26.5f);
  debugSerial.println("sendData " + String(ra08h.sendData(lpp.getBuffer(), lpp.getSize()) ? "true" : "false"));
}

void loop() {
  // Check incoming messages every 5 minutes
  if ((lastMessageReceiveAttempt + 300000) <= millis()) {
    debugSerial.println("receiveData " + String(ra08h.receiveData().hasReceivedData ? "true" : "false"));
    lastMessageReceiveAttempt = millis();
  }
  // Process manual command via Serial
  if (debugSerial.available()) {
    loraSerial.write(debugSerial.read());
  }
  // Output the command result
  if (loraSerial.available()) {
    debugSerial.write(loraSerial.read());
  }
}
