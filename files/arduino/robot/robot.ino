#include <XBee.h>
#include <string.h>

void encoderRight();
void encoderLeft();
void receivingSerial();
void forward();
void back();
void turnRight();
void turnLeft();
void stopped();

XBee xbee = XBee();
Tx16Request tx;
Rx16Response rx = Rx16Response();

const byte AIN2 = 4;
const byte AIN1 = 5;
const byte STBY = 6;
const byte BIN1 = 7;
const byte BIN2 = 8;
const byte PWM_MOTORB = 9;
const byte PWM_MOTORA = 10;

const float frequency = 10; // Hz
const float speed_calc = (2 * PI * 1.6) / 750 ; // radius (1.6), encoder readings (375)

float left_speed = 0;
float right_speed = 0;

int left_cont = 0;
int right_cont = 0;

int direction;
int pwm1 = 200;
int pwm2 = 200;

unsigned long last_millis = 0;

void setup() {
  //void setPwmFrequency();

  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM_MOTORB, OUTPUT);
  pinMode(PWM_MOTORA, OUTPUT);

  attachInterrupt(0, encoderLeft, CHANGE);
  attachInterrupt(1, encoderRight, CHANGE);

  void configTimerControler();

  Serial.begin(9600);
  xbee.setSerial(Serial);
}

void loop() {
  receivingSerial();

  forward();
  analogWrite(PWM_MOTORA, pwm1);
  analogWrite(PWM_MOTORB, pwm2);

  if (millis() - last_millis > 4) {
    Serial.print("LEFT: ");
    Serial.println(left_speed);
    Serial.print("RIGHT: ");
    Serial.println(right_speed);
    Serial.println("");
    last_millis = millis();
  }

  switch (direction) {
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

ISR(TIMER2_COMPA_vect) {
  left_speed = left_cont;
  right_speed = right_cont;
}

void receivingSerial() {
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {

    if (xbee.getResponse().getApiId() == RX_16_RESPONSE) {

      xbee.getResponse().getRx16Response(rx);

      direction = int(rx.getData(0));
      pwm1 = (rx.getData(1) - '0') * 100 + (rx.getData(2) - '0') * 10 + (rx.getData(3) - '0');
      pwm2 = (rx.getData(4) - '0') * 100 + (rx.getData(5) - '0') * 10 + (rx.getData(6) - '0');
    }
  }
}

void encoderLeft() {
  left_cont++;
}

void encoderRight() {
  right_cont++;
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

void configTimerControler() {
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}

// Set pin 9 and 10 PWM frequency to 31250 Hz (31250/1 = 31250)
void setPwmFrequency() {
  byte mode = 0x01;
  TCCR1B = TCCR1B & 0b11111000 | mode;
  TCCR2B = TCCR2B & 0b11111000 | mode;
}
