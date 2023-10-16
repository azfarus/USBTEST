/**
 * Simple CDC device connect with putty to use it
 * author: chegewara
 *Serial0 - used only for logging
 *Serial01 - can be used to control GPS or any other device, may be replaced withSerial0
 */
#include "cdcusb.h"
#if CFG_TUD_CDC
CDCusb USBSerial;

class MyUSBCallbacks : public CDCCallbacks {
    void onCodingChange(cdc_line_coding_t const* p_line_coding)
    {
        int bitrate = USBSerial.getBitrate();
       Serial0.printf("new bitrate: %d\n", bitrate);
    }

    bool onConnect(bool dtr, bool rts)
    {
       Serial0.printf("connection state changed, dtr: %d, rts: %d\n", dtr, rts);
        return true;  // allow to persist reset, when Arduino IDE is trying to enter bootloader mode
    }

    void onData()
    {
        int len = USBSerial.available();
       Serial0.printf("\nnew data, len %d\n", len);
        uint8_t buf[len] = {};
        USBSerial.read(buf, len);
       Serial0.write(buf, len);
    }

    void onWantedChar(char c)
    {
       Serial0.printf("wanted char: %c\n", c);
    }
};


void setup()
{
   Serial0.begin(115200);
    USBSerial.setCallbacks(new MyUSBCallbacks());
    USBSerial.setWantedChar('x');

    if (!USBSerial.begin())
       Serial0.println("Failed to start CDC USB stack");

}

void loop()
{
    while (Serial0.available())
    {
        int len =Serial0.available();
        char buf1[len];
       Serial0.read(buf1, len);
        int a = USBSerial.write((uint8_t*)buf1, len);
    }
}

#endif