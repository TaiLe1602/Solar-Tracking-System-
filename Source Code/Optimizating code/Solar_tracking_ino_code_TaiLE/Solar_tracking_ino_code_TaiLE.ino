#include <Servo.h> 
#include <Wire.h>
#include <stdio.h>
#include <LiquidCrystal_I2C.h>
//Set the LCD address to 0x27 for 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo horizontal; // horizontal servo - thiết lập motor trục đứng
int servoh = 0; 
int servohLimitHigh = 180; 
int servohLimitLow = 0;
// 65 degrees MAX

Servo vertical; // vertical servo - motor quay ngang khai báo motor
int servov = 0; 
int servovLimitHigh = 180; // quay max goc 60 co gi hieu chinh lai
int servovLimitLow = 0;

// LDR pin connections - các giá trị điện trở quang thu về qua analog
// name = analogpin;
int ldrlt = A1; //LDR top left - BOTTOM LEFT <--- BDG// điện trở quang
int ldrrt = A2; //LDR top right - BOTTOM RIGHT
int ldrld = A3; //LDR down left - TOP LEFT
int ldrrd = A4; //LDR down right - TOP RIGHT
int ldrmt = A5; //LDR center


const int motorA =  9;
const int motorB =  8;
  int OpenFlowerValue = 30;
  int timeopen = 2000;
  int timeclose=2000;
  int dtime = 10; int tol = 50;
  int diff(int a, int b, int c, int d);
     static int FlowerStatus = 0;


void setup(){
Serial.begin(19200);// chỉnh cái này
/*lcd.init();
lcd.backlight();
lcd.setCursor(2,0);
lcd.print("TaiLe Project!");*/
horizontal.attach(7);
vertical.attach(6);
horizontal.write(0);
vertical.write(0);
  pinMode(motorA, OUTPUT);//setup motor canh hoa nguoc chieu cung chieu
  pinMode(motorB, OUTPUT);
delay(1);
}
void loop() {
  Serial.println(FlowerStatus);
  while (int(analogRead(ldrmt)) > OpenFlowerValue) {
    if(FlowerStatus == 0)
    {
    FlowerStatus = 1;
    Serial.println(FlowerStatus); // đổi biến trạng thái mở cánh về 1
    digitalWrite(motorA, HIGH); //mở cánh hoa
    digitalWrite(motorB, LOW);
    delay(timeopen); //set time sao cho mo het canh hoa
    digitalWrite(motorA, LOW);
    digitalWrite(motorB, LOW);
    
    }

    //************* tracking ngang
  if(diff(ldrlt,0,0,ldrrd) <= -1*tol || diff(ldrlt,0,0,ldrrd)>=tol)
  {
    while(diff(ldrlt,0,0,ldrrd)<0)
    {
      if((horizontal.read()+1)>servohLimitHigh)
      {
        horizontal.write(servohLimitHigh);
        delay(dtime);
        break;
      }else
      {
        horizontal.write(horizontal.read()+1);
        delay(dtime);
      }
      if(diff(ldrlt,0,0,ldrrd) >= -1*tol && diff(ldrlt,0,0,ldrrd)<=tol)
      {
        delay(dtime);
        break;
      }
    }
    while(diff(ldrlt,0,0,ldrrd)>0)
    {
      if((horizontal.read()-1)<servohLimitLow)
      {
        horizontal.write(servohLimitLow);
        delay(dtime);
        break;
      }else
      {
        horizontal.write(horizontal.read()-1);
        delay(dtime);
      }
      if(diff(ldrlt,0,0,ldrrd) >= -1*tol && diff(ldrlt,0,0,ldrrd)<=tol)
      {
        delay(dtime);
        break;
      }
    }
  }

  //*** tracking trục đứng
  if(diff(0,ldrld,ldrrt,0) <= -1*tol || diff(0,ldrld,ldrrt,0)>=tol)
  {
    while(diff(0,ldrld,ldrrt,0)<0)
    {
      if((vertical.read()+1)>servovLimitHigh)
      {
        vertical.write(servovLimitHigh);
        delay(dtime);
        break;
      }else
      {
        vertical.write(vertical.read()+1);
        delay(dtime);
      }
      if(diff(0,ldrld,ldrrt,0) >= -1*tol && diff(0,ldrld,ldrrt,0)<=tol)
      {
        delay(dtime);
        break;
      }
    }
    while(diff(0,ldrld,ldrrt,0)>0)
    {
      if((vertical.read()-1)<servovLimitLow)
      {
        vertical.write(servovLimitLow);
        delay(dtime);
        break;
      }else
      {
        vertical.write(vertical.read()-1);
        delay(dtime);
      }
      if(diff(0,ldrld,ldrrt,0) >= -1*tol && diff(0,ldrld,ldrrt,0)<=tol)
      {
        delay(dtime);
        break;
      }
    }
  }
  }
  while(int(analogRead(ldrmt)) < OpenFlowerValue)
  {Serial.println(analogRead(ldrmt));
  if(FlowerStatus == 1)
  {
    Serial.println(FlowerStatus);
    FlowerStatus=0;
    digitalWrite(motorA, LOW); //ngược chiều lại với FlowerStatus = 0
    digitalWrite(motorB, HIGH);
    delay(timeclose);
    digitalWrite(motorA,LOW);
    digitalWrite(motorB,LOW);
    horizontal.write(0);
    vertical.write(0);
  }
  }
  Serial.println(analogRead(ldrmt));
  
}

//Function calculate difference
    int diff(int a, int b, int c, int d){
      int averab=(int(analogRead(a))+int(analogRead(b)))/2;
      int avercd=(int(analogRead(c))+int(analogRead(d)))/2;
      return int(averab - avercd); 
    }
void Tracking(int leftvalue, int rightvalue, volatile servotype){
  

}
    //ExxportFunction
    /*void ExportData(void){
    float LDRstatus = float(analogRead(ldrmt));
  Serial.print("Middle:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = float(analogRead(ldrlt));
  Serial.print("Left_Top:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = float(analogRead(ldrld));
  Serial.print("Left_Down:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = float(analogRead(ldrrt));
  Serial.print("Right_Top:"); Serial.print(LDRstatus); Serial.print(",");
  LDRstatus = float(analogRead(ldrrd));
  Serial.print("Right_Down:"); Serial.print(LDRstatus); Serial.print(",");
  int val = analogRead(A0);
  float voltage = (val*10.0)/1024.0;
  lcd.setCursor(0,1);
  lcd.print("Voltage:"); lcd.print(voltage);
  Serial.print("Voltage:"); Serial.println(voltage);
  delay(0);
    }*/
    