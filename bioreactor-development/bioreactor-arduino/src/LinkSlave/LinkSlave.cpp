#include "Arduino.h"
#include <Wire.h>

#include "../SubsystemMessage/SubsystemMessage.h"
#include "../MessageHandler/MessageHandler.h"

#include "LinkSlave.h"

#define SLAVE_ADDRESS 1

LinkSlave :: LinkSlave()
{


}

// =========== API FUNCTIONALITY ===========

void LinkSlave :: boot(int sdaPin, int sclPin)
{

	_sdaPin = sdaPin;
	_sclPin = sclPin;
	_slaveAddr = SLAVE_ADDRESS;

	Wire.begin(_slaveAddr);

}

void LinkSlave :: poll()
{

	
	if (_messageHandler.isIncomingAvailable())
	{

		_messageHandler.getIncoming(&latestCommand);

		if (_onMessageReady)
		{
		  _onMessageReady();
		}

	}




}

void LinkSlave :: flush()
{

	_messageHandler.flushOutgoing();

}

void LinkSlave :: sendReport(Message* report)
{

	_messageHandler.putOutgoing(report);

}

void LinkSlave :: onCommand(void (*function)())
{

   _onMessageReady = function;

}


// =========== LOW LEVEL FUNCTIONALITY ===========

void LinkSlave :: send(uint8_t data)
{

	Wire.write(data);

}

bool LinkSlave :: receive(int recCount)
{

	bool received = Wire.available();

	while (Wire.available()) 
	{

		uint8_t incoming = Wire.read();
		_messageHandler.putIncoming(incoming);
		
	}
	
	return received;
}


void LinkSlave :: onRequest()
{

	uint8_t outgoing = _messageHandler.getOutgoing();
	this->send(outgoing);

}

void LinkSlave :: onReceive(int recCount)
{

	this->receive(recCount);

}


LinkSlave Link = LinkSlave();

// Attach callbacks

void Init_Link()
{

	Wire.onRequest([] () { Link.onRequest(); });
  	Wire.onReceive([] (int recCount) { Link.onReceive(recCount); });

}
