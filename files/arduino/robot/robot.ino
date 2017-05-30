#include <XBee.h>
#include <string.h>

XBee xbee = XBee();
Tx16Request tx;
Rx16Response rx = Rx16Response();

const byte PWM_MOTOR1 = 5;
const byte AIN2 = 6;
const byte AIN1 = 7;
const byte STBY = 8;
const byte BIN1 = 9;
const byte BIN2 = 10;
const byte PWM_MOTOR2 = 11;

void setup() {
  pinMode(PWM_MOTOR1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM_MOTOR2, OUTPUT);

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {
  String message = receivingSerial();

  analogWrite(PWM_MOTOR1, 0);
  analogWrite(PWM_MOTOR2, 0);

  switch (message[0]) {
    case 'F': {
        forward();
      } break;
    case 'B': {
        back();
      } break;
    case 'R': {
        turnRight();
      } break;
    case 'L': {
        turnLeft();
      } break;
    case 'S': {
        stopped();
      } break;
  }
}

String receivingSerial() {
  String buf = "";
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {

    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {

      xbee.getResponse().getRx16Response(rx);
      for (int i = 0; i < rx.getDataLength(); i++) {
        buf += rx.getData(i);
      }
    }
  }

  Serial.println(buf);

  return buf;
}

void forward() {
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(STBY, HIGH);
}

void back() {
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(STBY, HIGH);
}

void turnRight() {
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(STBY, HIGH);
}

void turnLeft() {
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(STBY, HIGH);
}

void stopped() {
  digitalWrite(STBY, LOW);
}
