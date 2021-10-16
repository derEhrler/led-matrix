#include <Wire.h>

byte data[5];

void setup() {
    Serial.begin(9600);
    Wire.begin();

}

void loop() {
    
    Wire.requestFrom(5, 2); // adress, number of bytes
    
    if(Wire.available()) {
        for (int i = 0; i<2; i++) {
            data[i] = Wire.read();    
            Serial.println(data[i]);
        }
    }

    delay(2000);
    Serial.println();

}
