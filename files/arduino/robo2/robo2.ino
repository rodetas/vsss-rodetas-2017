#include <SoftwareSerial.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define ROBO2

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

#define rxPin 0
#define txPin 1

SoftwareSerial XBee =  SoftwareSerial(rxPin, txPin);

int pwm_motor1 = 5;
int ain2 = 6;
int ain1 = 7;
int stby = 8;
int bin1 = 9;
int bin2 = 10;
int pwm_motor2 = 11;

String mensagem;

boolean recebeMensagem();
boolean verificaCheckSum();
void paraFrente();
void paraTras();
void paraDireita();
void paraEsquerda();
void parado();

void setup() {
  pinMode(pwm_motor1, OUTPUT);//PWMA
  pinMode(ain2, OUTPUT);//AIN2
  pinMode(ain1, OUTPUT);//AIN1
  pinMode(stby, OUTPUT);//STBY
  pinMode(bin1, OUTPUT);//BIN1
  pinMode(bin2, OUTPUT);//BIN2
  pinMode(pwm_motor2, OUTPUT);//PWMB

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  XBee.begin(19200);
  Serial.begin(19200);
}

void loop() {

  //  549
  // [ A 123 123 600 ]
  //   mensagem = "A123123549]";

  if (XBee.overflow()) {
    Serial.println("Xbee overflow!");
  }
  
  mensagem = "";
  if (recebeMensagem() && verificaCheckSum()) {

    String pwm1_s = mensagem.substring(4, 7);
    String pwm2_s = mensagem.substring(1, 4);

    int pwm1 = pwm1_s.toInt();
    int pwm2 = pwm2_s.toInt();

    
      // Serial.print(pwm1);
      // Serial.print(" ");
      // Serial.print(pwm2);
      // Serial.print(" ");
      // Serial.println(mensagem[0]);
    
    
    analogWrite(pwm_motor1, pwm1);
    analogWrite(pwm_motor2, pwm2);
    digitalWrite(stby, HIGH); //NAO FUNCIONA SEM ISSO!!

    char direcao = mensagem[0];
    switch (direcao) {
      case 'A': {
          paraFrente();
        } break;
      case 'V': {
          paraTras();
        } break;
      case 'D': {
          paraDireita();
        } break;
      case 'E': {
          paraEsquerda();
        } break;
      case 'P': {
          parado();
        } break;
      default: {
          parado();
        } break;
    }
  }
}

boolean recebeMensagem() {
  if (XBee.available() > 0) {
    char c = XBee.read();
    char caracter[10] = "";

    if (c == CARACTER_ROBOI) {
      XBee.readBytes(caracter, 10);
      
      for (int i = 0; i < 10; i++) {
        mensagem = mensagem + caracter[i];
      }
      
      //Serial.println(mensagem);

      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

boolean verificaCheckSum() {
  int checkSumRecebido = 1;
  int checkSumLido = 0;

  String num = mensagem.substring(7, 10);
  checkSumRecebido = num.toInt();

  checkSumLido += int(CARACTER_ROBOI);
  checkSumLido += int(CARACTER_ROBOF);

  for (int i = 0 ; i < 7 ; i++) {
    checkSumLido += int(mensagem[i]);
  }

  if (checkSumLido == checkSumRecebido) return true;
  else return false;

}

void paraFrente() {
  digitalWrite(ain2, HIGH);
  digitalWrite(ain1, LOW);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void paraTras() {
  digitalWrite(ain2, LOW);
  digitalWrite(ain1, HIGH);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void paraDireita() {
  digitalWrite(ain2, LOW);
  digitalWrite(ain1, HIGH);
  digitalWrite(bin1, HIGH);
  digitalWrite(bin2, LOW);
}

void paraEsquerda() {
  digitalWrite(ain2, HIGH);
  digitalWrite(ain1, LOW);
  digitalWrite(bin1, LOW);
  digitalWrite(bin2, HIGH);
}

void parado() {
  digitalWrite(stby, LOW);
}
