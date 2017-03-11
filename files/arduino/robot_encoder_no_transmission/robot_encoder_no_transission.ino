#include "PinChangeInt.h"

const byte PWM_MOTOR1 = 5;
const byte AIN2 = 6;
const byte AIN1 = 7;
const byte STBY = 8;
const byte BIN1 = 9;
const byte BIN2 = 10;
const byte PWM_MOTOR2 = 11;

unsigned long left_border_millis = 0;
unsigned long left_last_border_millis = 0;
unsigned long right_border_millis = 0;
unsigned long right_last_border_millis = 0;

const float controler_frequency = 150; // Hz
const float dt = 1 / controler_frequency;
const float speed_calc = ( (2 * PI * 1.6) / 375 ) / 0.000001; // 1.6 -> raio | 375 -> leituras encoder | 0.000001 -> ajuste de microsegundo para segundo

int left_array[5] = {0,0,0,0,0};
int speed_left = 0;
int speed_right = 0;
int pwm_left = 0;
int pwm_right = 0;
double time = 0;

double error = 0, error_prior = 0;
double integral = 0, derivative = 0;
double KP = 15, KI = 0, KD = 0;

void encoderLeft();
void encoderRight();

void back();
void forward();
void stopped();
void turnLeft();
void turnRight();

void setup() {
  pinMode(PWM_MOTOR1, OUTPUT);//PWMA
  pinMode(AIN2, OUTPUT);//AIN2
  pinMode(AIN1, OUTPUT);//AIN1
  pinMode(STBY, OUTPUT);//STBY
  pinMode(BIN1, OUTPUT);//BIN1
  pinMode(BIN2, OUTPUT);//BIN2
  pinMode(PWM_MOTOR2, OUTPUT);//PWMB

  Serial.begin(115200); 

  pinMode(A0, INPUT);
  PCintPort::attachInterrupt(A0, encoderLeft, RISING);
  pinMode(A3, INPUT);
  PCintPort::attachInterrupt(A3, encoderRight, RISING);

  configTimerControler();
}

void loop() {

  forward();
  
  analogWrite(PWM_MOTOR2, 100);
  delay(1000);
  
  analogWrite(PWM_MOTOR2, 255);
  delay(1000);
  
}

int PID(double setpoint, double input){

  error = setpoint - input;
  integral = integral + (error * dt);
  derivative = (error - error_prior) / dt;
  error_prior = error;

  double output = KP*error + KI*integral + KD*derivative;

  if (output > 255) output = 255;
  if (output < 0) output = 0;

  return setpoint + output;
}

void encoderLeft() {
  left_border_millis = micros() - left_last_border_millis;
  left_last_border_millis = micros();
  speed_left = speedAverage(speed_calc / left_border_millis);
  //speed_left = (speed_left + speed_calc / left_border_millis) / 2;
}

void encoderRight() {
  right_border_millis = micros() - right_last_border_millis;
  right_last_border_millis = micros();
  speed_right = (speed_right + speed_calc / right_border_millis) / 2;
}

void configTimerControler(){
  // INICILALIZANDO OS REGISTRADORES COM ZERO
  TCCR1A = 0;
  TCCR1B = 0;
  // O VALOR REAL DO TEMPORIZADOR É ARMAZENADO AKI.
  TCNT1  = 0;
  // DEFININDO TEMPORIZADOR PARA  CADA 33ms OU 30Hz.
  OCR1A = (16000000/1024/controler_frequency) - 1;// = 16BIT -> [(16Mhz/1024)/30Hz]  --> (7812.5 <65536)
  // ATIVANDO MODO DE OPERAÇÃO CTC
  TCCR1B |= (1 << WGM12);
  // ESCOLHENDO PRESCALER DE 1024
  TCCR1B |= (1 << CS12);   
  TCCR1B |= (1 << CS10);  
  // ATIVANDO O MODO CTC PARA TIMER1
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  //pwm_left = PID(15, speed_left);
  //pwm_right = PID(15, speed_right);
    
  //analogWrite(PWM_MOTOR1, pwm_right);
  //analogWrite(PWM_MOTOR2, pwm_left);
  
  time += dt; 
  
  Serial.print(speed_left);
  Serial.print(" | ");
  Serial.println(time);
}

int speedAverageLeft(int num){
  left_array[5] = left_array[4];
  left_array[4] = left_array[3];
  left_array[3] = left_array[2];
  left_array[2] = left_array[1];
  left_array[1] = left_array[0];
  left_array[0] = num;
  
  return (left_array[0] + left_array[1] + left_array[2] + left_array[3] + left_array[4])/5;
}

int speedAverageRight(int num){
  right_array[5] = right_array[4];
  right_array[4] = right_array[3];
  right_array[3] = right_array[2];
  right_array[2] = right_array[1];
  right_array[1] = right_array[0];
  right_array[0] = num;
  
  return (left_array[0] + left_array[1] + left_array[2] + left_array[3] + left_array[4])/5;
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