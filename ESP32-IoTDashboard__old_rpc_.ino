// "InitWifi()" Adapted from https://github.com/martinius96/ESP32-eduroam to work at UCL

#include <WiFi.h>
#include "esp_wpa2.h"   // wpa2 for connection to enterprise networks (eduroam)
#include <ThingsBoard.h>
#include <ArduinoJSON.h>
#include <PubSubClient.h>

// Credentials
#define TOKEN               "0exNgzuDnwPw7iaUumqi"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

#define EAP_IDENTITY "zcabdgj@ucl.ac.uk"                
#define EAP_PASSWORD "dycdiz-nyMbi1-jopjyz" // Secure

WiFiClient espClient;
ThingsBoard tb(espClient);

PubSubClient rpcClient(espClient);

int status = WL_IDLE_STATUS;

const char* essid = "eduroam";

void setup();
void loop();
void InitWifi();

int getCurrentTemperature();
int getCurrentPH();

int getCurrentRPM();

int currentTemperature = 0;
int currentPH = 0;
int currentRPM = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Made it here!");
  InitWifi();
  rpcClient.setServer(THINGSBOARD_SERVER, 1883);
  rpcClient.setCallback(on_rpc_message);
}

/**

// Processes function for RPC call "example_set_temperature"
// RPC_Data is a JSON variant, that can be queried using operator[]
// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
RPC_Response processTemperatureChange(const RPC_Data &data){
  Serial.println("Received the set temperature RPC method");

  // Process data

  float example_temperature = data["temp"];

  Serial.print("Example temperature: ");
  Serial.println(example_temperature);

  // Just an response example
  return RPC_Response("example_response", 42);
}

// Processes function for RPC call "example_set_switch"
// RPC_Data is a JSON variant, that can be queried using operator[]
// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
RPC_Response processSwitchChange(const RPC_Data &data){
  Serial.println("Received the set switch method");

  // Process data

  bool switch_state = data["switch"];

  Serial.print("Example switch state: ");
  Serial.println(switch_state);

  // Just an response example
  return RPC_Response("example_response", 22.02);
}


const size_t callbacks_size=2;
RPC_Callback callbacks[callbacks_size] = {
  {"example_set_temperature", processTemperatureChange},
  {"example_set_switch", processSwitchChange}  
};
**/

bool subscribed = false;



void loop(){
  // Reconnect to ThingsBoard, if needed
  if (!tb.connected()) {
    subscribed = false;
    
    // Connect to the ThingsBoard 
    Serial.print("Connecting to: "); 
    Serial.print(THINGSBOARD_SERVER); 
    Serial.print(" with token "); 
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    } 
  }

  if (!rpcClient.connected()) {
    rpcReconnect();
  }

/**
  if (!subscribed){
    Serial.println("Subscribing for RPC...");
    // Perform a subscription. ALl consequent data will happen in processTemperatureChange() and processSwitchChange() functions, as denoted by calbacks[] array.
    if (!tb.RPC_Subscribe(callbacks, callbacks_size)){
      Serial.println("Failed to subscribed for RPC");
      return;  
    }  
    Serial.println("Subscribe done");
    subscribed = true;
  }
  Serial.println("Subscribe done");
  **/
  currentTemperature = getCurrentTemperature();
  currentPH = getCurrentPH();
  currentRPM = getCurrentRPM();
  
  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  tb.sendTelemetryFloat("temperature", (float)currentTemperature / 10);
  tb.sendTelemetryFloat("pH", (float)currentPH / 10);
  tb.sendTelemetryInt("stirring", currentRPM);
  // Process messages
  tb.loop();
  rpcClient.loop();
}

void on_rpc_message(const char* topic, byte* payload, unsigned int length)
{
  Serial.println("------ RECEIVED RPC MESSAGE --------");
}

void rpcReconnect() {
  // Loop until we're reconnected
  while (!rpcClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (rpcClient.connect("ESP32", TOKEN, NULL)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(rpcClient.state());
    }
  }
}

//Random Data
int getCurrentTemperature(){
  int temperature = ((float)rand()/(float)(RAND_MAX)) * 60; // Multiple by 10 for fixed point noation
  return temperature * 10;
}

int getCurrentPH(){
  int ph = ((float)rand()/(float)(RAND_MAX)) * 14; // Multiply by 10 for fixed point notation
  return ph * 10;
}

int getCurrentRPM(){
  int rpm = random(2000);
  return rpm;
}

void InitWifi(){
  bool eduroamFound = false;
  delay(10);
  // Scan available WiFi networks until eduroam is seen
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Repeatedly scan until we see eduroam
  //
  while (!eduroamFound) {
    Serial.println("scan start");
    int n = WiFi.scanNetworks(); // WiFi.scanNetworks returns the number of networks found
    Serial.println("scan done");
    
    if (n == 0) {
        Serial.println("no networks found");
    } else {
        Serial.print(n);
        Serial.println(" networks found");
        
        for (int i = 0; i < n; ++i) {
            String ssid = WiFi.SSID(i);
            int    rssi = WiFi.RSSI(i);
          
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(ssid);
            Serial.print(" (");
            Serial.print(rssi);
            Serial.print(")");
            Serial.print((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
            delay(10);
            
            ssid.trim();
            if (ssid == essid) {
              Serial.print(" <==== eduroam found");
              eduroamFound = true;
            }
            Serial.println("");
        }
    }
    Serial.println("");

    // Wait a bit before scanning again
    if (!eduroamFound)
      delay(5000);
  }
  
  // If we come here, we've successfully found eduroam. Try to connect to it.
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(essid);
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // This is where the wpa2 magic happens to allow us to connect to eduroam
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
  esp_wifi_sta_wpa2_ent_enable();
  
  WiFi.begin(essid);       //connect to eduroam
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi is connected to ");
  Serial.println(essid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //print LAN IP
}

void setTargetTemperature(int targetTemp){
  //Send targetTemp to Alex
  
}

void setTargetPH(int targetPH){
  //Send targetPH to Alex
  
}

void setTargetRPM(int targetRPM){
  //Send targetRPM to Alex
  
}
