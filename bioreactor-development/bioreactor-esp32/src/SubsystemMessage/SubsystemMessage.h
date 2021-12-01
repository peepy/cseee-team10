/*
  SubsystemMessage.h - Arduino library for passing structured messages between boards
  Alex Clarke, Nov 28
*/

#ifndef SubsystemMessage_h
#define SubsystemMessage_h

enum MessageTarget 
{
  HEATING_SYSTEM,
  STIRRING_SYSTEM,
  PH_SYSTEM
};

enum MessageFlag 
{
  SET_ARMED,
  SET_REPORT_INTERVAL,
  SET_POLL_INTERVAL,
  SET_TARGET_VALUE,

  RET_SENSOR_VALUE,
  RET_ACTUATOR_STATE,
  RET_ERROR_CODE
};

typedef struct __attribute__((__packed__)) messageHeader_t
{
  uint8_t _target;
  uint8_t _flag;
} messageHeader_t;

typedef union __attribute__((__packed__)) messageBody_t
{
  uint32_t _intParameter;
  bool _boolParameter;
  float _floatParameter;
} messageBody_t;

typedef struct __attribute__((__packed__)) messageContents_t
{
  messageHeader_t _header;
  messageBody_t _body;
} messageContents_t;

typedef union  __attribute__((__packed__)) message_t
{
  messageContents_t contents;
  uint8_t buffer[sizeof(contents)];
} Message;

void clearMessage(Message* message);

void buildMessage(Message* message, uint8_t _target, uint8_t _flag, float _parameter);
void buildMessage(Message* message, uint8_t _target, uint8_t _flag, int _parameter);
void buildMessage(Message* message, uint8_t _target, uint8_t _flag, bool _parameter);

void debugBuffer(uint8_t buffer[]);
void debugMessage(Message* message);
void debugMessageValue(Message* message);

#endif