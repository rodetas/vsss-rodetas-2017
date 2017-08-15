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

const float frequency = 250; // Hz
const float dt = 1 / frequency; // seconds

int left_cont = 0;
int right_cont = 0;

int direction;
int pwm1 = 0;
int pwm2 = 0;

unsigned long last_millis = 0;

void setup() {

  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM_MOTORB, OUTPUT);
  pinMode(PWM_MOTORA, OUTPUT);

  attachInterrupt(0, encoderLeft, CHANGE);
  attachInterrupt(1, encoderRight, CHANGE);

  Serial.begin(9600);
  xbee.setSerial(Serial);
  
  configTimerControler();
}

void loop() {
  receivingSerial();

  analogWrite(PWM_MOTORA, pwm1);
  analogWrite(PWM_MOTORB, pwm2);

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

ISR(TIMER1_COMPA_vect) {
  
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
    // TIMER 1 for interrupt frequency 200 Hz:
  cli(); // stop interrupts
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  TCNT1  = 0; // initialize counter value to 0
  // set compare match register for 200 Hz increments
  OCR1A = 16000000 / (8 * frequency) - 1 ;//(must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS12, CS11 and CS10 bits for 8 prescaler
  TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei(); // allow interrupts
}