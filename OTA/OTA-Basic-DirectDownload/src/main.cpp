#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  Serial.println("Hello! I have been applied via OTA");
}

int x = 0;
void loop() {
  delay(500);

  Serial.print("I am at: ");
  Serial.print(x);
  Serial.print("\n");
  x++;
}