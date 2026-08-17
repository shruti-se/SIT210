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

void loop() // for defining the process 
{
  // Lights will start glowing when the button is pressed
  // LOW indicates that the button is pressed because INPUT_PULLUP is used
  if (!timerRunning && digitalRead(button) == LOW) {

    // Indicate that the lighting timer has started
    timerRunning = true;

    // To record the current time when the button is pressed
    startTime = millis();

    // Turn both the porch and hallway lights 'ON'
    digitalWrite(ledPorch, HIGH);
    digitalWrite(ledHallway, HIGH);
  }

  // Continue checking the elapsed time while the timer is running
  if (timerRunning) {

    // To calculate how much time has passed since the button was pressed
    unsigned long elapsedTime = millis() - startTime;

    // After 30 seconds, turn the porch light OFF
    if (elapsedTime >= 30000) {
      digitalWrite(ledPorch, LOW);
    }

    // After 60 seconds, turn the hallway light OFF
    // This also ends the current lighting sequence
    if (elapsedTime >= 60000) {
      digitalWrite(ledHallway, LOW);
      timerRunning = false;
    }
  }
}