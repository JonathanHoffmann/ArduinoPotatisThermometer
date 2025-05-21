#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS18B20.h>

float warningtemp = 23.5;
bool dbg = true;

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
}

void loop()
{
  lcd.clear();
  
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
  if (temp2 < warningtemp || temp4 < warningtemp || temp6 < warningtemp || temp8 < warningtemp)
  {
    digitalWrite(RELAY_PIN, HIGH); // Turn relay ON
    if (dbg)
    {
      Serial.println("Turning Lamp on");
    }
  }
  else
  {
    digitalWrite(RELAY_PIN, LOW); // Turn relay OFF
    if (dbg)
    {
      Serial.println("Turning Lamp off");
    }
  }

  delay(1000);
}
