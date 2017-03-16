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
int right_array[5] = {0,0,0,0,0};
int speed_left = 0;
int speed_right = 0;
int pwm_left = 0;
int pwm_right = 0;

double error_prior_left = 0, error_prior_right = 0;
double integral_left = 0, integral_right = 0;
double KP = 5.3, KI = 220, KD = 0;

unsigned long fps_time = 0;
int cont = 0;

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

  if (millis() - fps_time > 1000){
    Serial.println(cont);
    cont = 0;
    fps_time = millis(); 
  } else {
    cont++;
  }

  forward();

  /*
  pwm_left = 100;
  analogWrite(PWM_MOTOR2, pwm_left);
  delay(1000);
  
  pwm_left = 255;
  analogWrite(PWM_MOTOR2, pwm_left);
  delay(1000);
  */
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
  left_border_millis = micros() - left_last_border_millis;
  left_last_border_millis = micros();
  speed_left = speedAverage(speed_calc / left_border_millis, left_array);
}

void encoderRight() {
  right_border_millis = micros() - right_last_border_millis;
  right_last_border_millis = micros();
  speed_right = speedAverage(speed_calc / right_border_millis, right_array);
}

void configTimerControler(){
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = (16000000/1024/controler_frequency) - 1;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);   
  TCCR1B |= (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
}

ISR(TIMER1_COMPA_vect) {
  
  pwm_left = PID(70, speed_left, &integral_left, &error_prior_left);
  analogWrite(PWM_MOTOR2, pwm_left);
  
  pwm_right = PID(70, speed_right, &integral_right, &error_prior_right);    
  analogWrite(PWM_MOTOR1, pwm_right);
  
  Serial.print(speed_left);
  Serial.print("\t");
  Serial.print(pwm_left);
  Serial.print("\t");
  Serial.print(speed_right);
  Serial.print("\t");
  Serial.println(pwm_right);
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