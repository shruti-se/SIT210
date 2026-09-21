#include <Wire.h>
#include <BH1750.h>

// PIN DEFINITIONS
// PIR motion sensor is connected to digital pin 2
const int PIR_PIN = 2;
// Slider switch is connected to digital pin 3
const int SWITCH_PIN = 3;
// LED 1 represents the porch light
const int LED1_PIN = 5;
// LED 2 represents the hallway light
const int LED2_PIN = 6;

// BH1750 LIGHT SENSOR
// Create an object for communicating with the BH1750
// BH1750 measures the surrounding light intensity in lux.
BH1750 lightSensor;


// LIGHT THRESHOLD:
// If the measured light level is below 50 lux,
// the environment is considered dark.
// The system will automatically turn the lights ON
// when motion is detected only if the light level
// is below this threshold.
const float DARK_THRESHOLD = 50.0;

// INTERRUPT FLAGS:
// These variables are used as flags to communicate  between the interrupt service routines (ISRs) and the main loop.
// 'volatile' is important because these variables can be changed at any time by an interrupt.
volatile bool motionDetected = false;
volatile bool switchActivated = false;

// PIR MOTION INTERRUPT SERVICE ROUTINE:
// This function is automatically called when the PIR sensor detects a rising-edge signal.
// The ISR should be kept short and simple.
// Instead of controlling the lights directly here, we simply set a flag.
//
// The main loop will later check this flag and
// perform the required actions.
void motionISR()
{
    motionDetected = true;
}

// SLIDER SWITCH INTERRUPT SERVICE ROUTINE: 
// This function is automatically called when the slider switch produces a falling-edge signal.
//
// Again, the ISR only sets a flag.
// The actual lighting operation is handled inside the main loop.
void switchISR()
{
    switchActivated = true;
}

// SETUP FUNCTION:
// setup() runs only once when the Arduino starts or when it is reset.
void setup()
{
    // Start serial communication so that sensor readings and system messages can be viewed in the Serial Monitor.
    Serial.begin(115200);

    // LED CONFIGURATION:
    // Configure both LED pins as OUTPUT pins because the Arduino needs to control their ON/OFF state.
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);

    // Make sure both lights are OFF when the system starts.
    digitalWrite(LED1_PIN, LOW);
    digitalWrite(LED2_PIN, LOW);

    // PIR SENSOR CONFIGURATION:
    // Configure the PIR sensor pin as an INPUT.
    // The PIR sensor sends a signal when motion is detected.
    pinMode(PIR_PIN, INPUT);

    // SWITCH CONFIGURATION:
    // INPUT_PULLUP enables the Arduino's internal pull-up resistor.
    // This means the switch normally reads HIGH.
    // When the switch is activated, the pin is connected to GND and reads LOW.
    pinMode(SWITCH_PIN, INPUT_PULLUP);

    // I2C INITIALISATION:

    // Start I2C communication.
    // The BH1750 light sensor communicates with the Arduino using the I2C communication protocol.
    Wire.begin();

    // BH1750 INITIALISATION:

    // Try to start communication with the BH1750 sensor.
    if (lightSensor.begin())
    {
        // This message confirms that the sensor was detected.
        Serial.println("BH1750 initialised successfully.");
    }
    else
    {
        // This message appears if the Arduino cannot communicate with the BH1750.
        Serial.println("Error: BH1750 not detected.");
    }

    // HARDWARE INTERRUPT CONFIGURATION:

    // Attach an interrupt to the PIR sensor.

    // digitalPinToInterrupt(PIR_PIN) converts the pin number into the interrupt number supported by the Arduino.
    //
    // motionISR is the function that will be executed when the interrupt occurs.

    // RISING means the interrupt occurs when the signal changes from LOW to HIGH.
    attachInterrupt(
        digitalPinToInterrupt(PIR_PIN),
        motionISR,
        RISING
    );

    // Attach an interrupt to the slider switch.
    // The switch uses INPUT_PULLUP, so its normal state is HIGH. When activated, it changes to LOW.
    //
    // FALLING detects the HIGH-to-LOW transition.
    attachInterrupt(
        digitalPinToInterrupt(SWITCH_PIN),
        switchISR,
        FALLING
    );

    // START-UP MESSAGES:

    Serial.println("System Initialised.");
    Serial.println("Waiting for events...");
}

