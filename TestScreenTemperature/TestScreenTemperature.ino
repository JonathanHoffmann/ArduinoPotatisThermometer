#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DS18B20.h>

//LCD Pin1 VSS GND
//LCD Pin2 VDD +5V
//LCD Pin3 VDA A4
//LCD Pin4 VCK A5
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Thermometers on pins 2–5
DS18B20 ds2(2);
DS18B20 ds3(3);
DS18B20 ds4(4);
DS18B20 ds5(5);

float temp2 = 0, temp3 = 0, temp4 = 0, temp5 = 0;
char buf[6];

void setup() {
  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Starting...");
  delay(200);
  lcd.clear();
}

void loop() {
  // Read sensors
  if (ds2.selectNext()) temp2 = ds2.getTempC();
  if (ds3.selectNext()) temp3 = ds3.getTempC();
  if (ds4.selectNext()) temp4 = ds4.getTempC();
  if (ds5.selectNext()) temp5 = ds5.getTempC();

  // Display temps with °C
  lcd.setCursor(0, 0);   // Top-left
  dtostrf(temp2, 4, 1, buf);
  lcd.print(buf); lcd.print((char)223); lcd.print("C");

  lcd.setCursor(9, 0);   // Top-right
  dtostrf(temp3, 4, 1, buf);
  lcd.print(buf); lcd.print((char)223); lcd.print("C");

  lcd.setCursor(0, 1);   // Bottom-left
  dtostrf(temp4, 4, 1, buf);
  lcd.print(buf); lcd.print((char)223); lcd.print("C");

  lcd.setCursor(9, 1);   // Bottom-right
  dtostrf(temp5, 4, 1, buf);
  lcd.print(buf); lcd.print((char)223); lcd.print("C");

  delay(1000);
}
