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

int pwm_motor1 = 5;
int ain2 = 6;
int ain1 = 7;
int stby = 8;
int bin1 = 9;
int bin2 = 10;
int pwm_motor2 = 11;

int encoder_left = 0;
int encoder_right = 0;

int pwm = 0;
String message;

unsigned long last_millis_left = 0;
unsigned long last_millis_right = 0;

const float speed_calculation =  (2 * PI * 1.6) / 450; //1.6 -> raio da roda, 450 -> numero de leituras que o encoder da em uma volta

void contEncoderLeft();
void contEncoderRight();
void turnRight();
void turnLeft();
void stopped();
void forward();
void back();
void motorTest();
float speedMotorLeft();
float speedMotorRight();
boolean checkSum();
boolean messageReceived();

void setup() {
  pinMode(pwm_motor1, OUTPUT);//PWMA
  pinMode(ain2, OUTPUT);//AIN2
  pinMode(ain1, OUTPUT);//AIN1
  pinMode(stby, OUTPUT);//STBY
  pinMode(bin1, OUTPUT);//BIN1
  pinMode(bin2, OUTPUT);//BIN2
  pinMode(pwm_motor2, OUTPUT);//PWMB

  Serial.begin(19200); 

  pinMode(A0, INPUT);
  PCintPort::attachInterrupt(A0, contEncoderLeft, CHANGE);
  pinMode(A3, INPUT);
  PCintPort::attachInterrupt(A3, contEncoderRight, CHANGE);
}

void loop() {

  if (messageReceived() && checkSum()) {

    String pwm1_s = message.substring(4, 7);
    String pwm2_s = message.substring(1, 4);

    int pwm1 = pwm1_s.toInt();
    int pwm2 = pwm2_s.toInt();

    analogWrite(pwm_motor1, pwm1);
    analogWrite(pwm_motor2, pwm2);
    digitalWrite(stby, HIGH); //NAO FUNCIONA SEM ISSO!!

    char direcao = message[0];
    switch (direcao) {
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

  Serial.print("[");
  Serial.print(speedMotorLeft());
  Serial.print("|");
  Serial.print(speedMotorRight());
  Serial.println("]");
  
}


boolean messageReceived() {
message = "";
  if (Serial.available() > 0) {
    char c = Serial.read();
    char caracter[10] = "";

    if (c == CARACTER_ROBOI) {
      Serial.readBytes(caracter, 10);

      for (int i = 0; i < 10; i++) {
        message = message + caracter[i];
      }
      return true;

    } else {
      return false;
    }
  } else {
    return false;
  }
}

boolean checkSum() {
  int checkSumRecebido = 1;
  int checkSumLido = 0;

  String num = message.substring(7, 10);
  checkSumRecebido = num.toInt();

  checkSumLido += int(CARACTER_ROBOI);
  checkSumLido += int(CARACTER_ROBOF);

  for (int i = 0 ; i < 7 ; i++) {
    checkSumLido += int(message[i]);
  }

  if (checkSumLido == checkSumRecebido) return true;
  else return false;

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
  digitalWrite(ain2, HIGH);
  digitalWrite(ain1, LOW);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void back() {
  digitalWrite(ain2, LOW);
  digitalWrite(ain1, HIGH);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void turnRight() {
  digitalWrite(ain2, LOW);
  digitalWrite(ain1, HIGH);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void turnLeft() {
  digitalWrite(ain2, HIGH);
  digitalWrite(ain1, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void stopped() {
  digitalWrite(stby, LOW);
}