#include "TouchControl.h"

bool touched = false;
void IRAM_ATTR TouchControl::touchInterrupt()
{
    touched = true;
}

std::string TouchControl::handleTouch()
{
    if (touched)
    {
        touched = false;
        elapsed = millis() - lastRegisteredTouchTime;
        if (elapsed > touchDelay)
        {
            if (debugEnabled)
            
                serial->println("Touch sensor touched");

            lastRegisteredTouchTime = millis();
            touchCounter++;
        }
         
        anyTouchTime = millis();
    }
    else
    {
        if ( millis() - anyTouchTime > touchDelay && touchCounter > 0) { // If there hasn't been a touch for a good moment
            lastTouchPattern = lastTouchPattern + getTouchPattern(touchCounter); 

            touchCounter = 0;
            serial->println(lastTouchPattern.c_str());
            
            if (lastTouchPattern.length() >= 3) {
                if (patterns.count(lastTouchPattern) != 0)
                    return patterns[lastTouchPattern];
    
                lastTouchPattern = "";
            }
        }
    }

    return "No pattern";
}

void TouchControl::enableDebug(Stream &stream)
{
    serial = &stream;
    debugEnabled = true;
}

void TouchControl::setupTouchInterrupt()
{
    touchAttachInterrupt(T0, touchInterrupt, threshold);
}

char TouchControl::getTouchPattern(int touchCounter) {
    return touchCounter >= 5 ? 'L' : 'S';
}

/*std::string TouchControl::handleTouch()
{
    if (touchRead(T0) < 20)
    {
        touched = false;
        elapsed = millis() - sinceLastTouch;
        if (elapsed > touchDelay)
        {
            if (debugEnabled)
                serial->println("Touch sensor touched");

            if (sinceFirstTouch == 0)
                sinceFirstTouch = millis();

            touchCounter++;   
        }
    }
    else
    {
        if (touchCounter > 0) {
            if (touchCounter > 5)
                serial->println("long");
            else
                serial->println("short");

            lastTouchPattern = lastTouchPattern + getTouchPattern(touchCounter); 

            touchCounter = 0;

            if (lastTouchPattern == unlockPattern)
                return "Unlock the door";
            else if (lastTouchPattern == lockPattern)
                return "Lock the door";
        }
    }

    delay(100);
    return "No pattern";
}*/