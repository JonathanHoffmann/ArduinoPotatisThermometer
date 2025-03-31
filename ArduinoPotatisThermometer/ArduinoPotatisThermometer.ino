#include <OneWire.h>
#include <DallasTemperature.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>

#define ONE_WIRE_BUS 2      // Pin for DS18B20 sensors
#define RELAY_PIN 3         // Pin for relay control
#define BUTTON_PIN 4        // Pin for button
#define POTI_PIN A0         // Analog pin for potentiometer

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if necessary

float minTemp, maxTemp;
bool settingMode = false;
bool settingMin = true;

void readEEPROM() {
    EEPROM.get(0, minTemp);
    EEPROM.get(sizeof(float), maxTemp);
    if (isnan(minTemp) || isnan(maxTemp)) { // Default values if EEPROM is empty
        minTemp = 10.0;
        maxTemp = 30.0;
    }
}

void writeEEPROM() {
    EEPROM.put(0, minTemp);
    EEPROM.put(sizeof(float), maxTemp);
}

void setup() {
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    sensors.begin();
    lcd.begin();
    lcd.backlight();
    readEEPROM();
}

void loop() {
    static bool lastButtonState = HIGH;
    bool buttonState = digitalRead(BUTTON_PIN);
    
    if (buttonState == LOW && lastButtonState == HIGH) { // Button pressed
        settingMode = !settingMode;
        settingMin = true;
    }
    lastButtonState = buttonState;

    if (settingMode) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(settingMin ? "Set Min: " : "Set Max: ");
        float value = map(analogRead(POTI_PIN), 0, 1023, -10, 50);
        lcd.print(value);
        delay(300);
        
        if (buttonState == LOW) {
            if (settingMin) {
                minTemp = value;
                settingMin = false;
            } else {
                maxTemp = value;
                settingMode = false;
                writeEEPROM();
            }
        }
        return;
    }

    sensors.requestTemperatures();
    bool relayOn = false;
    lcd.clear();
    lcd.setCursor(0, 0);
    
    for (int i = 0; i < 4; i++) {
        float temp = sensors.getTempCByIndex(i);
        lcd.setCursor(i * 4, 1);
        lcd.print(temp, 1);
        if (temp < minTemp || temp > maxTemp) {
            relayOn = true;
        }
    }
    
    digitalWrite(RELAY_PIN, relayOn ? HIGH : LOW);
    delay(1000);
}
