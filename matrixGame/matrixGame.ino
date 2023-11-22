#include <LedControl.h>
#include <EEPROM.h>

// EEPROM MAP
// 0 - 3 - EASY1
// 4 - 7 - EASY2
// 8 - 11 - EASY3
// 12 - 15 - MEDIUM1
// 16 - 19 - MEDIUM2
// 20 - 23 - MEDIUM3
// 24 - 27 - HARD1
// 28 - 31 - HARD2
// 32 - 35 - HARD3
// 36 - 39 - N11
// 40 - 43 - N12
// 44 - 47 - N13
// 48 - 51 - N21
// 52 - 55 - N22
// 56 - 59 - N23
// 60 - 63 - N31
// 64 - 67 - N32
// 68 - 71 - N33

const byte startScore = 0;
const byte floatSize = 4;
const byte numberOfSavings = 3;
const byte startNicknames = 36;

const byte yPin = A0;
const byte xPin = A1;

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const byte buttonPin = 8;

const byte matrixSize = 8;

const int BAUD = 9600;

char matrix[matrixSize][matrixSize];

const byte NONE = 0;
const byte EASY = 1;
const byte MEDIUM = 2;
const byte HARD = 3;

byte LEVEL = NONE;

const byte wallTypes = 2;
const char walls[wallTypes] = {' ', '#'};

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);  // DIN, CLK, LOAD, No. DRIVER

byte matrixBrightness = 5;

byte xPos = NONE;                     // Variables to track the current and previous positions of the joystick-controlled LED
byte yPos = NONE;
byte xLastPos = NONE;
byte yLastPos = NONE;

const int minThreshold = 200;         // Thresholds for detecting joystick movement
const int maxThreshold = 600;

byte playerRow;
byte playerCol;

int bombRow = -1;
int bombCol = -1;

const int userBlinkingInterval = 500;
const byte bombBlinkingInterval[3] = {50, 100, 200};

unsigned long currentTime;
unsigned long startTime = millis();

unsigned long startBombingTime;
unsigned long bombPlacedTime;

unsigned long userStartTime;
unsigned long userWinTime;

bool blinkState = false;
bool bombBlinkState = false;

const unsigned int joystickSensitivity = 100;
bool joystickMoved = false;

unsigned int minimumThreshold = 350;
unsigned int maximumThreshold = 650;

const unsigned int debounceDelay = 50;
bool buttonPressed = false;
unsigned long buttonPressedTime;

const byte ASCII = 48;

bool won = false;

unsigned long getRandomSeed() {
    unsigned long seed = NONE;
    seed = millis();
    for (int i = NONE; i < matrixSize; ++i)
        seed = seed + analogRead(i);
    return seed;
}

void setup() {
  Serial.begin(BAUD);
  pinMode(buttonPin, INPUT_PULLUP);

  // delay(5000);
  // checkWin();
  // checkHighscores();
  // for(int i = 0; i < 1024; i++)
  //   EEPROM.update(i, 0);

  randomSeed(getRandomSeed());              // seed the random number generator

  lc.shutdown(NONE, false);                 // turn off power saving, enables display
  lc.setIntensity(NONE, matrixBrightness);  // sets brightness (NONE~15 possible values)
  
  setStartingEndingPositions();

  printWaitingMatrix();
  selectLevel();

  fillMatrix();
  printMatrix();

  userStartTime = millis();
}

void printWaitingMatrix() {
  lc.clearDisplay(NONE);               // clear screen
  byte waitingMatrix[matrixSize][matrixSize] = {
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 0, 0, 0, 0, 0, 1}, 
    {1, 0, 1, 0, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 0, 1, 0, 1}, 
    {1, 0, 1, 0, 0, 1, 0, 1}, 
    {1, 0, 0, 1, 1, 0, 0, 1}, 
    {1, 0, 0, 1, 1, 0, 0, 1}
  };

  for(int row = NONE; row < matrixSize; row++) {
    for(int col = NONE; col < matrixSize; col++) {
      lc.setLed(NONE, row, col, waitingMatrix[row][col]);
    }
  }
}

void selectLevel() {
  Serial.println(F("Select a difficulty (1-3). 1 - EASY, 2 - MEDIUM, 3 - HARD"));
  while(!LEVEL) {
    if(Serial.available() > 0) {
      int readValue = Serial.parseInt();
      Serial.read();
      if(readValue && readValue <= HARD) 
        LEVEL = readValue;
      else Serial.println(F("Difficulty not implemented. You must put a value between 1 and 3.\nSelect a difficulty (1-3). 1 - EASY, 2 - MEDIUM, 3 - HARD"));
    }
  }
}

