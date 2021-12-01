/*
  Subsystem.h - Arduino library for managing each subsystem
  Alex Clarke, Nov 28
*/

#ifndef Subsystem_h
#define Subsystem_h

class Subsystem
{
  public:

    Subsystem();

    void boot(int pollInterval, int reportInterval);
    void update();

    void setTarget(int parameter);

    void setReportInterval(int msInterval);
    void setPollInterval(int msInterval);

    void setReportCallback(void (*)(int));

  private:

    void poll();
    void report();

    int _value;
    int _target;
    int _reportInterval;
    int _pollInterval;
    unsigned long _lastPolled;
    unsigned long _lastReported;

    void (*_onReportReady)(int);

};

#endif