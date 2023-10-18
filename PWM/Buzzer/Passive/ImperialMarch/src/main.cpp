#include <Arduino.h>
#include <map>

int freq = 2000;
int channel = 0;
int resolution = 8;
int buzzerPin = 5;  

std::map<std::string, int> frequencyMap = {
    {"F4", 349},
    {"A4", 440},  
    {"C5", 523},
    {"E5", 659},
    {"F6", 1396}
};

void playTone(std::string note, int duration) {
  ledcWriteTone(channel, frequencyMap[note]);
  delay(duration);
  ledcWriteTone(channel, 0);
}
 
void setup() {
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(buzzerPin, channel);
}

void loop() {
    playTone("A4", 500); 
    playTone("A4", 500);
    playTone("A4", 500);
    playTone("F4", 350);
    playTone("C5", 150);
    
    playTone("A4", 500);
    playTone("F4", 350);
    playTone("C5", 150);
    playTone("A4", 650);

    playTone("E5", 500);
    playTone("E5", 500);
    playTone("E5", 500);
    playTone("F6", 350);
    playTone("C5", 150);

    delay(500);
}