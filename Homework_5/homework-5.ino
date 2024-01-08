// Collab - Mihai Ionescu (shared ideas)


#include <EEPROM.h>

// EEPROM - storage map

// 0        -> Sampling interval
// 1 - 2    -> Ultrasonic threshold
// 3 - 4    -> LDR threshold
// 5 - 44   -> Ultrasonic readings
// 45 - 84  -> LDR readings
// 85       -> Automatic
// 86       -> RED
// 87       -> GREEN
// 88       -> BLUE

const byte samplingIntervalEEPROM = 0;                // index of byte in memory for the sampling rate interval used for sensors

const byte ultrasonicThresholdEEPROM_start = 1;       // starting byte in memory for the Ultrasonic sensor's threshold
const byte ultrasonicThresholdEEPROM_end = 2;         // last byte in memory for the Ultrasonic sensor's threshold

const byte ldrThresholdEEPROM_start = 3;              // starting byte in memory for the LDR sensor's threshold
const byte ldrThresholdEEPROM_end = 4;                // last byte in memory for the LDR sensor's threshold

const byte ultrasonicReadingsEEPROM_start = 5;        // starting byte in memory for the Ultrasonic sensor's readings
const byte ultrasonicReadingsEEPROM_end = 44;         // last byte in memory for the Ultrasonic sensor's readings

const byte ldrReadingsEEPROM_start = 45;              // starting byte in memory for the LDR sensor's readings
const byte ldrReadingsEEPROM_end = 84;                // last byte in memory for the LDR sensor's readings

const byte automaticEEPROM = 85;                      // index of byte in memory for the mode used for sensors and printings
const byte redLEDEEPROM = 86;                         // index of byte in memory used for storing the red value of the RGB
const byte greenLEDEEPROM = 87;                       // index of byte in memory used for storing the green value of the RGB
const byte blueLEDEEPROM = 88;                        // index of byte in memory used for storing the blue value of the RGB

byte samplingInterval = 5;
int ultrasonicThreshold = 100;
int ldrThreshold = 750;

const byte defaultSamplingInterval = 5;
const int defaultUltrasonicThreshold = 100;
const int defaultLdrThreshold = 750;

const byte numbersInInterval = 2;

const int samplingRateInterval[numbersInInterval] = {1, 10};
const int ultrasonicInterval[numbersInInterval] = {100, 400};
const int ldrThresholdInterval[numbersInInterval] = {500, 800};

const int sensorReadings = 10;

float ultrasonicStoredValues[sensorReadings] = {0};
float ldrStoredValues[sensorReadings] = {0};

int counter = 0;

const byte ldrPin = A0;          // LDR pin

const byte triggerPin = 5;       // Ultrasonic pins
const byte echoPin = 6;

const byte redPin = 9;           // RGB LED pins
const byte greenPin = 10;
const byte bluePin = 11;

byte redValue;                  // RGB LED values
byte greenValue;
byte blueValue;

const byte minLEDValue = 20;    // minimum value for LED to turn on (preventing flickering)

const int BAUD = 9600;          // BAUD value 

byte incomingByte;
const byte backSlashN = 10;

const byte minNumberValue = 48;  // value of 0 in ASCII

byte menuOption = 0;             // menu option selected
byte submenuOption = 0;          // submenu option selected

bool menuPrinted = false;        // if the text was printed for each (menu, submenu or option)
bool submenuPrinted = false;
bool functionPrinted = false;

bool inSubmenu = false;          // location of the user 
bool inFunction = false;

bool automatic = true;

const byte clearByte = 0;
const int seconds = 1000;        // ms in s

byte debounceVerifyLED = 100;
unsigned int currentTime = millis();
unsigned int lastReading = currentTime;
unsigned int lastVerifying = currentTime - debounceVerifyLED;

const byte triggerMSL = 2;       // delay for triggering low the pin of the sensor
const byte triggerMSH = 10;      // delay for triggering high the pin of the sensor

const float soundSpeed = 0.034;

int keyToExit = 5;               // key pressed to exit the real time readings

int floatSize = 4;

