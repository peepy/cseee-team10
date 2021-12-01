#include "Arduino.h"

#include "Subsystem.h"

Subsystem :: Subsystem()
{

   _value = -1;
   _target = -1;

   _pollInterval = 1000;
   _reportInterval = 1000;

   _lastPolled = 0;
   _lastReported = 0;

}

void Subsystem :: boot(int pollInterval, int reportInterval)
{
   _pollInterval = pollInterval;
   _reportInterval = reportInterval;
}

void Subsystem :: poll()
{

   // _value = getSensorValue();

   _lastPolled = millis();
}

void Subsystem :: report()
{

   _lastReported = millis();

   if (_onReportReady)
   {
      _onReportReady(_value);
   }

}

void Subsystem :: update()
{


   if (millis() - _lastPolled > _pollInterval)
   {
      this->poll();
   }

   if (millis() - _lastReported > _reportInterval)
   {
      this->report();
   }


}



void Subsystem :: setTarget(int target)
{
   _target = target;
}

void Subsystem :: setReportInterval(int msInterval)
{
   _reportInterval = msInterval;
}

void Subsystem :: setPollInterval(int msInterval)
{
   _pollInterval = msInterval;
}

void Subsystem :: setReportCallback(void (*function)(int))
{

   _onReportReady = function;

}