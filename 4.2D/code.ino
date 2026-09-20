#define BLYNK_TEMPLATE_ID "TMPL3J6jQ_0yR"
#define BLYNK_TEMPLATE_NAME "LindaHomeLights"
#define BLYNK_AUTH_TOKEN "MSZxjXMC_brJKTxPAjlFmZxp3GOOSNh4"

#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>

char ssid[] = "Shruti";
char pass[] = "shruti@04";

// LED pins
const int LIVING_ROOM_LED = 2;
const int BATHROOM_LED = 3;
const int CLOSET_LED = 4;

// Function to control a light using the room name
void controlLight(String room, int state)
{
  if (room == "living room")
  {
    digitalWrite(LIVING_ROOM_LED, state);
  }
  else if (room == "bathroom")
  {
    digitalWrite(BATHROOM_LED, state);
  }
  else if (room == "closet")
  {
    digitalWrite(CLOSET_LED, state);
  }
}

// Living Room - V0
BLYNK_WRITE(V0)
{
  int state = param.asInt();
  controlLight("living room", state);
}

// Bathroom - V1
BLYNK_WRITE(V1)
{
  int state = param.asInt();
  controlLight("bathroom", state);
}

// Closet - V2
BLYNK_WRITE(V2)
{
  int state = param.asInt();
  controlLight("closet", state);
}

void setup()
{
  Serial.begin(9600);
  delay(1000);

  // Set LED pins as outputs
  pinMode(LIVING_ROOM_LED, OUTPUT);
  pinMode(BATHROOM_LED, OUTPUT);
  pinMode(CLOSET_LED, OUTPUT);

  // Start with all LEDs OFF
  digitalWrite(LIVING_ROOM_LED, LOW);
  digitalWrite(BATHROOM_LED, LOW);
  digitalWrite(CLOSET_LED, LOW);

  // Connect Nano to Wi-Fi and Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop()
{ 
  Blynk.run(); // Keep the Blynk connection active and process incoming commands
}