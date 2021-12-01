#include "Arduino.h"

#include "SubsystemMessage.h"

#define TEMPERATURE_PRECISION_DP 1
#define PH_PRECISION_DP 1

const char* MESSAGE_TARGET_LABELS[] = {"Heating Subsystem", "Stirring Subsystem", "pH Subsystem"};
const char* MESSAGE_FLAG_LABELS[] = {"Arm / disarm", "Set report interval", "Set poll interval", "Set target value", "Report sensor value", "Report actuator state", "Report error with code"};

void clearMessage(Message* message)
{
	memset(message->buffer, 0, sizeof(message->buffer));
}

void buildMessage(Message* message, uint8_t _target, uint8_t _flag, float _parameter)
{

	/**
	// Temp and pH should expect converting, force round otherwise.
	int _intParameter = round(_parameter);

	if (_flag == SET_TARGET_VALUE || _flag == RET_SENSOR_VALUE)
	{

		if (_target == PH_SYSTEM)
		{
			_intParameter = round(_parameter * pow(10, PH_PRECISION_DP)); 
		}
		else if (_target == HEATING_SYSTEM)
		{
			_intParameter = round(_parameter * pow(10, TEMPERATURE_PRECISION_DP)); 
		}

	}
	**/

	clearMessage(message);

	message->contents._header._target = _target;
	message->contents._header._flag = _flag;
	message->contents._body._floatParameter = _parameter;

}

void buildMessage(Message* message, uint8_t _target, uint8_t _flag, int _parameter)
{

	clearMessage(message);

	message->contents._header._target = _target;
	message->contents._header._flag = _flag;
	message->contents._body._intParameter = _parameter;

}

void buildMessage(Message* message, uint8_t _target, uint8_t _flag, bool _parameter)
{

	clearMessage(message);

	message->contents._header._target = _target;
	message->contents._header._flag = _flag;
	message->contents._body._boolParameter = _parameter;

}

void debugBuffer(uint8_t buffer[])
{

  Serial.println("--- START Byte Dump ---");
  Serial.print("| ");
  for (int i = 0; i < sizeof(buffer); i++)
  {
    Serial.print(buffer[i]);
    Serial.print(" | ");
  }
  Serial.println("\n--- END Byte Dump ---");

}


void debugMessage(Message* message)
{

		Serial.println("\nMessage: ");
    Serial.print("Target: ");
    Serial.println(MESSAGE_TARGET_LABELS[message->contents._header._target]);

    Serial.print("Flag: ");
    Serial.println(MESSAGE_FLAG_LABELS[message->contents._header._flag]);

    Serial.print("Value: ");
    if (message->contents._header._target == STIRRING_SYSTEM)
    {
      Serial.println(message->contents._body._intParameter);
    }
    else
    {
      Serial.println(message->contents._body._floatParameter);
    }
    Serial.println("");

}


void debugMessageValue(Message* message)
{

    Serial.print("Message value: ");

    if (message->contents._header._target == STIRRING_SYSTEM)
    {
    	int parameter = message->contents._body._intParameter;
      Serial.println(parameter);
    }
    else
    {
      Serial.println(message->contents._body._floatParameter);
    }

}