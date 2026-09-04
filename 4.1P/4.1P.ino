#include <Wire.h>
#include <BH1750.h> 

//PIN DEFINITIONS 
const int PIR_PIN = 2;
const int SWITCH_PIN = 3;

const int LED1_PIN = 5;   // Porch light (green LED)
const int LED2_PIN = 6;   // Hallway light (Yellow LED)

//BH1750
BH1750 lightSensor;

//this value, the system considers the environment dark.
const float DARK_THRESHOLD = 50.0;

//INTERRUPT FLAGS
volatile bool motionDetected = false;
volatile bool switchActivated = false;


//PIR INTERRUPT
void motionISR()
{
    motionDetected = true;
}


//SWITCH INTERRUPT
void switchISR()
{
    switchActivated = true;
}


//SETUP
void setup()
{
    Serial.begin(115200);

    // LED configuration
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    // PIR configuration
    pinMode(PIR_PIN, INPUT);

    // Slider switch configuration
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // Start I2C
    Wire.begin();

    // Start BH1750
    if (lightSensor.begin())
    {
        Serial.println("BH1750 initialised successfully.");
    }
    else
    {
        Serial.println("Error: BH1750 not detected.");
    }

    // Attach hardware interrupts
    attachInterrupt(
        digitalPinToInterrupt(PIR_PIN),
        motionISR,
        RISING
    );

    attachInterrupt(
        digitalPinToInterrupt(SWITCH_PIN),
        switchISR,
        FALLING
    );

    Serial.println("System Initialised.");
    Serial.println("Waiting for events...");
}


//MAIN LOOP
void loop()
{
    // Read light intensity from BH1750
    float lux = lightSensor.readLightLevel();

    Serial.print("Light intensity: ");
    Serial.print(lux);
    Serial.println(" lux");

    // MOTION INTERRUPT
    if (motionDetected)
    {
        noInterrupts();
        motionDetected = false;
        interrupts();

        Serial.println("Motion detected!");

        // Only automatically activate lights if it is dark
        if (lux < DARK_THRESHOLD)
        {
            Serial.println("Environment is dark.");
            Serial.println("Automatic lighting activated.");

            turnLightsOn();

            delay(3000);

            turnLightsOff();
        }
        else
        {
            Serial.println("Environment is bright.");
            Serial.println("Lights remain OFF.");
        }

        Serial.println();
    }

    // SLIDER SWITCH INTERRUPT
    if (switchActivated)
    {
        noInterrupts();
        switchActivated = false;
        interrupts();

        Serial.println("Slider switch activated!");
        Serial.println("Manual lighting activated.");

        turnLightsOn();

        delay(3000);

        turnLightsOff();

        Serial.println();
    }

    delay(500);
}


//LIGHT FUNCTIONS
void turnLightsOn()
{
    digitalWrite(LED1_PIN, HIGH);
    digitalWrite(LED2_PIN, HIGH);

    Serial.println("Porch light ON");
    Serial.println("Hallway light ON");
}


void turnLightsOff()
{
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    Serial.println("Porch light OFF");
    Serial.println("Hallway light OFF");
}