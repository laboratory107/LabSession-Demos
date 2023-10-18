#include <Arduino.h>
#include <map>

int freq = 2000;
int channel = 0;
int resolution = 8;
int buzzerPin = 5;  

std::map<std::string, int> frequencyMap = {
    {"F3", 174},
    {"A3", 220},  
    {"C4", 261}
};

void playTone(std::string note, int duration) {
  Serial.println(frequencyMap[note]);
  ledcWriteTone(channel, frequencyMap[note]);
  delay(duration);
  ledcWriteTone(channel, 0);
}
 
void setup() {
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzerPin, channel);
}

void loop() {
    playTone("A3", 500); 
    playTone("A3", 500);
    playTone("A3", 500);
    playTone("F3", 350);
    playTone("C4", 150);
    
    playTone("A3", 500);
    playTone("F3", 350);
    playTone("C4", 150);
    playTone("A3", 650);

    delay(500);
}