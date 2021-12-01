#include "src/LinkMaster/LinkMaster.h"
#include "src/SubsystemMessage/SubsystemMessage.h"

#define LINK_SDA 21
#define LINK_SCL 22

int lastValue = 0;

void setup()
{

  Serial.begin(9600);

  Link.boot(LINK_SDA, LINK_SCL);

  delay(2000);

  Serial.println("ESP32 running.");
  Link.flush();

  Link.onReport([] () {

    int value = Link.latestReport.contents._body._intParameter;
    Serial.println(value);

  });

}

void loop()
{

  Link.poll();

}
