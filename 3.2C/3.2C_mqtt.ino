#include <WiFiNINA.h>
#include <PubSubClient.h>

// Wi-Fi credentials
char ssid[] = "Shruti";
char password[] = "shruti@04";

// MQTT settings
const char* mqtt_server = "broker.emqx.io";
const int mqtt_port = 1883;

// MQTT topics
const char* waveTopic = "Shruti/ES/Wave";
const char* patTopic = "Shruti/ES/Pat";

// Ultrasonic sensor pins
const int trigPin = 7;
const int echoPin = 6;

// LED pins
const int bathroomLED = 4; // Red LED
const int hallwayLED = 5;  // Yellow LED

// My name
const char* studentName = "Shruti";

// MQTT and Wi-Fi objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Variables
long duration;  // Stores the ultrasonic echo duration
float distance;  // Stores the calculated distance in cm

// Prevents repeated gesture detection
unsigned long lastGestureTime = 0;  // Stores the time of the last detected gesture
const unsigned long gestureCooldown = 2000;  // Sets a 2-second delay between gestures

// SETUP
void setup() {

  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(bathroomLED, OUTPUT);
  pinMode(hallwayLED, OUTPUT);

  digitalWrite(bathroomLED, LOW);
  digitalWrite(hallwayLED, LOW);

  connectWiFi();

  // Set MQTT broker and callback function
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqttCallback);

  connectMQTT();

  Serial.println("System ready.");
}

// MAIN LOOP
void loop() {

  // Reconnect if MQTT connection is lost
  if (!mqttClient.connected()) {
    connectMQTT();
  }

  // Check for incoming MQTT messages
  mqttClient.loop();

  // Read ultrasonic distance
  distance = getDistance();

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Detect gestures
  if (millis() - lastGestureTime > gestureCooldown) {

    // WAVE
    if (distance >= 20 && distance <= 40) {

      Serial.println("WAVE detected!");

      // Publish name to the wave topic
      mqttClient.publish(waveTopic, studentName);

      lastGestureTime = millis();

      delay(500);
    }

    // PAT
    else if (distance > 0 && distance <= 10) {

      Serial.println("PAT detected!");

      // Publish name to the pat topic
      mqttClient.publish(patTopic, studentName);

      lastGestureTime = millis();

      delay(500);
    }
  }

  delay(100);
}

// CONNECT TO WI-FI
void connectWiFi() {

  Serial.print("Connecting to Wi-Fi");

  // Keep trying until Wi-Fi connects
  while (WiFi.begin(ssid, password) != WL_CONNECTED) {

    Serial.print(".");
    delay(2000);
  }

  Serial.println();
  Serial.println("Wi-Fi connected!");

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// CONNECT TO MQTT BROKER
void connectMQTT() {

  // Keep trying until MQTT connects
  while (!mqttClient.connected()) {

    Serial.print("Connecting to MQTT broker...");

    // Generate a unique client ID
    String clientID = "ArduinoNano33IoT-";
    clientID += String(random(0xffff), HEX);

    if (mqttClient.connect(clientID.c_str())) {

      Serial.println("connected!");

      // Subscribe to both topics
      mqttClient.subscribe(waveTopic);
      mqttClient.subscribe(patTopic);

      Serial.println("Subscribed to:");
      Serial.println(waveTopic);
      Serial.println(patTopic);
    }

    else {

      Serial.print("Failed, MQTT state = ");
      Serial.println(mqttClient.state());

      delay(5000);
    }
  }
}

// MQTT CALLBACK
void mqttCallback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  // Convert received payload into a String
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Message received on topic: ");
  Serial.println(topic);

  Serial.print("Message: ");
  Serial.println(message);

  // Ignore messages from other students
  if (message != studentName) {
    Serial.println("Message is from another student. Ignoring.");
    return;
  }

  // ES/Wave received from me
  if (strcmp(topic, waveTopic) == 0) {

    Serial.println("My WAVE message received.");
    Serial.println("Turning ON both lights.");

    // Turn both LEDs ON
    digitalWrite(bathroomLED, HIGH);
    digitalWrite(hallwayLED, HIGH);
  }

  // ES/Pat received from me
  else if (strcmp(topic, patTopic) == 0) {

    Serial.println("My PAT message received.");
    Serial.println("Turning OFF both lights.");

    // Turn both LEDs OFF
    digitalWrite(bathroomLED, LOW);
    digitalWrite(hallwayLED, LOW);
  }
}

// GET DISTANCE FROM ULTRASONIC SENSOR
float getDistance() {

  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  // Measure echo response time
  duration = pulseIn(echoPin, HIGH, 30000);

  // Return -1 if no echo is received
  if (duration == 0) {
    return -1;
  }

  // Calculate distance in centimetres
  float distanceCM = duration * 0.0343 / 2;

  return distanceCM;
}