float lastDistance;
float lastLightIntensity;

bool startSwitch = true;

void setup() {
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  EEPROM.get(samplingIntervalEEPROM, samplingInterval);                     // reading the values for the last stored (in EEPROM) settings for sensors
  EEPROM.get(ultrasonicThresholdEEPROM_start, ultrasonicThreshold);
  EEPROM.get(ldrThresholdEEPROM_start, ldrThreshold);

  for (int index = 0; index < sensorReadings; index++) {                    // reading the values for the last stored (in EEPROM) readings from sensors 
    EEPROM.get(ultrasonicReadingsEEPROM_start + index * floatSize, ultrasonicStoredValues[index]);
    EEPROM.get(ldrReadingsEEPROM_start + index * floatSize, ldrStoredValues[index]);
  }

  automatic = EEPROM.read(automaticEEPROM);

  redValue = EEPROM.read(redLEDEEPROM);                                     // reading the last stored (in EEPROM) values of the RGB
  greenValue = EEPROM.read(greenLEDEEPROM);
  blueValue = EEPROM.read(blueLEDEEPROM);

  EEPROM.get(ultrasonicReadingsEEPROM_end - floatSize, lastDistance);       // getting the last values for readings to be stored at the start of the program
  EEPROM.get(ldrReadingsEEPROM_end - floatSize, lastLightIntensity);

  Serial.begin(BAUD);
}

void loop() {
  currentTime = millis();
  mainMenu();
  storeValues();
  verifyLED();
}

void verifyLED() {
  if(currentTime - lastVerifying >= debounceVerifyLED) {      // we are verifying if the debounce is being respected so we can prevent flickering
    float distanceNow = distanceReading();
    float lightIntensityNow = lightIntensityReading();
    byte lowLED = 0;
    byte highLED = 255;

    if (!automatic)
      rgbWriting(redValue, greenValue, blueValue);

    if (distanceNow <= ultrasonicThreshold && lightIntensityNow <= ldrThreshold) {                          // verifying if the actual readings are in the thresholds
      if (lastDistance > ultrasonicThreshold || lastLightIntensity > ldrThreshold || startSwitch) {         // verifying if the last readings were not in the thresholds so it can be a switch of states and
        if (automatic)                                                                                      // the rgb should be updated
          rgbWriting(lowLED, highLED, lowLED);
        startSwitch = false;
      }
    } else if (lastDistance <= ultrasonicThreshold && lastLightIntensity <= ldrThreshold || startSwitch) {  // same here, but vice versa + the alert if in manual mode
      if (automatic)
        rgbWriting(highLED, lowLED, lowLED);
      else Serial.println("ALERT: You are out of your boundaries! Be careful!");
      startSwitch = false;
    }

    lastDistance = distanceNow;
    lastLightIntensity = lightIntensityNow;
    lastVerifying = currentTime;
  }
}

void storeValues() {
  if (currentTime - lastReading >= samplingInterval * seconds) {                    // verifying if the interval was completed so a new value of a reading is stored in the EEPROM
    lastReading = currentTime;
    ultrasonicStoredValues[counter % sensorReadings] = distanceReading();
    ldrStoredValues[counter % sensorReadings] = lightIntensityReading();
    EEPROM.put(ultrasonicReadingsEEPROM_start + counter % sensorReadings * floatSize, ultrasonicStoredValues[counter % sensorReadings]);
    EEPROM.put(ldrReadingsEEPROM_start + counter % sensorReadings * floatSize, ldrStoredValues[counter % sensorReadings]);
    counter++;
  }
}

// ALL MENUS

void mainMenu() {
  if (!menuPrinted) {                      // if menu wasn't printed, we are printing it (same for the other submenus and functions)
    printMainMenuCommands();
    menuPrinted = true;
  }

  if (Serial.available() > 0) {            // waiting for the serial to be opened
    if(!inSubmenu) {
      incomingByte = Serial.parseInt();    // reading the next submenu option
      Serial.read();
      menuOption = incomingByte;
      inSubmenu = true;
    }
    
    if(inSubmenu) {
      switch (menuOption) {
        case 0:
          inSubmenu = false;
          break;
        case 1:
          sensorSettingsMenu();
          break;
        case 2:
          resetLoggerDataMenu();
          break;
        case 3:
          systemStatusMenu();
          break;
        case 4:
          rgbLEDControlMenu();
          break;
        default:
          Serial.println("Non-existing command. Please try again!");
          menuOption = 0;
          break;
      }
    }
  }
}

