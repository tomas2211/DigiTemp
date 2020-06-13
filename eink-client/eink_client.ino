#include "Arduino.h"
#include "epd2in9/epd2in9.h"
#include "epd2in9/epdpaint.h"
#include "epd2in9/DialogInput_bold_16.h"

//#include <epd2in9.h>
//#include <epdpaint.h>
//#include <DialogInput_bold_16.h>

#include "RFM69/RFM69.h"
//#include <RFM69_ATC.h>

#include <SPI.h>
//#include <epd2in9.h>
//#include <epdpaint.h>
//#include "imagedata.h"

// RFM69 defines
#define NODEID        2    // keep UNIQUE for each node on same network
#define NETWORKID     100  // keep IDENTICAL on all nodes that talk to each other
#define GATEWAYID     1    // "central" node
#define FREQUENCY   RF69_433MHZ
#define ENCRYPTKEY    "sampleEncryptKey"

RFM69 radio(3, 2, false);
byte ackCount = 0;
uint32_t packetCount = 0;


// Eink defines
#define COLORED     0
#define UNCOLORED   1

unsigned char image[1024]; // image buffer
Paint paint(image, 0, 0);    // width should be the multiple of 8
Epd epd;
unsigned long time_start_ms;
unsigned long time_now_s;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);

    radio.initialize(FREQUENCY, NODEID, NETWORKID);
    radio.encrypt(ENCRYPTKEY);

    if (epd.Init(lut_full_update) != 0) {
        Serial.print("e-Paper init failed");
        return;
    }

    /**
     *  there are 2 memory areas embedded in the e-paper display
     *  and once the display is refreshed, the memory area will be auto-toggled,
     *  i.e. the next action of SetFrameMemory will set the other memory area
     *  therefore you have to clear the frame memory twice.
     */
    epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    epd.DisplayFrame();
    epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
    epd.DisplayFrame();

    if (epd.Init(lut_partial_update) != 0) {
        Serial.print("e-Paper init failed");
        return;
    }
//
//    paint.SetRotate(ROTATE_0);
//    paint.SetWidth(128);
//    paint.SetHeight(24);
//
//    /* For simplicity, the arguments are explicit numerical coordinates */
//    paint.Clear(COLORED);
//    paint.DrawStringAt(0, 4, "Hello world!", &Font16, UNCOLORED);
//    epd.SetFrameMemory(paint.GetImage(), 0, 10, paint.GetWidth(), paint.GetHeight());
//
//    paint.Clear(UNCOLORED);
//    paint.DrawStringAt(0, 4, "e-Paper Demo", &Font16, COLORED);
//    epd.SetFrameMemory(paint.GetImage(), 0, 30, paint.GetWidth(), paint.GetHeight());
//
//    paint.SetWidth(64);
//    paint.SetHeight(64);
//
//    paint.Clear(UNCOLORED);
//    paint.DrawRectangle(0, 0, 40, 50, COLORED);
//    paint.DrawLine(0, 0, 40, 50, COLORED);
//    paint.DrawLine(40, 0, 0, 50, COLORED);
//    epd.SetFrameMemory(paint.GetImage(), 16, 60, paint.GetWidth(), paint.GetHeight());
//
//    paint.Clear(UNCOLORED);
//    paint.DrawCircle(32, 32, 30, COLORED);
//    epd.SetFrameMemory(paint.GetImage(), 72, 60, paint.GetWidth(), paint.GetHeight());
//
//    paint.Clear(UNCOLORED);
//    paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
//    epd.SetFrameMemory(paint.GetImage(), 16, 130, paint.GetWidth(), paint.GetHeight());
//
//    paint.Clear(UNCOLORED);
//    paint.DrawFilledCircle(32, 32, 30, COLORED);
//    epd.SetFrameMemory(paint.GetImage(), 72, 130, paint.GetWidth(), paint.GetHeight());
//    epd.DisplayFrame();
//
//    delay(2000);
//
//    if (epd.Init(lut_partial_update) != 0) {
//        Serial.print("e-Paper init failed");
//        return;
//    }
//
//    /**
//     *  there are 2 memory areas embedded in the e-paper display
//     *  and once the display is refreshed, the memory area will be auto-toggled,
//     *  i.e. the next action of SetFrameMemory will set the other memory area
//     *  therefore you have to set the frame memory and refresh the display twice.
//     */
//    epd.SetFrameMemory(IMAGE_DATA);
//    epd.DisplayFrame();
//    epd.SetFrameMemory(IMAGE_DATA);
//    epd.DisplayFrame();



    time_start_ms = millis();
}

void loop() {
    // put your main code here, to run repeatedly:
//    time_now_s = (millis() - time_start_ms) / 1000;
//    char time_string[] = {'0', '0', ':', '0', '0', '\0'};
//    time_string[0] = time_now_s / 60 / 10 + '0';
//    time_string[1] = time_now_s / 60 % 10 + '0';
//    time_string[3] = time_now_s % 60 / 10 + '0';
//    time_string[4] = time_now_s % 60 % 10 + '0';

    if (radio.receiveDone()) {
        Serial.print("#[");
        Serial.print(++packetCount);
        Serial.print(']');
        Serial.print('[');
        Serial.print(radio.SENDERID, DEC);
        Serial.print("] ");

        for (byte i = 0; i < radio.DATALEN; i++)
            Serial.print((char) radio.DATA[i]);
        Serial.print("   [RX_RSSI:");
        Serial.print(radio.RSSI);
        Serial.print("]");

        if (radio.ACKRequested()) {
            byte theNodeID = radio.SENDERID;
            radio.sendACK();
            Serial.print(" - ACK sent.");

            // When a node requests an ACK, respond to the ACK
            // and also send a packet requesting an ACK (every 3rd one only)
            // This way both TX/RX NODE functions are tested on 1 end at the GATEWAY
            if (ackCount++ % 3 == 0) {
                Serial.print(" Pinging node ");
                Serial.print(theNodeID);
                Serial.print(" - ACK...");
                delay(3); //need this when sending right after reception .. ?
                if (radio.sendWithRetry(theNodeID, "ACK TEST", 8, 0))  // 0 = only 1 attempt, no retries
                    Serial.print("ok!");
                else Serial.print("nothing");
            }
        }
        Serial.println();
    }


    paint.SetWidth(20);
    paint.SetHeight(296);
    paint.SetRotate(ROTATE_90);

    paint.Clear(UNCOLORED);
    paint.setTextColor(COLORED);
    paint.setFont(&DialogInput_bold_16);
    paint.setCursor(0, 0);
//    paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);
    paint.print(millis());
//    paint.setCursor(0, 30);
    paint.print("  Hello world!");
//    paint.DrawCharAt(0,4,'a', &Open_Sans_Regular_10, COLORED);
//    paint.DrawCharAt(12,4,'h', &Open_Sans_Regular_10, COLORED);
    epd.SetFrameMemory(paint.GetImage(), 128 - 20, 0, paint.GetWidth(), paint.GetHeight());
    epd.DisplayFrame();

    delay(100);
}