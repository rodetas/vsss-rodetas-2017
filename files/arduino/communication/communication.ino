#define COORDINATOR_XBEE 3

#include <XBee.h>
#include <string.h>

// Xbee.h variables
XBee xbee = XBee();
Tx16Request tx;
Rx16Response rx = Rx16Response();

void setup() {
  Serial.begin(9600);
  xbee.setSerial(Serial);
  Serial.println("Ready");
}

void loop() {
  //sendData();
  receivingData();
  //receivingSerial();
}

String receivingData() {
  String message = "";
  xbee.readPacket();
  if (xbee.getResponse().isAvailable()) {
    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {
      xbee.getResponse().getRx16Response(rx);
      for (int i = 0; i < rx.getDataLength(); i++) {
        message += rx.getData(i);
      }
      Serial.println(message);
    }
  } else if (xbee.getResponse().isError()) {
    message = "Error code: " + xbee.getResponse().getErrorCode();
  }

  return message;
}

void sendData(uint8_t send_buffer[]) {
  tx = Tx16Request(COORDINATOR_XBEE, send_buffer, sizeof(send_buffer));
  xbee.send(tx);
}

// Apenas pela serial sem utilização do Xbee.h
String receivingSerial() {
  String message;
  
  if (Serial.available() > 0) {
    char buffer_rx[16];
    int size_buffer = Serial.readBytes(buffer_rx, 16);
    for (int i = 7; i < 15; i++) {
      message += buffer_rx[i];
    }
    Serial.println(message);
  }
  
  return message;
}