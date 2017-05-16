const byte LEFT_ENCODER = 0;
const byte RIGHT_ENCODER = 1;
const byte PWM_MOTOR_LEFT = 5;
const byte AIN2 = 6;
const byte AIN1 = 7;
const byte STBY = 8;
const byte BIN1 = 9;
const byte BIN2 = 10;
const byte PWM_MOTOR_RIGHT = 11;

int left_cont = 0, right_cont = 0;

void setup() {

  pinMode(PWM_MOTOR_LEFT, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM_MOTOR_RIGHT, OUTPUT);

  pinMode(LEFT_ENCODER, INPUT_PULLUP);
  attachInterrupt(0, encoderLeft, RISING);
  pinMode(RIGHT_ENCODER, INPUT_PULLUP);
  attachInterrupt(1, encoderRight, RISING);

  Serial.begin(9600);
}

void loop() {
  forward();

  analogWrite(PWM_MOTOR_RIGHT, 150);
  //Serial.println(left_cont);
 Serial.println(right_cont);

  //analogWrite(PWM_MOTOR_LEFT, 150);
  
  

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