void setStartingEndingPositions() {
  // Clear the matrix
  clearMatrix();

  playerCol = random(matrixSize);
  playerRow = random(matrixSize);

  matrix[playerRow][playerCol] = 'S';
}

void loop() {
  currentTime = millis();
  if(!won) {
    updateUserBlinking();
    movement();
    bombing();
  } else {
    waitForReset();
  }
}

void waitForReset() {
  byte buttonState = digitalRead(buttonPin);
  if (!buttonState) {
    if (!buttonPressed) { // verifying if the button was pressed
      buttonPressedTime = currentTime; // starting recording the time when the button was first pressed
      buttonPressed = true; // saving the fact that it was pressed
    }
  } else {
    if (buttonPressed && currentTime - buttonPressedTime >= debounceDelay) { // if it's not a long press, it will just !state of the LED
      LEVEL = NONE;
      won = false;
      setup();
    }
    buttonPressed = false; // resetting the value for the next press
  }
}

void bombing() {
  byte buttonState = digitalRead(buttonPin);
  if (!buttonState) {
    if (!buttonPressed) { // verifying if the button was pressed
      buttonPressedTime = currentTime; // starting recording the time when the button was first pressed
      buttonPressed = true; // saving the fact that it was pressed
    }
  } else {
    if (buttonPressed && currentTime - buttonPressedTime >= debounceDelay) { // if it's not a long press, it will just !state of the LED
      if(bombRow == -1) {
        bombRow = playerRow;
        bombCol = playerCol;
        matrix[bombRow][bombCol] = '3';
        startBombingTime = millis();
        bombPlacedTime = millis();
      }
    }
    buttonPressed = false; // resetting the value for the next press
  }
  if(bombRow != - 1) {
    if(currentTime - bombPlacedTime >= 1000) {
      matrix[bombRow][bombCol] -= 1;
      printMatrix();
      bombPlacedTime = currentTime;
    }
    if(matrix[bombRow][bombCol] == '0') {
      matrix[bombRow][bombCol] = walls[NONE];
      bombed();
      bombRow = -1;
    } else if(currentTime - startBombingTime >= bombBlinkingInterval[matrix[bombRow][bombCol] - 1 - ASCII]) {
        bombBlinkState = !bombBlinkState;
        if(bombRow - 1 >= NONE && matrix[bombRow - 1][bombCol] == walls[1]) {
          lc.setLed(NONE, bombRow - 1, bombCol, bombBlinkState);
        }
        if(bombRow + 1 < matrixSize && matrix[bombRow + 1][bombCol] == walls[1]) {
          lc.setLed(NONE, bombRow + 1, bombCol, bombBlinkState);
        }
        if(bombCol - 1 >= NONE && matrix[bombRow][bombCol - 1] == walls[1]) {
          lc.setLed(NONE, bombRow, bombCol - 1, bombBlinkState);
        }
        if(bombCol + 1 < matrixSize && matrix[bombRow][bombCol + 1] == walls[1]) {
          lc.setLed(NONE, bombRow, bombCol + 1, bombBlinkState);
        }
        startBombingTime = currentTime;
    }
  }
}

void bombed() {
  if(bombRow - 1 >= NONE) {
    lc.setLed(NONE, bombRow - 1, bombCol, false);
    matrix[bombRow - 1][bombCol] = walls[NONE];
  }
  if(bombRow + 1 < matrixSize) {
    lc.setLed(NONE, bombRow + 1, bombCol, false);
    matrix[bombRow + 1][bombCol] = walls[NONE];
  }
  if(bombCol - 1 >= NONE) {
    lc.setLed(NONE, bombRow, bombCol - 1, false);
    matrix[bombRow][bombCol - 1] = walls[NONE];
  }
  if(bombCol + 1 < matrixSize) {
    lc.setLed(NONE, bombRow, bombCol + 1, false);
    matrix[bombRow][bombCol + 1] = walls[NONE];
  }

  if (checkWin()) {
    won = true;
    displayWinLED();
  }
}

void displayWinLED() {
  byte winMatrix[matrixSize][matrixSize] = {
    {1, 1, 0, 0, 0, 0, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 1, 1}, 
    {1, 1, 0, 1, 1, 0, 1, 1}, 
    {1, 1, 1, 1, 1, 1, 1, 1}, 
    {1, 1, 1, 0, 0, 1, 1, 1}, 
    {1, 1, 0, 0, 0, 0, 1, 1} 
  };

  for(int row = NONE; row < matrixSize; row++) {
    for(int col = NONE; col < matrixSize; col++) {
      lc.setLed(NONE, row, col, winMatrix[row][col]);
    }
  }
}

void updateUserBlinking() {
  if (currentTime - startTime >= userBlinkingInterval) { // blinking state of the cursor
    blinkState = !blinkState;
    lc.setLed(NONE, playerRow, playerCol, blinkState);
    startTime = currentTime;
  }
}

