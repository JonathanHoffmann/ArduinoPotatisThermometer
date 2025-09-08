#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS18B20.h>

float warningtemphigh = 12;
float warningtemplow = 5;
bool dbg = true;

// Switch pins
const int LOW_SWITCH_PIN  = 12;
const int HIGH_SWITCH_PIN = 13;
bool checkHigh, checkLow;

//LCD Pin1 VSS GND
//LCD Pin2 VDD +5V
//LCD Pin3 VDA A4
//LCD Pin4 VCK A5
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Thermometers on pins 2–5
DS18B20 ds2(2);
DS18B20 ds4(4);
DS18B20 ds6(6);
DS18B20 ds8(8);

const int RELAY_PIN = 10; // Pin connected to relay IN

float temp2 = 0, temp4 = 0, temp6 = 0, temp8 = 0;
char buf[6];

void setup()
{
  if (dbg)
  {
    Serial.begin(9600);
  }
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Starting...");
  delay(200);
  lcd.clear();

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Start with relay off

    // Switches use internal pull-ups
  pinMode(LOW_SWITCH_PIN, INPUT_PULLUP);
  pinMode(HIGH_SWITCH_PIN, INPUT_PULLUP);
}

void loop()
{
  lcd.clear();

    // Read switch states
  bool checkLow  = (digitalRead(LOW_SWITCH_PIN)  == LOW); // switch ON = active
  bool checkHigh = (digitalRead(HIGH_SWITCH_PIN) == LOW);
  
  // Read sensors
  if (ds2.selectNext()) temp2 = ds2.getTempC();
  if (ds4.selectNext()) temp4 = ds4.getTempC();
  if (ds6.selectNext()) temp6 = ds6.getTempC();
  if (ds8.selectNext()) temp8 = ds8.getTempC();

  // Display temps with °C
  lcd.setCursor(0, 0);
  dtostrf(temp2, 4, 1, buf);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(9, 0);
  dtostrf(temp4, 4, 1, buf);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(0, 1);
  dtostrf(temp6, 4, 1, buf);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print("C");

  lcd.setCursor(9, 1);
  dtostrf(temp8, 4, 1, buf);
  lcd.print(buf);
  lcd.print((char)223);
  lcd.print("C");

  // Relay logic

  if (checkLow && 
      (temp2 < warningtemplow || temp4 < warningtemplow || temp6 < warningtemplow || temp8 < warningtemplow))
  {
    turnOn = true;
    if (dbg) Serial.println("Turning Lamp on (LOW temp)");
  }

  if (checkHigh &&
      (temp2 > warningtemphigh || temp4 > warningtemphigh || temp6 > warningtemphigh || temp8 > warningtemphigh))
  {
    turnOn = true;
    if (dbg) Serial.println("Turning Lamp on (HIGH temp)");
  }

  if (turnOn)
  {
    digitalWrite(RELAY_PIN, HIGH); // Relay ON
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW); // Relay OFF
    if (dbg) Serial.println("Turning Lamp off");
  }

  delay(1000);
}