void goToMainMenu() {                       // resetting the menu to reach main menu
  submenuOption = 0;
  menuOption = 0;
  menuPrinted = false;
  submenuPrinted = false;
}

void goToSubmenu() {                        // resetting the menu to reach submenu
  submenuOption = 0;
  submenuPrinted = false;
}

void sensorSettingsMenu() {
  if (!submenuPrinted) {
    printSensorSettingsCommands();
    submenuPrinted = true;
  }

  if (!submenuOption) {
    if (Serial.available() > 0) {           // waiting for the serial to be opened
      incomingByte = Serial.parseInt();
      Serial.read();
      submenuOption = incomingByte;
    }
  }

  switch (submenuOption) {
      case 0:
        break;
      case 1:
        samplingIntervalSettings();
        break;
      case 2:
        ultrasonicAlertThreshold();
        break;
      case 3:
        ldrAlertThreshold();
        break;
      case 4:
        goToMainMenu();
        break;
      default:
        Serial.println("Non-existing command. Please try again!");
        goToSubmenu();
        break;
    }
}

void resetLoggerDataMenu() {
  if (!submenuPrinted) {
    printResetLoggerDataCommands();
    submenuPrinted = true;
  }

  if (Serial.available() > 0) {
    incomingByte = Serial.parseInt();
    Serial.read();
    submenuOption = incomingByte;
    switch (submenuOption) {
      case 0:
        break;
      case 1:
        deleteAllData();
        break;
      case 2:
        goToMainMenu();
        break;
      default:
        Serial.println("Non-existing command. Please try again!");
        goToSubmenu();
        break;
    }
  }
}

void systemStatusMenu() {
  if (!submenuPrinted) {
    printSystemStatusCommands();
    submenuPrinted = true;
  }

  if (!submenuOption) {
    if (Serial.available() > 0) {
      incomingByte = Serial.parseInt();
      submenuOption = incomingByte;
    }
  }

  switch (submenuOption) {
    case 0:
      break;
    case 1:
      currentSensorReadings();
      break;
    case 2:
      currentSensorSettings();
      break;
    case 3:
      displayLoggedData();
      break;
    case 4:
      goToMainMenu();
      break;
    default:
      Serial.println("Non-existing command. Please try again!");
      goToSubmenu();
      break;
  }
}

void rgbLEDControlMenu() {
  if (!submenuPrinted) {
    printRgbLEDControlCommands();
    submenuPrinted = true;
  }

  if (!submenuOption) {
    if (Serial.available() > 0) {
      incomingByte = Serial.parseInt();
      Serial.read();
      submenuOption = incomingByte;
    }
  }

  switch (submenuOption) {
      case 0:
        break;
      case 1:
        manualColorControl();
        break;
      case 2:
        automatic = !automatic;
        startSwitch = true;
        EEPROM.update(automaticEEPROM, automatic);
        goToSubmenu();
        break;
      case 3:
        goToMainMenu();
        break;
      default:
        Serial.println("Non-existing command. Please try again!");
        goToSubmenu();
        break;
    }
}

// SENSOR SETTINGS FUNCTIONS

void samplingIntervalSettings() {
  int readByte;
  byte modified = false;

  if (!functionPrinted) {
    Serial.println("Please, insert an integer value between 1 and 10 (seconds) for setting the sampling rate of the sensors. Default is 5.");
    Serial.println("----------------------");
    functionPrinted = true;
  }

  if (functionPrinted) {
    if (Serial.available() > 0) {                                                                   // reading an integer value and storing it just if it's between 1 and 10
      readByte = Serial.parseInt();
      if (readByte >= samplingRateInterval[0] && readByte <= samplingRateInterval[1]) {             // verifying if the read value is in the needed interval 
        samplingInterval = readByte;
        EEPROM.update(samplingIntervalEEPROM, samplingInterval);                                    // updating the value in EEPROM to keep it stored there
        modified = true;                                                                            // literally the same for the next ones
        functionPrinted = false;
      } else Serial.println("The value must be an integer between 1 and 10.");
    }
  }
  if(modified) 
    goToSubmenu();
}