// MAIN LOOP:

// loop() runs repeatedly after setup() has finished.
// The program continuously:
// 1. Reads the light level.
// 2. Checks for motion.
// 3. Checks for switch activation.
// 4. Controls the lights according to the event.
void loop()
{
    // READ LIGHT INTENSITY:
    // Read the current light intensity from the BH1750.
    // The value is returned in lux.
    float lux = lightSensor.readLightLevel();


    // Display the current light intensity in the
    // Serial Monitor for monitoring and testing.
    Serial.print("Light intensity: " + lux + " lux");

    // MOTION INTERRUPT HANDLING:

    // Check whether the PIR interrupt has detected motion.
    if (motionDetected)
    {
        // Temporarily disable interrupts while changing the shared interrupt flag.
        noInterrupts();

        // Reset the flag so the same event is not processed repeatedly.
        motionDetected = false;

        // Enable interrupts again after updating the flag.
        interrupts();

        // Display a message showing that motion was detected.
        Serial.println("Motion detected!");

        // CHECK WHETHER THE ENVIRONMENT IS DARK:

        // Automatic lighting should only occur when
        // the environment is darker than the threshold.
        if (lux < DARK_THRESHOLD)
        {
            Serial.println("Environment is dark. Automatic lighting activated.");

            // Turn both porch and hallway LEDs ON.
            turnLightsOn();

            // Keep the lights ON for 3 seconds.
            delay(3000);

            // Turn both lights OFF after 3 seconds.
            turnLightsOff();
        }
        else
        {
            // If the environment is bright,
            // motion will not activate the lights.
            Serial.println("Environment is bright. Lights remain OFF.");
        }

        // Print a blank line to make Serial Monitor
        // output easier to read.
        Serial.println();
    }

    // SLIDER SWITCH INTERRUPT HANDLING:

    // Check whether the slider switch interrupt has been triggered.
    if (switchActivated)
    {
        // Temporarily disable interrupts while modifying the shared flag.
        noInterrupts();

        // Reset the flag after detecting the event.
        switchActivated = false;

        // Enable interrupts again.
        interrupts();


        // Display a message showing that the switch was activated.
        Serial.println("Slider switch activated!");
        Serial.println("Manual lighting activated.");


        // Turn both lights ON.
        // Unlike the automatic motion system, the manual switch does not check the light level.
        turnLightsOn();

        // Keep the lights ON for 3 seconds.
        delay(3000);

        // Turn the lights OFF after 3 seconds.
        turnLightsOff();

        // Print a blank line for easier Serial Monitor reading.
        Serial.println();
    }
    // LOOP DELAY:
    // This reduces the frequency of Serial Monitor messages
    // and gives the system a short processing interval.
    delay(500);
}

// LIGHT CONTROL FUNCTIONS:

// This function turns both lights ON.
// Creating a separate function makes the program easier to read and avoids repeating the same digitalWrite()
// commands in multiple places.
void turnLightsOn()
{
    // Turn ON the porch light.
    digitalWrite(LED1_PIN, HIGH);

    // Turn ON the hallway light.
    digitalWrite(LED2_PIN, HIGH);


    // Display the status of each light.
    Serial.println("Porch light ON");
    Serial.println("Hallway light ON");
}

// TURN LIGHTS OFF:
// This function turns both lights OFF.
// It is called after the 3-second lighting period has finished.
void turnLightsOff()
{
    // Turn OFF the porch light.
    digitalWrite(LED1_PIN, LOW);

    // Turn OFF the hallway light.
    digitalWrite(LED2_PIN, LOW);

    // Display the status of each light.
    Serial.println("Porch light OFF");
    Serial.println("Hallway light OFF");
}