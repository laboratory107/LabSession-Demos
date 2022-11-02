#include <Arduino.h>
#include <map>

class TouchControl {
private:
    const uint8_t threshold = 20;

    const long touchDelay = 350;
 
    volatile unsigned long lastRegisteredTouchTime = 0;
    volatile unsigned long anyTouchTime = 0;

    long elapsed = 0;
    int touchCounter = 0;
    
    std::map<std::string, std::string> patterns = {
        { "LSLL", "Unlock the door" },
        { "SSL", "Lock the door" }
    };

    std::string lastTouchPattern = "";

    static void IRAM_ATTR touchInterrupt();
    char getTouchPattern(int);

    Stream *serial;
    bool debugEnabled = false;
public:
    void enableDebug(Stream &stream = Serial);
    void setupTouchInterrupt();
    std::string handleTouch();
};