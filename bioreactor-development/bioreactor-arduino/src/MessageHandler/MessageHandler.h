/*
  MessageHandler.h - I2C input/output stream library with an escape 
  character for buffering message packets and enclosing them with 
  start/stop markers.

  Alex Clarke, Nov 29
*/

#include "../SubsystemMessage/SubsystemMessage.h"

#ifndef MessageHandler_h
#define MessageHandler_h

#include <RingBuf.h>

const int I2C_COMMS_BUFFER_LENGTH = 100;

class MessageHandler
{
  public:

    MessageHandler();

    bool isIncomingAvailable();
    bool isOutgoingAvailable();

    void putIncoming(uint8_t incoming);
    void getIncoming(Message* message);

    void flushOutgoing();

    void putOutgoing(Message* message);
    uint8_t getOutgoing();


  private:

    Message _lastMessage = Message();
    
    bool _escapeActive;
    bool _messageAvailable;

    RingBuf<uint8_t, I2C_COMMS_BUFFER_LENGTH> _incomingBuffer;
    RingBuf<uint8_t, I2C_COMMS_BUFFER_LENGTH> _outgoingBuffer;

    void flushMessage();



};

#endif