// Include libraries required for Wi-Fi, MQTT communication, I2C and BH1750 sensor
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <BH1750.h>

// Wi-Fi Credentials
#define WIFI_SSID "Shruti"
#define WIFI_PASSWORD "shruti@04"

// HiveMQ MQTT Credentials
#define MQTT_SERVER "fe3b6df982a94b38b2250a5b11ca50dd.s1.eu.hivemq.cloud"
#define MQTT_PORT 8883

// MQTT authentication credentials 
#define MQTT_USER "task3.1P"
#define MQTT_PASSWORD "shruti@hivemq"

// MQTT Topic
#define MQTT_TOPIC "sensor/light"

// BH1750 Light Sensor
// Creating a BH1750 sensor object for measuring light intensity in lux
BH1750 lightMeter;

// Network and MQTT objects
WiFiSSLClient wifiSSLClient; // WiFiSSLClient provides a secure connection to the HiveMQ Cloud broker
PubSubClient mqttClient(wifiSSLClient); // PubSubClient manages MQTT communication using the secure Wi-Fi connection

// Connect to Wi-Fi
// Connects the Arduino to the configured Wi-Fi network.
// The function keeps attempting to connect until a connection is established.
void connectWiFi()
{
    Serial.print("Connecting to Wi-Fi");

    while (WiFi.status() != WL_CONNECTED)
    {
        // Start a Wi-Fi connection using the provided credentials
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

        // Wait before checking the connection again
        delay(2000);
        Serial.print(".");
    }

    // Display a confirmation message once connected
    Serial.println();
    Serial.println("Wi-Fi connected!");
    
    // Display the IP address assigned to the Arduino
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}

// Connect to HiveMQ
// Connects the Arduino to the HiveMQ MQTT broker. 
// A unique client ID is generated so the broker can identify this device.
void connectMQTT()
{
    while (!mqttClient.connected())
    {
        Serial.println("Connecting to MQTT...");

        // Creating a unique client ID
        String clientID = "ArduinoNanoIoT-";
        clientID += String(random(0, 9999));

        // Attempt to connect to the MQTT broker using the username and password
        if (mqttClient.connect(
                clientID.c_str(),
                MQTT_USER,
                MQTT_PASSWORD))
        {
            // Connection was successful
            Serial.println("Connected to MQTT Broker!");
        }
        else
        {
            // Display the MQTT error state if the connection fails
            Serial.print("MQTT connection failed. State: ");
            Serial.println(mqttClient.state());

            // Wait before attempting to reconnect
            Serial.println("Retrying in 5 seconds...");
            delay(5000);
        }
    }
}

// Setup
void setup()
// Runs once when the Arduino starts.
{
    Serial.begin(115200); // Start serial communication for displaying sensor and connection information

    delay(2000);

    // Display the project title in the Serial Monitor
    Serial.println();
    Serial.println("================================");
    Serial.println("Terrarium Light Monitoring");
    Serial.println("================================");

    // Initialise I2C communication used by the BH1750 sensor
    Wire.begin();

    // Start BH1750
    if (lightMeter.begin())
    {
        Serial.println("BH1750 started successfully!");
    }
    else
    {
        // Stop the program if the sensor cannot be detected
        Serial.println("ERROR: BH1750 not detected!");
        
        while (1)
        {
            delay(1000);
        }
    }
    // Connect to Wi-Fi
    connectWiFi();

    // Configure MQTT server
    mqttClient.setServer(MQTT_SERVER, MQTT_PORT);

    // Connect to MQTT broker
    connectMQTT();
}

// Main Loop
// Runs continuously after setup().
void loop()
{
    // Check whether the Wi-Fi connection is still active 
    // and reconnect if the connection has been lost
    if (WiFi.status() != WL_CONNECTED)
    {
        connectWiFi();
    }

    // Check whether the MQTT connection is still active 
    // and reconnect if the connection has been lost
    if (!mqttClient.connected())
    {
        connectMQTT();
    }

    // Maintain the MQTT connection and process MQTT communication
    mqttClient.loop();

    // Read the current light intensity from the BH1750 sensor 
    // The value is returned in lux (lx)
    float lightIntensity = lightMeter.readLightLevel();

    // Check whether the sensor returned an invalid reading
    if (lightIntensity < 0)
    {
        Serial.println("Invalid BH1750 reading. Skipping...");
        delay(1000);
        return;
    }

    // Display the measured light intensity in the Serial Monitor
    Serial.print("Light intensity: ");
    Serial.print(lightIntensity);
    Serial.println(" lux");


    // Convert the numerical light reading into a text message 
    // so that it can be sent through MQTT
    String lightMessage = String(lightIntensity, 2);

// Publish the light intensity to the specified MQTT topic
if (mqttClient.publish(MQTT_TOPIC, lightMessage.c_str()))
{
    // Display confirmation when the message is successfully published
    Serial.print("Published to ");
    Serial.print(MQTT_TOPIC);
    Serial.print(": ");
    Serial.println(lightMessage);
}
    else
    {
        // Display an error message if MQTT publishing fails
        Serial.println("MQTT publish failed!");
    }
    // Send reading every 5 seconds
    delay(5000);
}
