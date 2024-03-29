const int bluePotPin = A0;
const int bluePin = 9;
unsigned int blueValue;

const int greenPotPin = A1;
const int greenPin = 10;
unsigned int greenValue;

const int redPotPin = A2;
const int redPin = 11;
unsigned int redValue;

const int minPotValue = 0;
const int maxPotValue = 1023;

const int minLedValue = 0;
const int maxLedValue = 255;

const int minValue = 20;

void setup() {
  pinMode(bluePin, OUTPUT); // making sure the pins are set up correctly
  pinMode(bluePotPin, INPUT);

  pinMode(greenPin, OUTPUT); 
  pinMode(greenPotPin, INPUT);

  pinMode(redPin, OUTPUT); 
  pinMode(redPotPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  blueValue = analogRead(bluePotPin); // reading the values from the potentiometers for RGB led
  greenValue = analogRead(greenPotPin);
  redValue = analogRead(redPotPin);

  Serial.println(blueValue);
  Serial.println(greenValue);
  Serial.println(redValue);

  int blueLedValue = map(blueValue, minPotValue, maxPotValue, minLedValue, maxLedValue); // mapping the results into values that we are going to write on the LED
  int greenLedValue = map(greenValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  int redLedValue = map(redValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  
  Serial.println(blueLedValue);
  Serial.println(greenLedValue);
  Serial.println(redLedValue);

  if(blueLedValue >= minValue) // verifying if the value should pe printed as it is or it should not as it would blink
    analogWrite(bluePin, blueLedValue); 
  else analogWrite(bluePin, minLedValue);

  if(greenLedValue >= minValue)
    analogWrite(greenPin, greenLedValue); 
  else analogWrite(greenPin, minLedValue);

  if(redLedValue >= minValue)
    analogWrite(redPin, redLedValue); 
  else analogWrite(redPin, minLedValue);
}
