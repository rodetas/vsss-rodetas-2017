#include "PinChangeInt.h"

#define ROBO0

#ifdef ROBO0
#define CARACTER_ROBOI '['
#define CARACTER_ROBOF ']'
#endif

#ifdef ROBO1
#define CARACTER_ROBOI '('
#define CARACTER_ROBOF ')'
#endif

#ifdef ROBO2
#define CARACTER_ROBOI '{'
#define CARACTER_ROBOF '}'
#endif

int PWM_MOTOR1 = 5;
int AIN2 = 6;
int AIN1 = 7;
int STBY = 8;
int BIN1 = 9;
int BIN2 = 10;
int PWM_MOTOR2 = 11;

String message;
int pwm = 0;
int failed_read = 0;
int encoder_left = 0;
int encoder_right = 0;
unsigned long last_millis_left = 0;
unsigned long last_millis_right = 0;
unsigned long last_millis_send = 0;

const float speed_calculation =  (2 * PI * 1.6) / 450; //1.6 -> raio da roda, 450 -> numero de leituras que o encoder da em uma volta

void contEncoderLeft();
void contEncoderRight();
void turnRight();
void turnLeft();
void stopped();
void forward();
void back();
void sendParameter();
float speedMotorLeft();
float speedMotorRight();
boolean checkSum();
boolean messageReceived();

void setup() {
  pinMode(PWM_MOTOR1, OUTPUT);//PWMA
  pinMode(AIN2, OUTPUT);//AIN2
  pinMode(AIN1, OUTPUT);//AIN1
  pinMode(STBY, OUTPUT);//STBY
  pinMode(BIN1, OUTPUT);//BIN1
  pinMode(BIN2, OUTPUT);//BIN2
  pinMode(PWM_MOTOR2, OUTPUT);//PWMB

  Serial.begin(19200); 

  pinMode(A0, INPUT);
  PCintPort::attachInterrupt(A0, contEncoderLeft, CHANGE);
  pinMode(A3, INPUT);
  PCintPort::attachInterrupt(A3, contEncoderRight, CHANGE);
}

void loop() {

  if (messageReceived() && checkSum()) {

    int pwm1 = (message.substring(4, 7)).toInt();
    int pwm2 = (message.substring(1, 4)).toInt();

    analogWrite(PWM_MOTOR1, pwm1);
    analogWrite(PWM_MOTOR2, pwm2);
    digitalWrite(STBY, HIGH); //NAO FUNCIONA SEM ISSO!!

    switch (message[0]) {
    case 'A': 
      {
        forward();
      } 
      break;
    case 'V': 
      {
        back();
      } 
      break;
    case 'D': 
      {
        turnRight();
      } 
      break;
    case 'E': 
      {
        turnLeft();
      } 
      break;
    case 'P': 
      {
        stopped();
      } 
      break;
    default: 
      {
        stopped();
      } 
      break;
    }
  } 
  sendParameter();
}
void sendParameter(){
  if (millis() - last_millis_send > 30){
    Serial.print(CARACTER_ROBOI);
    Serial.print(speedMotorLeft());
    Serial.print("|");
    Serial.print(speedMotorRight());
    Serial.print("|");
    Serial.print(failed_read);
    Serial.println(CARACTER_ROBOF);
    last_millis_send = millis();
  }
}


boolean messageReceived() {

  if (Serial.available() > 0) {
    char c = Serial.read();

    if (c == CARACTER_ROBOI) {
      char caracter[10] = "";
      Serial.readBytes(caracter, 10); 

      message = "";
      for (int i = 0; i < 10; i++) {
        message += caracter[i];
      }

      return true;
      failed_read++;
    }
  }
  return false;
}

boolean checkSum() {
  int checkSumRecebido = 0;
  int checkSumLido = 0;

  String num = message.substring(7, 10);
  checkSumRecebido = num.toInt();

  checkSumLido += int(CARACTER_ROBOI);
  checkSumLido += int(CARACTER_ROBOF);

  for (int i = 0 ; i < 7 ; i++) {
    checkSumLido += int(message[i]);
  }

  if (checkSumLido == checkSumRecebido){
    return true;
    failed_read--;
  } 
  return false;
}

void contEncoderLeft() {
  encoder_left++;
}

void contEncoderRight() {
  encoder_right++;
}

float speedMotorLeft() {
  float speed_left = ((speed_calculation * encoder_left) / (millis() - last_millis_left)) * 1000;
  last_millis_left = millis();
  encoder_left = 0;
  return speed_left;
}

float speedMotorRight() {
  float speed_right = ((speed_calculation * encoder_right) / (millis() - last_millis_right)) * 1000;
  last_millis_right = millis();
  encoder_right = 0;
  return speed_right;
}

void forward() {
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void back() {
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void turnRight() {
  digitalWrite(AIN2, LOW);
  digitalWrite(AIN1, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
}

void turnLeft() {
  digitalWrite(AIN2, HIGH);
  digitalWrite(AIN1, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
}

void stopped() {
  digitalWrite(STBY, LOW);
}