void movement() {
  unsigned int xValue = analogRead(xPin);
  unsigned int yValue = analogRead(yPin);

  if (xValue + joystickSensitivity < minimumThreshold) {        // verifying the joystick motion for x value
    verifyJoystickMotion(NONE, -1);                                 // and the fact that the joystick could go in the desired direction
  } else if (xValue - joystickSensitivity > maximumThreshold) {
    verifyJoystickMotion(NONE, 1);
  } else if (yValue + joystickSensitivity < minimumThreshold) { // verifying the joystick motion for y value
    verifyJoystickMotion(1, NONE);                                 // and the fact that the joystick could go in the desired direction
  } else if (yValue - joystickSensitivity > maximumThreshold) {
    verifyJoystickMotion(-1, NONE);
  } else joystickMoved = false;
}

void verifyJoystickMotion(int posY, int posX) {
  if (!joystickMoved) { // verifying if it's not true so it won't move more than 1 place at a time
    if (verifyModifyState(posY, posX)) { // verifying that it can go to that path
      // Serial.println(String(posX) + String(posY));
      lc.setLed(NONE, playerRow, playerCol, false);
      playerRow += posY;
      playerCol += posX;
      lc.setLed(NONE, playerRow, playerCol, true);
      startTime = currentTime;
      joystickMoved = true; // setting it true so it won't move more than 1 place at a time
    }
  }
}

bool verifyModifyState(int posY, int posX) {
  // return matrix[playerRow + posY][playerCol + posX] == walls[NONE] && (playerRow + posY < matrixSize) && (playerRow + posY >= NONE) && (playerCol + posX < matrixSize) && (playerCol + posX >= NONE);
  if(matrix[playerRow + posY][playerCol + posX] == walls[1])
    return false;
  if(playerRow + posY >= matrixSize)
    return false;
  if(playerRow + posY < NONE)
    return false;
  if(playerCol + posX >= matrixSize)
    return false;
  if(playerCol + posX < NONE)
    return false;
  return true;
}

void fillMatrix() {
  lc.clearDisplay(NONE);               // clear screen
  int numHash;

  switch (LEVEL) {
    case 1:
      numHash = 32;
      break;
    case 2:
      numHash = 45;
      break;
    case 3:
      numHash = 55;
      break;
  }

  // Fill the matrix with # and !
  for (int i = NONE; i < numHash; i++) {
    int row = random(8);
    int col = random(8);
    if(matrix[row][col] == walls[NONE])
      matrix[row][col] = walls[1];
    else i -= 1;
  }

  matrix[playerRow][playerCol] = ' ';
}

void clearMatrix() {
  for (int i = NONE; i < matrixSize; i++) 
    for (int j = NONE; j < matrixSize; j++) 
      matrix[i][j] = walls[NONE];
}

void printMatrix() {
  for (int row = NONE; row < matrixSize; row++)
    for (int col = NONE; col < matrixSize; col++)
      lc.setLed(NONE, row, col, matrix[row][col] == walls[1] ? true : false);  // turns on LED at col, row
}

bool checkWin() {
  for (int row = NONE; row < matrixSize; row++)
    for (int col = NONE; col < matrixSize; col++)
      if (matrix[row][col] != ' ')
        return false;
  userWinTime = millis();
  float score = float(userWinTime - userStartTime) / 1000;
  // float score = 1.00;
  Serial.print(F("Ai completat nivelul in doar "));
  Serial.print(score);
  Serial.println(F(" secunde!"));

  float highscores[numberOfSavings];
  // Retrieve high scores from EEPROM
  for (int i = 0; i < numberOfSavings; i++)
    EEPROM.get((LEVEL - 1) * floatSize * numberOfSavings + i * floatSize, highscores[i]);

  for(int i = 0; i < numberOfSavings; i++)
    if(score < highscores[i] || !highscores[i]) {
      for(int j = numberOfSavings - 1; j > i; j--) {
        highscores[j] = highscores[j - 1];
      }
      highscores[i] = score;
      break;
    }

  for (int i = 0; i < numberOfSavings; i++) 
    EEPROM.put((LEVEL - 1) * floatSize * numberOfSavings + i * floatSize, highscores[i]);
  
  checkHighscores();
  return true;
}

void checkHighscores() {
  for(int levels = 0; levels < numberOfSavings; levels++) {
    Serial.println("Level " + String(levels + 1));
    for(int i = NONE; i < numberOfSavings; i++) {
      float highscore;
      EEPROM.get((levels) * floatSize * numberOfSavings + i * floatSize, highscore);
      Serial.println(String(i + 1) + ". " + String(highscore));
    }
  }
  Serial.println(F("Press the button to reset the game and start once again!"));
}
