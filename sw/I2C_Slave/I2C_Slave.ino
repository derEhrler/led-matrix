#include <Wire.h>
#include <IRremote.h>


#define IR_PIN          2
#define WAKEUP_PIN      6

#define NO_TX_OCCURED   3000


byte IRValue[2] = {0, 0};
bool MasterSleep = false;
bool SlavePrepareWakeup = false;
unsigned long txTime = 0;


void setup() {

    Serial.begin(9600);

    pinMode(WAKEUP_PIN, OUTPUT);
    digitalWrite(WAKEUP_PIN, HIGH);

    pinMode(IR_PIN, INPUT);
    IrReceiver.begin(IR_PIN);

    Wire.begin(5); // Start the I2C Bus as Slave on address 5

    Wire.onRequest(request_event_handler);

    Wire.onReceive(receive_event_handler);

    txTime = millis();
}


void request_event_handler() {

    Wire.write(IRValue, 2);
    IRValue[0] = 0;
    IRValue[1] = 0;
    txTime = millis();
}


void receive_event_handler() {

    if (Wire.available()) {
        MasterSleep = Wire.read();
        MasterSleep = true;
    }
    IRValue[0] = 0;
    IRValue[1] = 0;
    Serial.println("Prepare Wakeup");
}


void decodeIR() {

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
        IrReceiver.resume();
    }
}


void wakeupMaster() {

    Serial.println("Wakeup");
    MasterSleep = false;
    SlavePrepareWakeup = false;
    digitalWrite(WAKEUP_PIN, LOW);
    delay(50);
    digitalWrite(WAKEUP_PIN, HIGH);
    txTime = millis();

}


void checkStates() {
    unsigned long helpTime;
    long lastTx;

    if (SlavePrepareWakeup && MasterSleep) {
        if (IRValue[0] == 0x45) {
            Serial.println("Wakeup Master");
            wakeupMaster();
        }
        return;
    }

    if (!SlavePrepareWakeup && MasterSleep) {
        IRValue[0] = 0;
        IRValue[1] = 0;
        SlavePrepareWakeup = true;
        Serial.println("Slave Prepare Wakeup");
        delay(1000);
        return;
    }

    helpTime = millis() - txTime;
    lastTx = long(helpTime);
    if (!SlavePrepareWakeup && !MasterSleep) {
        if (lastTx  >= NO_TX_OCCURED) {
            Serial.println("Force Wakeup");
            wakeupMaster();
            return;
        }
    }
}


void loop() {

    decodeIR();

    checkStates();
}
