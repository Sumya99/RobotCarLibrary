#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"
//#include "index.h"

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

void setup(){

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

  pinMode(STDBY, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  digitalWrite(AIN1, LOW);
  digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, LOW);
  digitalWrite(BIN2, HIGH);

  digitalWrite(STDBY,HIGH);

  analogWrite(PWMA, 50);
  analogWrite(PWMB, 50);

  Serial.begin(9600);

}

void loop(){ 
  // int but1= digitalRead(BUT1);
  // int but2= digitalRead(BUT2);
  int but3= digitalRead(BUT3);
  int but4= digitalRead(BUT4);
  int but5= digitalRead(BUT5);

  // if(but1) digitalWrite(LED1, LOW);
  // else digitalWrite(LED1, HIGH);

  // if(but2) digitalWrite(LED2, LOW);
  // else digitalWrite(LED2, HIGH);

  // if(but3) digitalWrite(LED3, LOW);
  // else digitalWrite(LED3, HIGH);

  // if(but4) digitalWrite(LED4, LOW);
  // else digitalWrite(LED4, HIGH);

  // if(but5) digitalWrite(LED4, LOW);
  // else if(but5&but4) digitalWrite(LED4, HIGH);

  // //Touch sensor
  // uint64_t touch = analogRead(TOUCH);
  // Serial.println(touch);
  // if(touch>4000){
  //   digitalWrite(LED1,LOW);
  //   digitalWrite(LED2,LOW);
  //   digitalWrite(LED3,LOW);
  //   digitalWrite(LED4,LOW);
  //   delay(100);
  // }else{
  //   digitalWrite(LED1,HIGH);
  //   digitalWrite(LED2,HIGH);
  //   digitalWrite(LED3,HIGH);
  //   digitalWrite(LED4,HIGH);
  // }




  //Motor driver test
  if(but5){ //button 5 is configure command
    digitalWrite(STDBY,LOW);
    if(but4){
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    }else if(but3){
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    }
  }
  digitalWrite(STDBY,HIGH);

  analogWrite(PWMA, 100);
  analogWrite(PWMB, 100);
  delay(500);
}



