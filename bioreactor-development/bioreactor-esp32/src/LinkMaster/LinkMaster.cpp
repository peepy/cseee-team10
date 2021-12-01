#include "Arduino.h"
#include <Wire.h>

#include "../MessageHandler/MessageHandler.h"

#include "LinkMaster.h"

#define TRANSMISSION_BYTE_COUNT 1

#define MASTER_ADDRESS 0
#define SLAVE_ADDRESS 1

LinkMaster :: LinkMaster()
{

	MessageHandler _messageHandler = MessageHandler();

}

// =========== API FUNCTIONALITY ===========

void LinkMaster :: boot(int sdaPin, int sclPin)
{

	_sdaPin = sdaPin;
	_sclPin = sclPin;
	_slaveAddr = SLAVE_ADDRESS;

	Wire.begin(_sdaPin, _sclPin); // Different to the default library function

}

void LinkMaster :: poll()
{

	if (_messageHandler.isOutgoingAvailable())
	{
		uint8_t outgoing = _messageHandler.getOutgoing();
		this->send(outgoing);
	}

	Wire.requestFrom(_slaveAddr, TRANSMISSION_BYTE_COUNT);
	bool received = this->receive();

	if (_messageHandler.isIncomingAvailable())
	{

		_messageHandler.getIncoming(&latestReport);
		
		if (_onMessageReady)
		{
		  _onMessageReady();
		}


	}
	

}

void LinkMaster :: flush()
{

	_messageHandler.flushOutgoing();

}

void LinkMaster :: sendCommand(Message* command)
{

	_messageHandler.putOutgoing(command);

}

void LinkMaster :: onReport(void (*function)())
{

   _onMessageReady = function;

}

// =========== LOW LEVEL FUNCTIONALITY ===========

void LinkMaster :: send(uint8_t data)
{

  Wire.beginTransmission(_slaveAddr);
  Wire.write(data);
  Wire.endTransmission();

}

bool LinkMaster :: receive()
{

	bool received = Wire.available();

	while (Wire.available()) 
	{

		uint8_t incoming = Wire.read();
		_messageHandler.putIncoming(incoming);
		
	}
	
	return received;
}

LinkMaster Link = LinkMaster();