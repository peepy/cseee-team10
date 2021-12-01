#pragma once

/*
  LinkSlave.h - Arduino library for I2C communication with ESP32
  Alex Clarke, Nov 21
*/

#ifndef LinkSlave_h
#define LinkSlave_h

#include "../MessageHandler/MessageHandler.h"

class LinkSlave
{
  public:
    LinkSlave();

    void boot(int sdaPin, int sclPin);
    void poll();
    void flush();

    void sendReport(Message* report);

    void onRequest();
    void onReceive(int recCount);

    void onCommand(void (*)());
    Message latestCommand;

  private:
    int _sdaPin;
    int _sclPin;
    int _slaveAddr;
    MessageHandler _messageHandler;

    void send(uint8_t data);
    bool receive(int recCount);

    void (*_onMessageReady)();

};

extern LinkSlave Link;
extern void Init_Link();

#endif