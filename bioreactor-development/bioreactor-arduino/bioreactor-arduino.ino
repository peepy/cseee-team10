#include "src/LinkSlave/LinkSlave.h"
#include "src/Subsystem/Subsystem.h"
#include "src/SubsystemMessage/SubsystemMessage.h"

#define LINK_SDA 20
#define LINK_SCL 21

Subsystem heatingSystem = Subsystem();

int count = -2000;

void setup()
{
  
	Serial.begin(9600);

  Init_Link();
	Link.boot(LINK_SDA, LINK_SCL);
  heatingSystem.boot(500, 50);

	delay(2000);

	Serial.println("Arduino running.");
  Link.flush();
  
  Link.onCommand([] () { 

    debugMessage(&Link.latestCommand);
    
  });

  heatingSystem.setReportCallback([] (int value) { 

    Serial.print("Sending: ");
    Serial.println(count);

    Message heatReport;
    buildMessage(&heatReport, STIRRING_SYSTEM, RET_SENSOR_VALUE, count++);

    Link.sendReport(&heatReport);
    
    /**
    Serial.println("Byte debug");
    for (int i = 0; i < sizeof(heatReport.buffer); i++)
    {
      Serial.println(heatReport.buffer[i]);
    }
    **/


  });

}


void loop()
{

  heatingSystem.update();

	Link.poll();

}
