#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

#define ONE_WIRE_BUS 2
#define RELAY_PIN 3
#define RESET_TIMER_PIN 10
#define RESIST_PIN A1

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

double savedTemperature = 60;
double currentTemperature = 0;
unsigned long timerStart = 0;
byte temperatureMax = 99;
byte temperatureMin = 50;

void setup (void) {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(RESET_TIMER_PIN, INPUT_PULLUP);
  lcd.begin(16, 2);
  sensors.begin();
}

void loop (void) {
  checkInputs();
  currentTemperature = getTempearture();
  checkRelay();
  lcdPrint();
}

void checkInputs () {
  savedTemperature = map(analogRead(RESIST_PIN), 0, 1023, temperatureMin, temperatureMax);
  if ( !digitalRead(RESET_TIMER_PIN) )
    timerStart = millis();
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

