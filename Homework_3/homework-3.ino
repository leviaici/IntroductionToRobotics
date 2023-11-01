const int pinA = 10;
const int pinB = 9;
const int pinC = 13;
const int pinD = 7;
const int pinE = 6;
const int pinF = 12;
const int pinG = 8;
const int dpPin = 5;

const int A = 0;
const int B = 1;
const int C = 2;
const int D = 3;
const int E = 4;
const int F = 5;
const int G = 6;
const int DP = 7;
const int NA = -1;

const int segmentPins[] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, dpPin};
int segmentStates[] = {LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW};

const unsigned int segmentLength = 8;
unsigned int index = 7;

const unsigned int pinSW = 2;
const unsigned int pinX = A0;
const unsigned int pinY = A1;


byte swState = LOW;
int xValue = 0;
int yValue = 0;

const unsigned int directions = 4;

const unsigned int joystickSensitivity = 100;
bool joystickMoved = false;

unsigned int minimumThreshold = 350;
unsigned int maximumThreshold = 650;

const unsigned int UP = 0;
const unsigned int DOWN = 1;
const unsigned int LEFT = 2;
const unsigned int RIGHT = 3;

int BAUD = 9600;

const unsigned int swDebounceDelay = 50;
const int blinkInterval = 500;

unsigned long currentTime;
unsigned long startTime = millis();

bool blinkState = false;

bool buttonPressed = false;
unsigned long buttonPressedTime = 0;

const unsigned long longPressDuration = 1000;

// UP, DOWN, LEFT, RIGHT
int path[segmentLength][directions] = {
  NA, G, F, B, // a
  A, G, F, NA, // b
  G, D, E, DP, // c
  G, NA, E, C,  // d
  G, D, NA, C, // e
  A, G, NA, B, // f
  A, D, NA, NA, // g
  NA, NA, C, NA, // dp
};

enum State {
  IDLE,
  MOVING_UP,
  MOVING_DOWN,
  MOVING_LEFT,
  MOVING_RIGHT
};

State gameState = IDLE;

void setup() {
  int index;
  for (index = 0; index < segmentLength; index++) {
    pinMode(segmentPins[index], OUTPUT);
  }
  attachInterrupt(digitalPinToInterrupt(pinSW), changeState, CHANGE);
  pinMode(dpPin, OUTPUT);
  pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(BAUD);
}

void loop() {
  currentTime = millis();

  switch (gameState) {
    case IDLE:
      xValue = analogRead(pinX);
      yValue = analogRead(pinY);

      if (currentTime - startTime >= blinkInterval) {
        blinkState = !blinkState;
        digitalWrite(segmentPins[index], blinkState);
        startTime = currentTime;
      }

      if (xValue + joystickSensitivity < minimumThreshold) {
        verifyJoystickMotion(LEFT);
      } else if (xValue - joystickSensitivity > maximumThreshold) {
        verifyJoystickMotion(RIGHT);
      } else if (yValue + joystickSensitivity < minimumThreshold) {
        verifyJoystickMotion(DOWN);
      } else if (yValue - joystickSensitivity > maximumThreshold) {
        verifyJoystickMotion(UP);
      } else joystickMoved = false;

      break;
    case MOVING_UP:
      updateCase(UP);

      break;
    case MOVING_DOWN:
      updateCase(DOWN);

      break;
    case MOVING_LEFT:
      updateCase(LEFT);

      break;
    case MOVING_RIGHT:
      updateCase(RIGHT);
    
      break;
  }
}

void verifyJoystickMotion(int state) {
  if (!joystickMoved)
    if (path[index][state] != NA) {
      if (state == UP)
        gameState = MOVING_UP;
      else if (state == DOWN)
        gameState = MOVING_DOWN;
      else if (state == LEFT)
        gameState = MOVING_LEFT;
      else if (state == RIGHT)
        gameState = MOVING_RIGHT;
      joystickMoved = true;
    }
}

void resetGame() {
  for (int i = 0; i < segmentLength; i++) {
        segmentStates[i] = LOW;
        digitalWrite(segmentPins[i], segmentStates[i]);
      }
      index = 7;
      gameState = IDLE;
}

void updateCase(int nextState) {
  digitalWrite(segmentPins[index], segmentStates[index]);
  index = path[index][nextState];
  startTime = currentTime;
  gameState = IDLE;
}

void changeState() {
  currentTime = millis();
  swState = digitalRead(pinSW);

  if (!swState) {
    if (!buttonPressed) {
      buttonPressedTime = millis();
      buttonPressed = true;
    } else if (currentTime - buttonPressedTime >= longPressDuration) {
      resetGame();
      buttonPressed = false;
    }
  } else {
    if (buttonPressed && currentTime - buttonPressedTime >= swDebounceDelay) {
      segmentStates[index] = !segmentStates[index];
    }
    buttonPressed = false;
  }
}