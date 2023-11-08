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

const unsigned long longPressDuration = 3000;

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

// states of the cursor
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

      if (currentTime - startTime >= blinkInterval) { // blinking state of the cursor
        blinkState = !blinkState;
        digitalWrite(segmentPins[index], blinkState);
        startTime = currentTime;
      }

      if (xValue + joystickSensitivity < minimumThreshold) { // verifying the joystick motion for x value
        verifyJoystickMotion(LEFT);                          // and the fact that the joystick could go in the desired direction
      } else if (xValue - joystickSensitivity > maximumThreshold) {
        verifyJoystickMotion(RIGHT);
      } else if (yValue + joystickSensitivity < minimumThreshold) { // verifying the joystick motion for y value
        verifyJoystickMotion(DOWN);                                 // and the fact that the joystick could go in the desired direction
      } else if (yValue - joystickSensitivity > maximumThreshold) {
        verifyJoystickMotion(UP);
      } else joystickMoved = false;

      break;
    case MOVING_UP: // the cursor is going up
      updateCase(UP);

      break;
    case MOVING_DOWN: // the cursor is going down
      updateCase(DOWN);

      break;
    case MOVING_LEFT: // the cursor is going left
      updateCase(LEFT);

      break;
    case MOVING_RIGHT: // the cursor is going right
      updateCase(RIGHT);
    
      break;
  }
}

void verifyJoystickMotion(int state) {
  if (!joystickMoved) // verifying if it's not true so it won't move more than 1 place at a time
    if (path[index][state] != NA) { // verifying that it can go to that path
      if (state == UP)
        gameState = MOVING_UP;
      else if (state == DOWN)
        gameState = MOVING_DOWN;
      else if (state == LEFT)
        gameState = MOVING_LEFT;
      else if (state == RIGHT)
        gameState = MOVING_RIGHT;
      joystickMoved = true; // setting it true so it won't move more than 1 place at a time
    }
}

void resetGame() {
  for (int i = 0; i < segmentLength; i++) {
        segmentStates[i] = LOW;
        digitalWrite(segmentPins[i], segmentStates[i]); // resetting all 7-seg LEDs
      }
      index = 7; // putting back the cursor on the dp
      gameState = IDLE; // resetting the state
}

void updateCase(int nextState) {
  digitalWrite(segmentPins[index], segmentStates[index]); // we are either lighting up or turning off the LED we are currently on and preparing to leave
  index = path[index][nextState]; // modifying the cursor to the next one
  startTime = currentTime; // modifying startTime for the blinking
  gameState = IDLE;
}

void changeState() {
  swState = digitalRead(pinSW);

  if (!swState) {
    if (!buttonPressed) { // verifying if the button was pressed
      buttonPressedTime = currentTime; // starting recording the time when the button was first pressed
      buttonPressed = true; // saving the fact that it was pressed
    } else if (currentTime - buttonPressedTime >= longPressDuration) { // verifying if it's enough pressed so it can reset and that the button was pressed
      resetGame();
      buttonPressed = false; // resetting the value
    }
  } else {
    if (buttonPressed && currentTime - buttonPressedTime >= swDebounceDelay) { // if it's not a long press, it will just !state of the LED
      segmentStates[index] = !segmentStates[index];
    }
    buttonPressed = false; // resetting the value for the next press
  }
}
