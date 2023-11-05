const int firstFloorButtonPin = 2;
const int secondFloorButtonPin = 10;
const int thirdFloorButtonPin = 4;

const int buzzerPin = 5;
const int firstFloorLedPin = 6;
const int secondFloorLedPin = 7;
const int thirdFloorLedPin = 8;
const int betweenFloorsLedPin = 9;

unsigned int buzzerTone = 0;
unsigned int buzzerOnDuration = 1000;
unsigned int buzzerPeriodDuration = 0;
unsigned long buzzerLastPeriodStart = 0;

byte firstFloorButtonState = LOW;
byte secondFloorButtonState = LOW;
byte thirdFloorButtonState = LOW;

volatile int floorState = 1;
volatile int betweenFloors = 0;
volatile unsigned long elevatorStartTime = 0;
unsigned long lastFloorLedChange = 0;
unsigned long doorOpenStartTime = 0;

unsigned int debounceDelay = 100;
unsigned int debounce1Delay = 0;
unsigned int debounce2Delay = 0;
unsigned int debounce3Delay = 0;

unsigned int timeInterval = 3000;
unsigned int timeIntermediaryFloorLed = 500;
unsigned int timeToStartIntermediaryFloorLed = 3000;

unsigned long delayOnStart = 500;
unsigned long buttonPressTime = 0;
bool buttonPressed = false;

const unsigned int blinkInterval = 500;
int blinkingLedState = LOW;
bool doorOpenSoundPlayed = false;

bool toBlinkIntermediaryFloor = false;
bool reallyLightingUp = false;

int BAUD = 9600;

// states of the elevator
enum ElevatorState {
  IDLE,
  BETWEEN_FLOORS,
  ARRIVING,
  DOOR_OPENING,
  START_PROGRAM
};

ElevatorState elevatorState = START_PROGRAM;

void setup() {
  pinMode(firstFloorButtonPin, INPUT_PULLUP);
  pinMode(secondFloorButtonPin, INPUT_PULLUP);
  pinMode(thirdFloorButtonPin, INPUT_PULLUP);

  pinMode(firstFloorLedPin, OUTPUT);
  pinMode(secondFloorLedPin, OUTPUT);
  pinMode(thirdFloorLedPin, OUTPUT);
  pinMode(betweenFloorsLedPin, OUTPUT);

  pinMode(buzzerPin, OUTPUT);

  Serial.begin(BAUD);
}

