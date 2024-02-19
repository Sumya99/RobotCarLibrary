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
  CAR_speed_wheelA(100);
  CAR_speed_wheelB(100);
}

int cntr = 0;

void loop() {
  int out1 = digitalRead(OUT1);
  int out2 = digitalRead(OUT2);
  int out3 = digitalRead(OUT3);
  int out4 = digitalRead(OUT4);
  int out5 = digitalRead(OUT5);

  Serial.print(out1);
  Serial.print(out2);
  Serial.print(out3);
  Serial.print(out4);
  Serial.println(out5);
  
  if(out3==0){
    if(out2==1 && out4==1){
      CAR_moveForward();
    } else {
      if(out2==0){
        CAR_turnLeft();
        wait_middle_sensor();
      } else if(out4==0) {
        CAR_turnRight();
        wait_middle_sensor();
      } else if(out1==0){
        CAR_turnLeft();
        wait_middle_sensor();
      } else if(out5==0) {
        CAR_turnRight();
        wait_middle_sensor();
      } else {
        CAR_moveForward();
      }
    }
  } else {
    if(out2==0){
      CAR_turnLeft();
      wait_middle_sensor();
    } else if(out4==0){
      CAR_turnRight();
      wait_middle_sensor();
    } else if(out1==0){
      CAR_turnLeft();
      wait_middle_sensor();
    } else if(out5==0){
      CAR_turnRight();
      wait_middle_sensor();
    } else {
      CAR_moveForward();
    }
  }
}

void wait_middle_sensor(){
  while(digitalRead(OUT3)==1);
}

void CAR_moveForward() {
  //if(previous_command == CMD_BACKWARD){
    // CAR_stop();
    // delay(MOVE_DELAY);
 // }
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(STDBY,HIGH);
  previous_command = CMD_FORWARD;
}

void CAR_moveBackward() {
 // if(previous_command == CMD_FORWARD){
    // CAR_stop();
    // delay(MOVE_DELAY);
  //}
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(STDBY,HIGH);
  previous_command = CMD_BACKWARD;
}

void CAR_turnLeft() {
  //if(previous_command == CMD_RIGHT){
    // CAR_stop();
    // delay(MOVE_DELAY);
  //}
  digitalWrite(AIN1, HIGH);
  digitalWrite(AIN2, LOW);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);
  digitalWrite(STDBY,HIGH);
  previous_command = CMD_LEFT;
}

void CAR_turnRight() {
 // if(previous_command == CMD_LEFT){
    // CAR_stop();
    // delay(MOVE_DELAY);
  //}
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH);
  digitalWrite(BIN2, LOW);
  digitalWrite(STDBY,HIGH);
  previous_command = CMD_RIGHT;
}

void CAR_stop() {
  // previous_command = CMD_STOP;
  digitalWrite(STDBY,LOW);
}

void CAR_speed_wheelA(uint8_t val) {
    if(val>175) val =175;
    else if(val<0) val =0;
    analogWrite(PWMA,val);
}
void CAR_speed_wheelB(uint8_t val) {
    if(val>175) val =175;
    else if(val<0) val =0;
    analogWrite(PWMB,val);
}