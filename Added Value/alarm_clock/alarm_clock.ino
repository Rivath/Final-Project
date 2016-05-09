/*
  Alarm Clock Mat
  John Kessinger
*/

#include <Wire.h>
#include <DS3231.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

DS3231 clock; //sets clock directory
RTCDateTime dt;

int ahours; //the alarm hour value
int amins; //the alarm minute value

int fsrReading_1; //one force sensor variable
int fsrReading_2; //annother force sensor value

//check conditions
int foot_1; 
int foot_2;

//triggers the alarmActive() method
int alarmOn;

void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT); //the speaker
  
  pinMode(11, INPUT); //
  digitalWrite(11, HIGH);
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);
  pinMode(9, INPUT);
  digitalWrite(9, HIGH);
  pinMode(8, INPUT);
  digitalWrite(8, HIGH);

  
  lcd.begin(16, 2);

  // Initialize DS3231
  //Serial.println("Initialize DS3231");;
  clock.begin();

  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);
}

void loop() {
  fsrReading_1 = analogRead(0);
  fsrReading_2 = analogRead(1);

  //Serial.println(fsrReading_1);
  //Serial.println(fsrReading_2);

  if ( fsrReading_1 > 500 && fsrReading_2 > 500) {
    foot_1 = 1;
    foot_2 = 1;
  }

  dt = clock.getDateTime();
  //Serial.println(dt.hour);
  //Serial.println(dt.minute);

  lcd.setCursor(4, 0);
  lcd.print(dt.year); //displays the current year
  lcd.setCursor(9, 0);
  lcd.print(dt.month); //displays the current month

  if (dt.month > 9) {
    lcd.setCursor(12, 0);
    lcd.print(dt.day); //this compensates the spacing on the LCD screen for when the month is 2 digits
  }
  else {
    lcd.setCursor(11, 0);
    lcd.print(dt.day); //displays the current day
  }

  lcd.setCursor(5, 1);
  if (dt.hour < 10) { //adjusts so that the spacing remains consistent 
    lcd.print("0"); 
    lcd.print(dt.hour);
  }
  else {
    lcd.print(dt.hour);
  }
  lcd.print(":");
  lcd.setCursor(8, 1);
  if (dt.minute < 10) { //adjusts so that the spacing remains consistent 
    lcd.print("0");
    lcd.print(dt.minute);
  }
  else {
    lcd.print(dt.minute);
  }
  alarmSet();
  if (dt.hour == ahours && dt.minute == amins) { //if the alarm variables match the time variables the alarm activates
    alarmOn = 1;
  }
  if (alarmOn == 1) {
    alarmActive();
  }
}

void alarmSet() {
  while (digitalRead(8) == LOW) { //the alarm set button
    foot_1 = 0;
    foot_2 = 0;

    lcd.setCursor(10, 1);
    lcd.print("Alarm"); //the alarm is only displayed while the alarm set button is held
    lcd.setCursor(10, 0);
    if (ahours < 10) {
      lcd.print("0");
      lcd.print(ahours);
    }
    else {
      lcd.setCursor(10, 0);
      lcd.print(ahours);
    }
    lcd.print(":");

    if (amins < 10) {
      lcd.print("0");
      lcd.print(amins);
    }
    else {
      lcd.print(amins);
    }
    if (amins > 59) { //compensates so that amins never reaches 60
      ahours++;
      amins = 0;
    }
    if (amins < 0) { //the only way for ahours to be decreased if amins drops below 0
      ahours--;
      amins = 0;
    }
    if (ahours > 23) { //allows hours to wrap around so it never reaches 24
      ahours = 0;
    }
    if (digitalRead(11) == LOW) {
      amins++;
    }
    if (digitalRead(10) == LOW) {
      amins--;
    }
    if (digitalRead(9) == LOW) {
      ahours++;
    }
    delay(100);
    lcd.clear(); //clears the lcd of the alarm code so the clock code can be written on top again
  }
}

void alarmActive() {
  if (foot_1 == 1 && foot_2 == 1) { //if both variables are high the alarm is turned off
    noTone(12);
    alarmOn = 0;
  }
  else {
    tone(12, 1200, 200); //will run until sensors detect both feet
    delay(200);
  }
}

