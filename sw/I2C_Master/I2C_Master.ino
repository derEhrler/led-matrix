#include <Wire.h>
#include <avr/sleep.h>

#define WAKEUP_PIN      2

#define SLEEP_COOLDOWN  1000;

byte data[2];
bool gotoSleep = false;
unsigned long rxTime = 0;
unsigned long wakeupTime = 0;



void setup() {

    Serial.begin(9600);
    Wire.begin();

    pinMode(WAKEUP_PIN, INPUT_PULLUP);

}

void requestFromSlave() {

    rxTime = millis();
    Wire.requestFrom(5, 2); // adress, number of bytes
    if (Wire.available()) {
        for (int i = 0; i < 2; i++) {
            data[i] = Wire.read();
            Serial.println(data[i]);
        }
    }
}


void sendToSlave() {

    Wire.beginTransmission(5); 
    Wire.write(1);       
    Wire.endTransmission();
}


void checkIrValue() {
    unsigned long helpTime;
    long sleepTimeout;

    helpTime = millis() - wakeupTime;
    sleepTimeout = long(helpTime);
    
    if (data[0] == 0x45 && sleepTimeout > SLEEP_COOLDOWN) {
        gotoSleep = true;
        return;
    }
}


void isrWakeup() {

    sleep_disable();
    detachInterrupt(0);
}


void enterSleep() {

    attachInterrupt(0, isrWakeup, LOW);
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    Serial.println("Sleep Mode");
    delay(50);
    sleep_enable();
    sleep_mode();
    Serial.println("Wakeup");
    gotoSleep = false;
    data[0] = 0;
    wakeupTime = millis();
}


void loop() {

    requestFromSlave();

    checkIrValue();

    if (gotoSleep) {
        sendToSlave();
        enterSleep();
    }

    while (millis() < (rxTime + 200)) {}
    Serial.println();
}
