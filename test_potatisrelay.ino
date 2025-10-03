const int RELAY_PIN = 10; // Pin connected to relay IN

void setup()
{
    Serial.begin(9600);
  pinMode(RELAY_PIN, OUTPUT);
}

void loop()
{
  Serial.println("Turning Lamp on");
  digitalWrite(RELAY_PIN, HIGH); // Relay ON
  delay(1000);
  Serial.println("Turning Lamp off");
  digitalWrite(RELAY_PIN, LOW); // Relay ON
  delay(1000);
}
