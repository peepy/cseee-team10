#include <Arduino.h>

#include "../SubsystemMessage/SubsystemMessage.h"
#include "MessageHandler.h"

const uint8_t ESCAPE_KEY = '#';
const uint8_t MESSAGE_DELIMITER = '\n';

MessageHandler :: MessageHandler()
{

	_incomingBuffer.clear();
	_outgoingBuffer.clear();

	_lastMessage = Message();
	_escapeActive = false;
	_messageAvailable = false;

}

bool MessageHandler :: isIncomingAvailable()
{
	return _messageAvailable;
}

bool MessageHandler :: isOutgoingAvailable()
{
	return (!_outgoingBuffer.isEmpty());
}

void MessageHandler :: putIncoming(uint8_t incoming)
{

	bool bufferEmpty = _incomingBuffer.isEmpty();
	bool bufferFull = _incomingBuffer.isFull();

	if (bufferFull)
	{
		Serial.println("--- INCOMING BUFFER FULL! ---");
	}

	if (bufferEmpty)
	{

		// Not strictly necessary but ensures we can recover from a bad packet
		_escapeActive = false;

		if (incoming != MESSAGE_DELIMITER)
		{
			if (incoming == ESCAPE_KEY)
			{
				_escapeActive = true;
			}
			else
			{
				_incomingBuffer.push(incoming);
			}
			
		}
		else
		{
			this->flushMessage();
		}

	}
	else
	{

		if (_escapeActive)
		{
			_escapeActive = false;
			_incomingBuffer.push(incoming);
		}
		else
		{

			if (incoming == ESCAPE_KEY)
			{
				_escapeActive = true;
			}
			else if (incoming == MESSAGE_DELIMITER)
			{
				this->flushMessage();
			}
			else
			{
				_incomingBuffer.push(incoming);
			}

		}
	}

}

void MessageHandler :: flushOutgoing()
{
	_outgoingBuffer.push(MESSAGE_DELIMITER);
}

void MessageHandler :: putOutgoing(Message* message)
{

	bool bufferFull = _outgoingBuffer.isFull();
	if (bufferFull)
	{
		Serial.println("--- OUTGOING BUFFER FULL! ---");
		_outgoingBuffer.clear();
		_outgoingBuffer.push(MESSAGE_DELIMITER);
	}

	for(int i = 0; i < sizeof(message->buffer); i++) {

		uint8_t next = message->buffer[i];

		// Escape forbidden characters in message
		if (next == ESCAPE_KEY || next == MESSAGE_DELIMITER)
		{
			_outgoingBuffer.push(ESCAPE_KEY);
		}

		_outgoingBuffer.push(next);

	}

	_outgoingBuffer.push(MESSAGE_DELIMITER);

}

void MessageHandler :: getIncoming(Message* message)
{
	
	int count = sizeof(message->buffer);
	for (int i = 0; i < count; i++)
	{
		message->buffer[i] = _lastMessage.buffer[i];
	}

	// Clear last message
	memset(_lastMessage.buffer, 0, sizeof(_lastMessage.buffer));
	_messageAvailable = false;

}

uint8_t MessageHandler :: getOutgoing()
{

	uint8_t outgoing = '\0';
	if (!_outgoingBuffer.pop(outgoing))
	{
		outgoing = MESSAGE_DELIMITER;
	}

	return outgoing;
}

void MessageHandler :: flushMessage()
{
	
	int count = _incomingBuffer.size();
	if (count == sizeof(_lastMessage.buffer))
	{

		_messageAvailable = true;
		memset(_lastMessage.buffer, 0, sizeof(_lastMessage.buffer));

		uint8_t temp;
		for (int i = 0; i < count; i++)
		{
			temp = 0;
			_incomingBuffer.pop(temp);
			_lastMessage.buffer[i] = temp;

		}

	}
	else if (count == 0)
	{

	}
	else
	{
		Serial.println("--- DUMPED BAD PACKET ---");

		uint8_t temp[_incomingBuffer.size()];
		for (int i = 0; i < count; i++)
		{
			uint8_t tempChar;
			_incomingBuffer.pop(tempChar);
			temp[i] = tempChar;

		}

		debugBuffer(temp);
	}
	
	_incomingBuffer.clear();

}