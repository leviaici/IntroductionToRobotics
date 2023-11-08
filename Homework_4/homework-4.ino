const int button1Pin = 2;         // start / pause
const int button2Pin = 13;        // reset
const int button3Pin = 3;         // lap

const int latchPin = 11;          // pin for latching data into the register (STCP)
const int clockPin = 10;          // pin for the shift register clock (SHCP)
const int dataPin = 12;           // pin for the serial data input to the shift register (DS)

const int segD1 = 4;              // control pin for the first digit
const int segD2 = 5;              // control pin for the second digit
const int segD3 = 6;              // control pin for the third digit
const int segD4 = 7;              // control pin for the fourth digit

const byte regSize = 8;           // define the register size to be 8 bits (1 byte)

int displayDigits[] = {
  segD1, segD2, segD3, segD4
};
const int displayCount = 4;       // number of digits in the display
const int encodingsNumber = 10;   // number of different character encodings

unsigned long currentTime;
unsigned long startTime = millis();
unsigned long lastIncrementationTime = currentTime;
unsigned long lastDecrementationTime = currentTime;

const int swipeInterval = 500;    // interval of ms between swiping through laps

int numberOfExistingLaps = 0;

int byteEncodings[encodingsNumber] = {
  // A B C D E F G DP
  B11111100,  // 0
  B01100000,  // 1
  B11011010,  // 2
  B11110010,  // 3
  B01100110,  // 4
  B10110110,  // 5
  B10111110,  // 6
  B11100000,  // 7
  B11111110,  // 8
  B11110110   // 9
};

const int numberOfLaps = 4;
int indexOfLaps = 0;
int index = 0;
const int minIndex = 0;
const int maxIndex = 3;
const int dpLitUpIndex = 2;
int laps[numberOfLaps];

unsigned long lastIncrement = 0;
unsigned long delayCount = 100;  // delay between updates (milliseconds)
unsigned long number = 0;        // the number being displayed

byte startPauseButton;
byte lapButton;
byte resetButton;

byte startPauseButtonPressed = LOW;
byte lapButtonPressed = LOW;
byte resetButtonPressed = LOW;

unsigned long startPauseButtonPressedTime;
unsigned long lapButtonPressedTime;
unsigned long resetButtonPressedTime;

const int buttonDebounceDelay = 50;
const int longPressDuration = 250;

const int notReachableNumber = 10000;

const int BAUD = 9600;

enum States {
  STOPWATCH,
  PAUSE,
  RESET
};

States state = PAUSE;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  pinMode(button1Pin, INPUT_PULLUP);
  pinMode(button2Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(button1Pin), startPause, CHANGE);
  attachInterrupt(digitalPinToInterrupt(button3Pin), lap, CHANGE);

  for (int i = minIndex; i < displayCount; i++) {
    pinMode(displayDigits[i], OUTPUT);
    digitalWrite(displayDigits[i], LOW);
  }

  for (int i = 0; i < maxIndex; i++)
    laps[i] = minIndex;

  Serial.begin(BAUD);
}

void loop() {
  currentTime = millis();

  switch(state) {
    case STOPWATCH:
      if(currentTime - lastIncrement > delayCount) {            // stopwatch timer basically
        ++number;
        number %= notReachableNumber;
        lastIncrement = currentTime;
      }
      writeNumber(number);
      break;
    case PAUSE:
      verifyingResetButton();
      writeNumber(number);
      break;
    case RESET:
      verifyingResetButton();
      if (lapButtonPressed && currentTime - lapButtonPressedTime >= longPressDuration) {
        if(currentTime - lastDecrementationTime >= swipeInterval) {
          lastDecrementationTime = currentTime;
          if(index == numberOfExistingLaps - 1) 
            index = minIndex;                                   // going to minimum if we already reached the top of the array of laps
          else index++;
        }
      } else if (startPauseButtonPressed && currentTime - startPauseButtonPressedTime >= longPressDuration) {
        if(currentTime - lastDecrementationTime >= swipeInterval) {
          lastDecrementationTime = currentTime;
          if(!index)
            index = numberOfExistingLaps - 1;                   // going to maximum if we already reached the lowest of the array of laps
          else index--;
        }
      }
      writeNumber(laps[index]);
      break;
  }
}

