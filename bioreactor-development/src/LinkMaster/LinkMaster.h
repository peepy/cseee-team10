#pragma once

/*
  LinkMaster.h - ESP32 library for I2C communication with Arduino
  Alex Clarke, Nov 21
*/

#ifndef LinkMaster_h
#define LinkMaster_h

#include "../MessageHandler/MessageHandler.h"

class LinkMaster
{
  public:

    LinkMaster();

    void boot(int sdaPin, int sclPin);
    void poll();
    void flush();

    void sendCommand(Message* command);

    void onReport(void (*)());
    Message latestReport;

  private:
    int _sdaPin;
    int _sclPin;
    int _slaveAddr;
    MessageHandler _messageHandler;

    void send(uint8_t data);
    bool receive();

    void (*_onMessageReady)();

};

extern LinkMaster Link;

#endif