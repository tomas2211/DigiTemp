#include "Arduino.h"

#include <SPI.h>
#include "RFM69.h"
#include "RFM69_ATC.h"

// RFM69 defines
#define NODEID        0   // keep UNIQUE for each node on same network
#define NETWORKID     100  // keep IDENTICAL on all nodes that talk to each other
#define RECVID     1    // "central" node
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey"


RFM69_ATC radio(10, 2, false);
byte ackCount = 0;
uint32_t packetCount = 0;


uint8_t buff[RF69_MAX_DATA_LEN];
int buff_idx = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println("START - TX module");

    Serial.println("INIT radio");
    radio.initialize(FREQUENCY, NODEID, NETWORKID);
    radio.encrypt(ENCRYPTKEY);
}

void sendBuffer() {
    if (buff_idx > 0) {
        Serial.print("Sending[");
        Serial.print(buff_idx);
        Serial.print("]: ");
        for (byte i = 0; i < buff_idx; i++)
            Serial.print((char) buff[i]);

        if (radio.sendWithRetry(RECVID, buff, buff_idx))
            Serial.println(" --- ok!");
        else Serial.println(" --- no ACK");
    }
    buff_idx = 0;
}

void loop() {
    //process any serial input
    while (Serial.available() > 0) {
        char input = Serial.read();
        Serial.write(input);
        if (input == '\n') {
            sendBuffer();
        } else {
            buff[buff_idx] = input;
            buff_idx++;
            if (buff_idx >= RF69_MAX_DATA_LEN) {
                sendBuffer();
            }
        }
    }

    if (radio.receiveDone()) {
        Serial.print("Recv - packets:");
        Serial.print(++packetCount);
        Serial.print(' ');
        Serial.print('id: ');
        Serial.print(radio.SENDERID, DEC);
        Serial.print(" [RX_RSSI: ");
        Serial.print(radio.RSSI);
        Serial.print("]");

        for (byte i = 0; i < radio.DATALEN; i++)
            Serial.print((char) radio.DATA[i]);


        if (radio.ACKRequested()) {
            byte theNodeID = radio.SENDERID;
            radio.sendACK();
            Serial.print(" --- ACK sent.");

            // When a node requests an ACK, respond to the ACK
            // and also send a packet requesting an ACK (every 3rd one only)
            // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
//            if (ackCount++ % 3 == 0) {
//                Serial.print(" Pinging node ");
//                Serial.print(theNodeID);
//                Serial.print(" - ACK...");
//                delay(3); //need this when sending right after reception .. ?
//                if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
//                    Serial.print("ok!");
//                else Serial.print("nothing");
//            }
        }
        Serial.println();
    }

}