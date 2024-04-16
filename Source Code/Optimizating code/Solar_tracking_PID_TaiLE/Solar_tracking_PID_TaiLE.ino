#include <Servo.h> 
#include <Wire.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>
//Set the LCD address to 0x27 for 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo horizontal; // horizontal servo
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
int ldrlt = A1; //LDR top left - BOTTOM LEFT 
int ldrrt = A2; //LDR top right - BOTTOM RIGHT
int ldrld = A3; //LDR down left - TOP LEFT
int ldrrd = A4; //LDR down right - TOP RIGHT
int ldrmt = A5; //LDR center


const int motorA =  9;
const int motorB =  8;
int OpenFlowerValue = 40;
int timeopen = 3000;
int timeclose=3000;
int dtime = 10; int tol = 50;
int diff(int a, int b, int c, int d);
void Tracking(int leftvalue, int rightvalue, Servo servotype);
static int FlowerStatus = 0;
static int e;
static int dE =0;
static int zE = 0;
double Kph =0.0001, Kih =0.0001, Kdh = 0.0002;

void setup(){
Serial.begin(9600);

horizontal.attach(7);
vertical.attach(6);
horizontal.write(0);
vertical.write(0);
  pinMode(motorA, OUTPUT);
  pinMode(motorB, OUTPUT);
delay(1);
}
void loop() {
  while (int(analogRead(ldrmt)) >= OpenFlowerValue) {
    if(FlowerStatus == 0)
    {
    FlowerStatus = 1;// đổi biến trạng thái mở cánh về 1
    digitalWrite(motorA, HIGH); //mở cánh hoa
    digitalWrite(motorB, LOW);
    delay(timeopen); //set time sao cho mo het canh hoa
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    ExportData();
    
    }

  //************* tracking HORIZONTAL
    e = diff(ldrlt,0,0, ldrrd);
    zE = 0;
    dE =0;

    Tracking(ldrlt, ldrrd, horizontal);
  
  //******** tracking VERTICAL
    e = diff(ldrld,0,0, ldrrt);
    zE = 0;
    dE =0;
    Tracking(ldrld, ldrrt, vertical);
    ExportData();
  
  }
  while(int(analogRead(ldrmt)) < OpenFlowerValue)
  {
  if(FlowerStatus == 1)
  {
    FlowerStatus=0;
    horizontal.write(0);
    vertical.write(0);
    delay(500);
    digitalWrite(motorA, LOW); //
    digitalWrite(motorB, HIGH);
    delay(timeclose);
    digitalWrite(motorA,LOW);
    digitalWrite(motorB,LOW);
    delay(300);
  }
  }
}

//Function calculate difference
    int diff(int a, int b, int c, int d){
      int averab=int(analogRead(a));
      int avercd=int(analogRead(d));
      return int(avercd - averab); 
    }
void Tracking(int leftvalue, int rightvalue, Servo servotype){
 
    while(diff(leftvalue,0,0,rightvalue) < -1*tol || diff(leftvalue,0,0,rightvalue)>tol)
    {
      double delta = Kph*e + Kih*zE + Kdh*dE;
      Serial.print("PID:"); Serial.println(e);
      if((servotype.read()+int(delta))<0){
        servotype.write(0);
      } else if((servotype.read()+int(delta))>180){
        servotype.write(180);
      } else {servotype.write(servotype.read()+int(delta));delay(dtime);}
      dE = -e+diff(leftvalue,0,0,rightvalue);
      e = diff(leftvalue,0,0,rightvalue);
      zE+=e;
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
    