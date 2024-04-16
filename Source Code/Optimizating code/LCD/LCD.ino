#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//Set the LCD address to 0x27 for 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
	Serial.begin(9600);// initialize the LCD
	lcd.init();
lcd.backlight();
lcd.setCursor(1,0);
lcd.print("Solar Tracker!");

  }

void loop()
{
  int Battery = analogRead(A0);
  int Solarcell = analogRead(A1);
  Serial.println(Battery*10.0/1024);
  lcd.setCursor(0,1);
  lcd.print(Battery*10.0/1024.0); lcd.print("V ");
  lcd.print(Solarcell*15.0/1024.0); lcd.print("V");
   Serial.print("Baterry:"); Serial.print(Battery*10.0/1024.0); Serial.print(",");
   Serial.print("Solar:"); Serial.println(Solarcell*15.0/1024.0);
}