void verifyingResetButton() {
  resetButton = digitalRead(button2Pin);

  if (!resetButton) {
      if (!resetButtonPressed) {
        if(canBeResetted()) {
          resetButtonPressedTime = currentTime;
          resetButtonPressed = true;
        }
      }
  } else {
    if (resetButtonPressed && currentTime - resetButtonPressedTime >= buttonDebounceDelay) {
      if(state != RESET) {
        state = RESET;                                        // going into the reset state so it can display user's laps
        if(indexOfLaps < numberOfLaps) 
          numberOfExistingLaps = indexOfLaps;
        else numberOfExistingLaps = numberOfLaps;
      } else {
        number = minIndex;
        for(int i = minIndex; i < numberOfExistingLaps; i++)  // resetting all laps
          laps[i] = minIndex;
        indexOfLaps = minIndex;
        numberOfExistingLaps = minIndex;
        state = PAUSE;                                        // going back to initial state / pause state
      }
    }
    resetButtonPressed = false;
  }
}

byte canBeResetted() {
  if(laps[minIndex])                                          // verifying if at least one lap exists so the reset would be relevant
    return HIGH;
  return LOW;
}

void startPause(){
  startPauseButton = digitalRead(button1Pin);

  if (!startPauseButton) {
    if (!startPauseButtonPressed) {
      startPauseButtonPressedTime = currentTime;
      startPauseButtonPressed = true;
    }
  } else {
    if (startPauseButtonPressed && currentTime - startPauseButtonPressedTime >= buttonDebounceDelay) {
      if(state != RESET) 
        changeStartPause();
      else {
        if(!index)
          index = numberOfExistingLaps - 1;       // going to maximum if we already reached the lowest of the array of laps
        else index--;
      }
    }
    startPauseButtonPressed = false;
  }
}

void lap() {
  lapButton = digitalRead(button3Pin);
  if(state != PAUSE) {                              // we can lap ourselves just when we are counting, not when we are pausing the stopwatch
    if (!lapButton) {
      if (!lapButtonPressed) {
        lapButtonPressedTime = currentTime;
        lapButtonPressed = true;
      }
    } else {
      if (lapButtonPressed && currentTime - lapButtonPressedTime >= buttonDebounceDelay) {
        if(state == STOPWATCH) {
          laps[indexOfLaps%numberOfLaps] = number; // saving or overwriting an existing lap with the current one
          indexOfLaps++;                           // going for the next index
        } else if (state == RESET) {
          if(index == numberOfExistingLaps - 1) 
            index = minIndex;                      // going to minimum if we already reached the top of the array of laps
          else index++;
        }
      }
      lapButtonPressed = false;
    }
  } 
}

void changeStartPause() {
  if(state == STOPWATCH)
    state = PAUSE;
  else state = STOPWATCH;
}

void writeReg(int digit) {
  digitalWrite(latchPin, LOW);                          // prepare to shift data by setting the latch pin low
  shiftOut(dataPin, clockPin, MSBFIRST, digit);         // shift out the byte representing the current digit to the shift register
  digitalWrite(latchPin, HIGH);                         // latch the data onto the output pins by setting the latch pin high
}

void activateDisplay(int displayNumber) {
  for (int i = minIndex; i < displayCount; i++) {
    digitalWrite(displayDigits[i], HIGH);               // turn off all digit control pins to avoid ghosting
  }
  digitalWrite(displayDigits[displayNumber], LOW);      // turn on the current digit control pin
}

void writeNumber(int number) {
  int lastDigit;
  int currentNumber = number;
  int displayDigit = maxIndex;
  while(displayDigit >= minIndex) {
    lastDigit = currentNumber % 10;                     // extracting the last digit of the current number
    activateDisplay(displayDigit);                      // activating the current digit on the display
    if(displayDigit == dpLitUpIndex)
      writeReg(byteEncodings[lastDigit] + 1);           // activating the DP on the 2nd digit
    else writeReg(byteEncodings[lastDigit]);            // writing DP on the other digits as LOW
    --displayDigit;                                     // moving to the next digit
    currentNumber /= 10;                                // removing the last digit from the current number
    writeReg(B00000000);                                // clearing the display to prevent ghosting
  }  
}
