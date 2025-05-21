#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // You can try 0x3F if 0x27 doesn't work

void setup() {
  lcd.begin(16, 2);      // Add the correct parameters
  lcd.backlight();       // Turn on backlight
  lcd.print("Hello, world!2");
}

void loop() {
  // nothing for now
}
