#include <WiFi.h>

#define SOUND_SPEED 0.034 //define sound speed in cm/uS
long duration;
float distanceCm;
int previous_command;

#define CMD_STOP 0
#define CMD_FORWARD 1
#define CMD_BACKWARD 2
#define CMD_LEFT 4
#define CMD_RIGHT 8

#define MOVE_DELAY 200

// General purpose leds
#define LED1 33
#define LED2 4
#define LED3 26
#define LED4 5

// General purpose buttons
#define BUT1 22
#define BUT2 21
#define BUT3 19
#define BUT4 18
#define BUT5 17

// Touch pad
#define TOUCH 32

//Motor driver
#define STDBY 13
#define PWMA  23  
#define PWMB  27
#define AIN1  25
#define AIN2  26
#define BIN1  4
#define BIN2  33 

//Ultrasonic sensor
#define ECHO  21  
#define TRG   22

//Line following sensor  
#define OUT1  19
#define OUT2  18
#define OUT3  5
#define OUT4  16
#define OUT5  17

int MAX_Speed = 180;

void setup() {
  Serial.begin(115200);

  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
  pinMode(BUT3, INPUT);
  pinMode(BUT4, INPUT);
  pinMode(BUT5, INPUT);

  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);

  pinMode(TOUCH, INPUT);

  //Line following sensor
  pinMode(OUT1, INPUT);
  pinMode(OUT2, INPUT);
  pinMode(OUT3, INPUT);
  pinMode(OUT4, INPUT);
  pinMode(OUT5, INPUT);

  //Ultrasonic sensor
  pinMode(TRG, OUTPUT);
  pinMode(ECHO, INPUT); 

  pinMode(STDBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);

  CAR_stop();
  analogWrite(PWMA,MAX_Speed);
  analogWrite(PWMB,MAX_Speed);
}

int cntr = 0;
bool flg=false;
double P = 0.08;
double I = 0.002;
double D = 0.2;
int target = 2000;
int last_err = 0;
long long cummulErr = 0;

void loop() {
  while(!flg){
    if(digitalRead(BUT1)){
      flg=true;     
      break;
    }
  }
  CAR_moveForward();

  int pos = sense();
  int err = target - pos;
  cummulErr+=err;
  
  // + I*cummulErr;
  int new_value = P*err + D*(err-last_err);
  analogWrite(PWMA, constrain(MAX_Speed + new_value, 0, MAX_Speed));
  analogWrite(PWMB, constrain(MAX_Speed - new_value, 0, MAX_Speed));
  
  last_err = err;
  delay(10);
}

int sense(){
  int sensorReadings[5];
  if(digitalRead(OUT1)==0) sensorReadings[0] = 1;
    else sensorReadings[0] = 0;
  if(digitalRead(OUT2)==0) sensorReadings[1] = 1;
    else sensorReadings[1] = 0;
  if(digitalRead(OUT3)==0) sensorReadings[2] = 1;
    else sensorReadings[2] = 0;
  if(digitalRead(OUT4)==0) sensorReadings[3] = 1;
    else sensorReadings[3] = 0;
  if(digitalRead(OUT5)==0) sensorReadings[4] = 1;
    else sensorReadings[4] = 0;
  
  int total = 0;
  for (int i = 0; i < 5; i++) {
    total += sensorReadings[i] * (i * 1000);
  }
  return total;
}

void CAR_moveForward() {
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(STDBY,HIGH);
}

void CAR_stop(){
  digitalWrite(STDBY, LOW);
}




