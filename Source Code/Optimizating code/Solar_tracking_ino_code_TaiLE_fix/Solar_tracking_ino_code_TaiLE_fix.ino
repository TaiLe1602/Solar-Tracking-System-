#include <Servo.h> 
#include <Wire.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>
//Set the LCD address to 0x27 for 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo horizontal;
int servoh = 0; 
int servohLimitHigh = 180; 
int servohLimitLow = 0;
// 65 degrees MAX

Servo vertical; // vertical servo
int servov = 0; 
int servovLimitHigh = 180;
int servovLimitLow = 0;

// LDR pin connections
// name = analogpin;
int ldrlt = A1; //LDR top left - BOTTOM LEFT <--- BDG// điện trở quang
int ldrrt = A2; //LDR top right - BOTTOM RIGHT
int ldrld = A3; //LDR down left - TOP LEFT
int ldrrd = A4; //LDR down right - TOP RIGHT
int ldrmt = A5; //LDR center


const int motorA =  9;
const int motorB =  8;
int OpenFlowerValue = 30;
int timeopen = 3000;
int timeclose=3000;
int dtime = 10; int tol = 50;
int diff(int a, int b, int c, int d);
void Tracking(int leftvalue, int rightvalue, Servo servotype);
static int FlowerStatus = 0;


void setup(){
Serial.begin(19200);

horizontal.attach(7);
vertical.attach(6);
horizontal.write(0);
vertical.write(0);
  pinMode(motorA, OUTPUT);//setup motor canh hoa nguoc chieu cung chieu
  pinMode(motorB, OUTPUT);
delay(1);
}
void loop() {
  while (int(analogRead(ldrmt)) >= OpenFlowerValue) {
    if(FlowerStatus == 0)
    {
    FlowerStatus = 1;
    Serial.println(FlowerStatus);
    digitalWrite(motorA, HIGH);
    digitalWrite(motorB, LOW);
    delay(timeopen); 
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    ExportData();
    }

  //*************
    Tracking(ldrlt, ldrrd, horizontal);
    ExportData();
    delay(300);
  
  //********
    Tracking(ldrld, ldrrt, vertical);
    ExportData();
    delay(300);
  
  }
  while(int(analogRead(ldrmt)) < OpenFlowerValue)
  {;
  if(FlowerStatus == 1)
  {
    FlowerStatus=0;
    horizontal.write(0);
    vertical.write(0);
    delay(500);
    digitalWrite(motorA, LOW); //ngược chiều lại với FlowerStatus = 0
    digitalWrite(motorB, HIGH);
    delay(timeclose);
    digitalWrite(motorA,LOW);
    digitalWrite(motorB,LOW);
    
  }
  }
}

//Function calculate difference
    int diff(int a, int b, int c, int d){
      int averab=(int(analogRead(a))+int(analogRead(b)))/2;
      int avercd=(int(analogRead(c))+int(analogRead(d)))/2;
      return int(averab - avercd); 
    }
void Tracking(int leftvalue, int rightvalue, Servo servotype){
  if(diff(leftvalue,0,0,rightvalue) <= -1*tol || diff(leftvalue,0,0,rightvalue)>=tol)
  {
    while(diff(leftvalue,0,0,rightvalue)<0)
    {
      if((servotype.read()+1)>servohLimitHigh)
      {
        servotype.write(servohLimitHigh);
        delay(dtime);
        break;
      }else
      {
        servotype.write(servotype.read()+1);
        delay(dtime);
      }
      if(diff(leftvalue,0,0,rightvalue) >= -1*tol && diff(leftvalue,0,0,rightvalue)<=tol)
      {
        delay(dtime);
        break;
      }
    }
    while(diff(leftvalue,0,0,rightvalue)>0)
    {
      if((servotype.read()-1)<servohLimitLow)
      {
        servotype.write(servohLimitLow);
        delay(dtime);
        break;
      }else
      {
        servotype.write(servotype.read()-1);
        delay(dtime);
      }
      if(diff(leftvalue,0,0,rightvalue) >= -1*tol && diff(leftvalue,0,0,rightvalue)<=tol)
      {
        delay(dtime);
        break;
      }
    }
  }

}
    //ExxportFunction
    void ExportData(void){
    int LDRstatus = int(analogRead(ldrmt));
  Serial.print("Middle:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = analogRead(ldrlt);
  Serial.print("Left_Top:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = analogRead(ldrlt);
  Serial.print("Left_Down:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = analogRead(ldrlt);
  Serial.print("Right_Top:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = analogRead(ldrlt);
  Serial.print("Right_Down:"); Serial.print(LDRstatus); Serial.println(",");
    }
    