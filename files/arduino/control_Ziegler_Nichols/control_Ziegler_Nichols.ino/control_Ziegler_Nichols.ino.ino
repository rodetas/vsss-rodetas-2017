const byte LEFT_ENCODER = 0;
const byte RIGHT_ENCODER = 1;
const byte PWM_MOTOR_LEFT = 5;
const byte AIN2 = 6;
const byte AIN1 = 7;
const byte STBY = 8;
const byte BIN1 = 9;
const byte BIN2 = 10;
const byte PWM_MOTOR_RIGHT = 11;

unsigned long left_border_micros = 0, right_border_micros = 0;
unsigned long left_last_border_micros = 0, right_last_border_micros = 0;
unsigned long last_millis = 0;

const float frequency = 250; // Hz
const float dt = 1 / frequency; // seconds
const float speed_calc = ( (2 * PI * 1.6) / 375 ) / 0.000001; // radius (1.6), encoder readings (375), seconds (0.000001)

int left_pwm = 0, right_pwm = 0;
int left_speed = 0, right_speed = 0;
int left_array_average[8] = {0, 0, 0, 0, 0, 0, 0, 0}, right_array_average[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int left_cont = 0;

// PID variables
double left_setpoint = 0, left_input = 0, left_last_error = 0, left_second_last_error = 0, left_last_output = 0;
double right_setpoint = 0, right_input = 0, right_last_error = 0, right_second_last_error = 0, right_last_output = 0;
double KP = 4.85, KI = 103, KD = 0;  //Cohen Coon
//double KP = 4.24, KI = 23, KD = 0;  //Ziegler Nichols
//double KP = 5.3, KI = 220, KD = 0; //PID Tuning

unsigned long dt_print = 0;

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

  configTimerControler();
  
  Serial.begin(19200);
}

void loop() {

  forward();

  if (millis() - last_millis > 4){
    Serial.print(dt_print);
    Serial.print("\t");
    Serial.print(left_speed);
    Serial.print("\t");
    Serial.println(left_pwm);
    last_millis = millis();
  } 
  
}

ISR(TIMER1_COMPA_vect) {

  left_setpoint = 30;
  left_input = left_speed;
  //left_pwm = PID_LEFT();
  left_pwm = 91;
  analogWrite(PWM_MOTOR_LEFT, left_pwm);

  dt_print += dt*1000;
  
/*
  right_setpoint = 30;
  right_input = right_speed;
  right_pwm = PID_RIGHT();
  //analogWrite(PWM_MOTOR_RIGHT, right_pwm);

  Serial.println(right_speed);
  Serial.print("\t");
  Serial.println(right_pwm);
*/

}

int PID_LEFT() {

  double error = left_setpoint - left_input;
  double proportional = error - left_last_error;
  double integral = error * dt;
  double derivative = (error - (2 * left_last_error) - left_second_last_error) / dt;

  double output = left_last_output + KP * proportional + KI * integral + KD * derivative;

  left_last_error = error;
  left_second_last_error = left_last_error;
  left_last_output = output;

  if (output > 255) output = 255;
  if (output < 0) output = 0;

  return output;
}

int PID_RIGHT() {

  double error = right_setpoint - right_input;
  double proportional = error - right_last_error;
  double integral = error * dt;
  double derivative = (error - (2 * right_last_error) - right_second_last_error) / dt;

  double output = right_last_output + KP * proportional + KI * integral + KD * derivative;

  right_last_error = error;
  right_second_last_error = right_last_error;
  right_last_output = output;

  if (output > 255) output = 255;
  if (output < 0) output = 0;

  return output;
}

void encoderLeft() {
  left_cont++;
  left_border_micros = micros() - left_last_border_micros;
  left_last_border_micros = micros();
  left_speed = speedAverageLeft(speed_calc / left_border_micros);
}

void encoderRight() {
  right_border_micros = micros() - right_last_border_micros;
  right_last_border_micros = micros();
  right_speed = speedAverageRight(speed_calc / right_border_micros);
}

int speedAverageRight(int num) {
  right_array_average[0] = right_array_average[1];
  right_array_average[1] = right_array_average[2];
  right_array_average[2] = right_array_average[3];
  right_array_average[3] = right_array_average[4];
  right_array_average[4] = right_array_average[5];
  right_array_average[5] = right_array_average[6];
  right_array_average[6] = num;
  return (right_array_average[0] + right_array_average[1] + right_array_average[2] + right_array_average[3] + right_array_average[4]) / 5;
}

int speedAverageLeft(int num) {
  left_array_average[0] = left_array_average[1];
  left_array_average[1] = left_array_average[2];
  left_array_average[2] = left_array_average[3];
  left_array_average[3] = left_array_average[4];
  left_array_average[4] = left_array_average[5];
  left_array_average[5] = left_array_average[6];
  left_array_average[6] = num;
  return (left_array_average[0] + left_array_average[1] + left_array_average[2] + left_array_average[3] + left_array_average[4]) / 5;
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
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = (16000000 / 1024 / frequency) - 1;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12);
  TCCR1B |= (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
}
