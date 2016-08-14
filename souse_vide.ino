#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define ONE_WIRE_BUS 2
#define RELAY_PIN 3

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

double savedTemperature = 60;
double currentTemperature = 0;
unsigned long timerStart = 0;

void setup (void) {
  pinMode(RELAY_PIN, OUTPUT);
  lcd.begin(16, 2);
  sensors.begin();
}

void loop (void) { 
  checkButtons();
  currentTemperature = getTempearture();
  checkRelay();
  lcdPrint();
}

String zerofill (unsigned long dateInt) {
  String dateString = String(dateInt);
  if ( dateString.length() < 2 ) {
    dateString = "0" + dateString;
  }
  return dateString;
}

void lcdPrint () {
  unsigned long timeSeconds = (millis() - timerStart) / 1000;
  lcd.setCursor(0, 0);
  lcd.print(savedTemperature);
  lcd.print("   ");
  lcd.print( zerofill( timeSeconds/3600 ) + ":" + zerofill( (timeSeconds % 3600)/60 ) + ":" + zerofill( (timeSeconds % 3600) % 60 ) );
  lcd.setCursor(0, 1);
  lcd.print(currentTemperature);
  lcd.print("      ");
  
}

double getTempearture () {
  sensors.requestTemperatures();
  return sensors.getTempCByIndex(0);
}

void checkRelay () {
  if ( currentTemperature < savedTemperature ) {
    digitalWrite(RELAY_PIN, LOW);
  } else 
    digitalWrite(RELAY_PIN, HIGH);
}

void checkButtons () {
  int buttonPressed = analogRead(A0);
  if ( checkButton(buttonPressed, 1, "right", 110) )
    return;
  else if ( checkButton(buttonPressed, 2, "up", 310) )
    return;
  else if ( checkButton(buttonPressed, 3, "down", 450) )
    return;
  else if ( checkButton(buttonPressed, 4, "left", 600) )
    return;
  else 
    checkButton(buttonPressed, 5, "select", 800);
}

boolean checkButton (int analogResult, int buttonNum, String buttonName, int analogData) {
  if ( analogResult < analogData  ) {
    if ( buttonNum == 2 )
      if ( savedTemperature < 99 )
        savedTemperature++;
    if ( buttonNum == 3 )
      if ( savedTemperature > 30 )
        savedTemperature--;
    if ( buttonNum == 5 )
      timerStart = millis();
    return true;
  }

  return false;
}

