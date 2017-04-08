#include "PinChangeInt.h"

const byte PWM_MOTOR1 = 5;
const byte AIN2 = 6;
const byte AIN1 = 7;
const byte STBY = 8;
const byte BIN1 = 9;
const byte BIN2 = 10;
const byte PWM_MOTOR2 = 11;

unsigned long left_border_micros = 0, right_border_micros = 0;
unsigned long left_last_border_micros = 0, right_last_border_micros = 0;

const float frequency = 150; // Hz
const float dt = 1 / frequency; // seconds
const float speed_calc = ( (2 * PI * 1.6) / 375 ) / 0.000001; // radius, encoder readings, seconds

int left_pwm = 0, right_pwm = 0;
int left_speed = 0, right_speed = 0;
int left_array[5] = {0,0,0,0,0}, right_array[5] = {0,0,0,0,0};

// PID variables 
double KP = 5.3, KI = 220, KD = 0;
double left_integral = 0, right_integral = 0;
double left_error_prior = 0, right_error_prior = 0;

unsigned long fps_time = 0;
int cont = 0;

void setup() {
  pinMode(PWM_MOTOR1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWM_MOTOR2, OUTPUT);

  Serial.begin(115200); 

  pinMode(A0, INPUT);
  PCintPort::attachInterrupt(A0, encoderLeft, RISING);
  pinMode(A3, INPUT);
  PCintPort::attachInterrupt(A3, encoderRight, RISING);

  configTimerControler();
}

void loop() {

  if (millis() - fps_time > 1000){
    Serial.println(cont);
    cont = 0;
    fps_time = millis(); 
  } else {
    cont++;
  }

  forward();

  /*
  left_pwm = 100;
  analogWrite(PWM_MOTOR2, left_pwm);
  delay(1000);
  
  left_pwm = 255;
  analogWrite(PWM_MOTOR2, left_pwm);
  delay(1000);
  */
}

ISR(TIMER1_COMPA_vect) {
  
  left_pwm = PID(70, left_speed, &left_integral, &left_error_prior);
  analogWrite(PWM_MOTOR2, left_pwm);
  
  right_pwm = PID(70, right_speed, &right_integral, &right_error_prior);    
  analogWrite(PWM_MOTOR1, right_pwm);
  
  Serial.print(left_speed);
  Serial.print("\t");
  Serial.print(left_pwm);
  Serial.print("\t");
  Serial.print(right_speed);
  Serial.print("\t");
  Serial.println(right_pwm);
}

int PID(double setpoint, double input, double *integral, double *error_prior){
  double error = 0;
  double derivative = 0;

  error = setpoint - input;
  *integral = *integral + (error * dt);
  derivative = (error - (*error_prior)) / dt;
  *error_prior = error;

  double output = KP*error + KI*(*integral) + KD*derivative;

  if (output > 255) output = 255;
  if (output < 0) output = 0;

  return output;
}

void encoderLeft() {
  left_border_micros = micros() - left_last_border_micros;
  left_last_border_micros = micros();
  left_speed = speedAverage(speed_calc / left_border_micros, left_array);
}

void encoderRight() {
  right_border_micros = micros() - right_last_border_micros;
  right_last_border_micros = micros();
  right_speed = speedAverage(speed_calc / right_border_micros, right_array);
}

int speedAverage(int num, int *array){
  array[0] = array[1];
  array[1] = array[2];
  array[2] = array[3];
  array[3] = array[4];
  array[4] = num;
  return (array[0] + array[1] + array[2] + array[3] + array[4]) / 5;
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

void configTimerControler(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = (16000000/1024/frequency) - 1;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);   
  TCCR1B |= (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
}