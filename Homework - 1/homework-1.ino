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

void setup() {
  pinMode(bluePin, OUTPUT); 
  pinMode(bluePotPin, INPUT);

  pinMode(greenPin, OUTPUT); 
  pinMode(greenPotPin, INPUT);

  pinMode(redPin, OUTPUT); 
  pinMode(redPotPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  blueValue = analogRead(bluePotPin);
  greenValue = analogRead(greenPotPin);
  redValue = analogRead(redPotPin);

  Serial.println(blueValue);
  Serial.println(greenValue);
  Serial.println(redValue);

  int blueLedValue = map(blueValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  int greenLedValue = map(greenValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  int redLedValue = map(redValue, minPotValue, maxPotValue, minLedValue, maxLedValue);
  
  Serial.println(blueLedValue);
  Serial.println(greenLedValue);
  Serial.println(redLedValue);

  if(blueLedValue >= 20)
    analogWrite(bluePin, blueLedValue); 

  if(greenLedValue >= 20)
    analogWrite(greenPin, greenLedValue); 

  if(redLedValue >= 20)
    analogWrite(redPin, redLedValue); 
}