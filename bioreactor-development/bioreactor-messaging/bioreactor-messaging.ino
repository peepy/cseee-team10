#include "src/SubsystemMessage/SubsystemMessage.h"
#include "src/MessageHandler/MessageHandler.h"

MessageHandler messenger;

const char* MESSAGE_TYPE_LABELS[] = {"Command", "Report"};
const char* MESSAGE_TARGET_LABELS[] = {"Heating Subsystem", "Stirring Subsystem", "pH Subsystem"};
const char* MESSAGE_FLAG_LABELS[] = {"Arm / disarm", "Set report interval", "Set poll interval", "Set target value", "Report sensor value", "Report actuator state", "Report error with code"};

Message report;
Message received;

int flag = 0;
double target = 0;

void setup() 
{

  Serial.begin(9600);

  delay(1000);
  
  Serial.println("----- RUNNING -----");

  report = Message();
  buildMessage(&report, STIRRING_SYSTEM, SET_TARGET_VALUE, 1022);
  
  messenger.putOutgoing(&report);


  received = Message();
  
}

void loop() {

  if (messenger.isIncomingAvailable())
  {
    messenger.getIncoming(&received);

    Serial.println("\n...");
    
    Serial.print("Target: ");
    Serial.println(MESSAGE_TARGET_LABELS[received.contents._header._target]);

    Serial.print("Flag: ");
    Serial.println(MESSAGE_FLAG_LABELS[received.contents._header._flag]);

    Serial.print("Value: ");
    if (received.contents._header._target == STIRRING_SYSTEM)
    {
      Serial.println(received.contents._body._intParameter);
    }
    else
    {
      Serial.println(received.contents._body._doubleParameter);
    }

    Serial.println("...");



    buildMessage(&report, PH_SYSTEM, (flag++) % 7, target++);
    messenger.putOutgoing(&report);

    delay(500);
    /**
    Serial.println("Byte debug");
    for (int i = 0; i < sizeof(received.buffer); i++)
    {
      Serial.print(received.buffer[i] - '\0');
      Serial.print(" | ");
      Serial.println(report.buffer[i] - '\0');
    }
    **/
    
  }

  if (messenger.isOutgoingAvailable())
  {

    while(messenger.isOutgoingAvailable())
    {

      char next = messenger.getOutgoing();
      messenger.putIncoming(next);
      
    }
    
    
  }
  
}