void loop() {
  unsigned long currentTime = millis();

  switch (elevatorState) {
    case IDLE:
      firstFloorButtonState = digitalRead(firstFloorButtonPin);
      secondFloorButtonState = digitalRead(secondFloorButtonPin);
      thirdFloorButtonState = digitalRead(thirdFloorButtonPin);

      if (!betweenFloors) { // verifying if the elevator is not between the floors
        if (!firstFloorButtonState && floorState != 1 && currentTime - debounce1Delay > debounceDelay) { // verifying if the elevator's first floor button is pressed and if it isn't already at the first floor + debounceDelay
          if (floorState == 3) { 
            timeInterval = 7000; 
            toBlinkIntermediaryFloor = true; // it will turn on the 2nd floor LED intermediary
          } else timeInterval = 3000;
          floorState = 1;
          buttonPressTime = currentTime; // so that it can blink the elevator LED
          buttonPressed = true; // so we can make sure it won't be pressed multiple times during the motion of the elevator
        } else if (!secondFloorButtonState && floorState != 2 && currentTime - debounce2Delay > debounceDelay) {
          floorState = 2;
          buttonPressTime = currentTime;
          timeInterval = 3000;
          buttonPressed = true;
        } else if (!thirdFloorButtonState && floorState != 3 && currentTime - debounce3Delay > debounceDelay) {
          if (floorState == 1) {
            timeInterval = 7000;
            toBlinkIntermediaryFloor = true;
          } else timeInterval = 3000;
          floorState = 3;
          buttonPressTime = currentTime;
          buttonPressed = true;
        }
      }

      if (buttonPressed && (currentTime - buttonPressTime >= delayOnStart)) { // checking if we can put the elevator in motion
        buttonPressed = false;
        elevatorStartTime = currentTime;
        elevatorState = BETWEEN_FLOORS;
      }
      break;

    case BETWEEN_FLOORS:
      if (currentTime - elevatorStartTime >= timeInterval) {
        elevatorState = ARRIVING;
        doorOpenStartTime = currentTime;
        doorOpenSoundPlayed = false;
      }

      if (currentTime - lastFloorLedChange >= blinkInterval) { // blinking the LED and buzzer tone for elevator motion
        blinkingLedState = !blinkingLedState;
        lastFloorLedChange = currentTime;
        buzzerTone = 300;
        tone(buzzerPin, buzzerTone);
      }

      if (toBlinkIntermediaryFloor) { // if the 2nd floor LED should be lit up when moving from 1st to 3rd or 3rd to 1st floor 
        if (currentTime - elevatorStartTime == timeToStartIntermediaryFloorLed) { // if it is the right time to turn on
          unsigned long startIntermediaryFloorLed = millis();
          digitalWrite(secondFloorLedPin, HIGH);
          reallyLightingUp = true;
        } else if (currentTime - elevatorStartTime > timeToStartIntermediaryFloorLed + timeIntermediaryFloorLed) { // else, we turn it off
          digitalWrite(secondFloorLedPin, LOW);
          toBlinkIntermediaryFloor = false;
          reallyLightingUp = false;
        }
      }

      digitalWrite(betweenFloorsLedPin, blinkingLedState);
      digitalWrite(firstFloorLedPin, LOW);
      digitalWrite(secondFloorLedPin, reallyLightingUp ? HIGH : LOW);
      digitalWrite(thirdFloorLedPin, LOW);
      break;

    case ARRIVING: // the elevator is arriving so it can play the sound and continue the LED blinking
      if (currentTime - lastFloorLedChange >= blinkInterval) {
        blinkingLedState = !blinkingLedState;
        lastFloorLedChange = currentTime;
      }

      if (!doorOpenSoundPlayed) {
        buzzerTone = 700;
        tone(buzzerPin, buzzerTone);
        doorOpenSoundPlayed = true;
      }

      if (currentTime - doorOpenStartTime >= buzzerOnDuration) {
        noTone(buzzerPin);
        doorOpenSoundPlayed = false;
        elevatorState = DOOR_OPENING;
      }

      digitalWrite(betweenFloorsLedPin, blinkingLedState);
      break;

    case DOOR_OPENING: // the elevator is opening its doors so it can play the sound for the door opening
      if (!doorOpenSoundPlayed) {
        buzzerTone = 500;
        tone(buzzerPin, buzzerTone);
        doorOpenSoundPlayed = true;
      }

      if (currentTime - doorOpenStartTime >= buzzerOnDuration) {
        noTone(buzzerPin);
        elevatorState = IDLE;
      }

      digitalWrite(firstFloorLedPin, floorState == 1 ? HIGH : LOW);
      digitalWrite(secondFloorLedPin, floorState == 2 ? HIGH : LOW);
      digitalWrite(thirdFloorLedPin, floorState == 3 ? HIGH : LOW);
      digitalWrite(betweenFloorsLedPin, HIGH);
      break;

    case START_PROGRAM: // the first state of the program (the 1st floor LED is lit up)
      digitalWrite(firstFloorLedPin, floorState == 1 ? HIGH : LOW);
      digitalWrite(secondFloorLedPin, floorState == 2 ? HIGH : LOW);
      digitalWrite(thirdFloorLedPin, floorState == 3 ? HIGH : LOW);
      digitalWrite(betweenFloorsLedPin, HIGH);
      elevatorState = IDLE;
      break;
  }
}