void ultrasonicAlertThreshold() {
  int readByte;
  byte modified = false;

  if (!functionPrinted) {
    Serial.println("Please, insert an integer value between 100 and 400 for setting the maximum threshold for the Ultrasonic Sensor. Default is 100.");
    Serial.println("----------------------");
    functionPrinted = true;
  }

  if (functionPrinted) {
    if (Serial.available() > 0) {
      readByte = Serial.parseInt();
      if (readByte >= ultrasonicInterval[0] && readByte <= ultrasonicInterval[1]) {         // verifying if the read value is in the needed interval 
        ultrasonicThreshold = readByte;
        EEPROM.put(ultrasonicThresholdEEPROM_start, ultrasonicThreshold);
        modified = true;
        functionPrinted = false;
      } else Serial.println("The value must be an integer between 100 and 400.");
    }
  }
  if(modified) 
    goToSubmenu();
}

void ldrAlertThreshold() {
  int readByte;
  byte modified = false;

  if (!functionPrinted) {
    Serial.println("Please, insert an integer value between 500 and 800 for setting the maximum threshold for the LDR Sensor. Default is 750.");
    Serial.println("----------------------");
    functionPrinted = true;
  }

  if (functionPrinted) {
    if (Serial.available() > 0) {
      readByte = Serial.parseInt();
      if (readByte >= ldrThresholdInterval[0] && readByte <= ldrThresholdInterval[1]) {       // verifying if the read value is in the needed interval 
        ldrThreshold = readByte;
        EEPROM.put(ldrThresholdEEPROM_start, ldrThreshold);
        modified = true;
        functionPrinted = false;
      } else Serial.println("The value must be an integer between 500 and 800.");
    }
  }
  if(modified) 
    goToSubmenu();
}

// RESET LOGGER DATA FUNCTIONS

void deleteAllData() {
  for(int index = ultrasonicReadingsEEPROM_start; index < ldrReadingsEEPROM_end; index++) {
    EEPROM.update(index, clearByte);                          // clearing all the data stored in the EEPROM for log data 
  }
  Serial.println("All your logged data have been erased.");   // announcing the user its command it is done
  goToMainMenu();
}

// SYSTEM STATUS FUNCTIONS

void currentSensorReadings() {
  Serial.println("------------------------");
  Serial.print("Distance := ");
  Serial.println(distanceReading());
  Serial.print("Light := ");
  Serial.println(lightIntensityReading());               // the raw analog reading

  int readByte;
  byte exited = false;

  if (Serial.available() > 0) {
    readByte = Serial.parseInt();
    if(readByte == keyToExit) {                         // if the key is pressed, we can exit the function
      exited = true;
    }
  } else currentSensorReadings();

  if(exited) 
    goToSubmenu();
}

void currentSensorSettings() {
  Serial.println("Current sensor settings");
  Serial.println("------------------------");
  Serial.print("Sensor Sampling Interval Rate := ");
  Serial.println(samplingInterval);
  Serial.print("Ultrasonic Sensor Maximum Threshold := ");
  Serial.println(ultrasonicThreshold);
  Serial.print("LDR Sensor Maximum Threshold := ");
  Serial.println(ldrThreshold);
  
  goToSubmenu();
}

void displayLoggedData() {
  Serial.println("Sensors Logged Data");
  Serial.println("------------------------");
  for(int index = 0; index < sensorReadings; index++) {           // going through all the stored data from the last 10 readings of the sensors
    Serial.print(index + 1);
    Serial.print(". Distance := ");
    Serial.println(ultrasonicStoredValues[index]);
    Serial.print(index + 1);
    Serial.print(". Light := ");
    Serial.println(ldrStoredValues[index]);
  }
  goToSubmenu();
}

