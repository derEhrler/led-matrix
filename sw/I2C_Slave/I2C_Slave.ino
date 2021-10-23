#include <Wire.h>
#include <IRremote.h>


#define IR_PIN          2


byte IRValue[2] = {0, 0};


void setup() {
    Serial.begin(9600);
    
    pinMode(IR_PIN, INPUT);
    IrReceiver.begin(IR_PIN);

    Wire.begin(5); // Start the I2C Bus as Slave on address 5
    Wire.onRequest(request_event_handler);

}


void request_event_handler() {
    Wire.write(IRValue, 2);
    IRValue[0] = 0;
    IRValue[1] = 0;
}


void loop() {
    if (IrReceiver.decode()) {

        if (IrReceiver.decodedIRData.command == 0x0) {
            
        }
        
        else if (IrReceiver.decodedIRData.command == IRValue[0]) {
                IRValue[1]++;
        }

        else {
            IRValue[0] = IrReceiver.decodedIRData.command;
            IRValue[1] = 1;
        }

     
        Serial.println(IrReceiver.decodedIRData.command);
        IrReceiver.resume();
    }
}
