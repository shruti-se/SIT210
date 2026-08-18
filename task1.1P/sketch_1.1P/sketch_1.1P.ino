// Pin connected to the push button
const int button = 2;

// declared pins connected to porch & hallway LEDs
const int ledPorch = 11;    // Green LED
const int ledHallway = 12;  // Red LED

// Keeps track of whether the lighting timer is currently running
bool timerRunning = false;

unsigned long startTime = 0; // to store the time at which the button/switch was pressed 

void setup() // for configuration of the components 
{

  pinMode(button, INPUT_PULLUP); // configuring the push button as an input using the internal pull-up resistor

  // Configuring to set the mode of the pins used by the LEDs are set as 'OUTPUT'
  pinMode(ledPorch, OUTPUT);
  pinMode(ledHallway, OUTPUT);

  // Make sure both LEDs are OFF when the system starts by keeping their intialised state 'LOW'
  digitalWrite(ledPorch, LOW);
  digitalWrite(ledHallway, LOW);
}

// Start the lighting timer when button is pressed
void startLightingTimer() {
  timerRunning = true;
  startTime = millis();
  digitalWrite(ledPorch, HIGH);
  digitalWrite(ledHallway, HIGH);
}

// Update the timer and control LEDs accordingly
void updateLightingTimer() {
  unsigned long elapsedTime = millis() - startTime;

  // After 30 seconds, porch light will turn 'OFF' 
  if (elapsedTime >= 30000) {
    digitalWrite(ledPorch, LOW);
  }

  // After 60 seconds, hallway light will trun 'OFF' and stop timer
  if (elapsedTime >= 60000) {
    digitalWrite(ledHallway, LOW);
    timerRunning = false;
  }
}

void loop() {
  // Check button press ('LOW' means pressed because of INPUT_PULLUP)
  if (!timerRunning && digitalRead(button) == LOW) {
    startLightingTimer();
  }

  // If timer is running, keep updating lights
  if (timerRunning) {
    updateLightingTimer();
  }
}