// RGB LED CONTROL FUNCTIONS

void manualColorControl() {
  if (automatic) {
    Serial.println("You cannot access this submenu as you are in automatic mode!");
    goToSubmenu();
  } else {                                  // accessing the menu just in manual mode, otherwise, exiting this function with the print
    int readByte, readByte2, readByte3;
    byte modified = false;

    if (!functionPrinted) {
      Serial.println("Please, insert 3 integer values between 0 and 255 for setting the intensity of each RGB (1 - RED, 2 - GREEN, 3 - BLUE) LED using spaces betweeen them. Default is 255.");
      Serial.println("--------------------------------------");
      functionPrinted = true;
    }

    if (functionPrinted) {
      if (Serial.available() > 0) {             // reading the 3 values of the RGB
        readByte = Serial.parseInt();
        Serial.read();
        readByte2 = Serial.parseInt();
        Serial.read();
        readByte3 = Serial.parseInt();
        if ((readByte >= 0 && readByte <= 255) && (readByte2 >= 0 && readByte2 <= 255) && (readByte3 >= 0 && readByte3 <= 255)) {
            redValue = readByte;
            EEPROM.update(redLEDEEPROM, redValue);
            greenValue = readByte2;
            EEPROM.update(greenLEDEEPROM, greenValue);
            blueValue = readByte3;
            EEPROM.update(blueLEDEEPROM, blueValue);
            modified = true;
            functionPrinted = false;
          }
        }
      }

    if(modified) {
      goToSubmenu();
    }
  }
}

float distanceReading() {
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(triggerMSL);

  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(triggerMSH);

  digitalWrite(triggerPin, LOW);

  float duration = pulseIn(echoPin, HIGH);

  return duration * soundSpeed / 2;        // consider half of the distance traveled (as we don't need the distance traveled for triggering and echoing)
}

float lightIntensityReading() {            // reading raw value from ldr sensor
  return analogRead(ldrPin);
}

void rgbWriting(byte redIntensity, byte greenIntensity, byte blueIntensity) {       // writing user's value on the rgb
  analogWrite(redPin, redIntensity >= minLEDValue ? redIntensity : LOW); 
  analogWrite(greenPin, greenIntensity >= minLEDValue ? greenIntensity : LOW); 
  analogWrite(bluePin, blueIntensity >= minLEDValue ? blueIntensity : LOW); 
}

// PRINTING MENUS

void printMainMenuCommands() {
  Serial.println("Welcome to the Main Menu");
  Serial.println("-------------------------");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
}

void printSensorSettingsCommands() {
  Serial.println("1. Sensor Settings");
  Serial.println("-------------------------");
  Serial.println("  1.1 Sensors Sampling interval");
  Serial.println("  1.2 Ultrasonic Alert Threshold");
  Serial.println("  1.3 LDR Alert Threshold");
  Serial.println("  1.4. Back");
}

void printResetLoggerDataCommands() {
  Serial.println("2. Reset Logger Data");
  Serial.println("-------------------------");
  Serial.println("Are you sure you want to delete?");
  Serial.println("-------------------------");
  Serial.println("  2.1 Yes");
  Serial.println("  2.2 No");
}

void printSystemStatusCommands() {
  Serial.println("3. System status");
  Serial.println("-------------------------");
  Serial.println("  3.1 Current Sensor Readings - Press 5 to Exit");
  Serial.println("  3.2 Current Sensor Settings");
  Serial.println("  3.3 Display Logged Data");
  Serial.println("  3.4. Back");
}

void printRgbLEDControlCommands() {
  Serial.println("4. RGB LED Control");
  Serial.println("-------------------------");
  Serial.println("  4.1 Manual Color Control");
  if (automatic)                                                            // verifying which print should we use for the mode we are in
    Serial.println("  4.2 LED: Toggle Automatic OFF (CURRENTLY ON)");
  else Serial.println("  4.2 LED: Toggle Automatic ON (CURRENTLY OFF)");
  Serial.println("  4.3. Back");
}
