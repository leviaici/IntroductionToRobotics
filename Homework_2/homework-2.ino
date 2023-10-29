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

int BAUD = 9600;

enum ElevatorState {
  IDLE,
  BETWEEN_FLOORS,
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

      if (!betweenFloors) {
        if (!firstFloorButtonState && floorState != 1 && currentTime - debounce1Delay > debounceDelay) {
          if (floorState == 3) {
            timeInterval = 7000;
            toBlinkIntermediaryFloor = true;
          } else timeInterval = 3000;
          floorState = 1;
          buttonPressTime = currentTime;
          buttonPressed = true;
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

      if (buttonPressed && (currentTime - buttonPressTime >= delayOnStart)) {
        buttonPressed = false;
        elevatorStartTime = currentTime;
        elevatorState = BETWEEN_FLOORS;
      }
      break;

    case BETWEEN_FLOORS:
      if (currentTime - elevatorStartTime >= timeInterval) {
        elevatorState = DOOR_OPENING;
        doorOpenStartTime = currentTime;
        doorOpenSoundPlayed = false;
      }

      if (currentTime - lastFloorLedChange >= blinkInterval) {
        blinkingLedState = !blinkingLedState;
        lastFloorLedChange = currentTime;
        buzzerTone = 300;
        tone(buzzerPin, buzzerTone);
      }

      if (toBlinkIntermediaryFloor) {
        if (currentTime - elevatorStartTime == timeToStartIntermediaryFloorLed) {
          unsigned long startIntermediaryFloorLed = millis();
          digitalWrite(secondFloorLedPin, HIGH);
          while (millis() - startIntermediaryFloorLed <= timeIntermediaryFloorLed);
          digitalWrite(secondFloorLedPin, LOW);
          toBlinkIntermediaryFloor = false;
        }
      }

      digitalWrite(betweenFloorsLedPin, blinkingLedState);
      digitalWrite(firstFloorLedPin, LOW);
      digitalWrite(secondFloorLedPin, LOW);
      digitalWrite(thirdFloorLedPin, LOW);
      break;

    case DOOR_OPENING:
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

    case START_PROGRAM:
      digitalWrite(firstFloorLedPin, floorState == 1 ? HIGH : LOW);
      digitalWrite(secondFloorLedPin, floorState == 2 ? HIGH : LOW);
      digitalWrite(thirdFloorLedPin, floorState == 3 ? HIGH : LOW);
      digitalWrite(betweenFloorsLedPin, HIGH);
      elevatorState = IDLE;
      break;
  }
}