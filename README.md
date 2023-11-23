# Introduction to Robotics (2023 - 2024)

## Description
Welcome to the repository showcasing my robotics homework assignments completed during my 3rd year at the University of Bucharest, pursuing a degree in Computer Science. In this collection, you'll find a series of projects and assignments that delve into the fascinating world of robotics.

## Homeworks
  
<details>
<summary><h3>Homework 0 : Setup</h3></summary>
<br><b>To do/use list:</b><br><br>
  
- [X] GitHub repository created
- [X] Arduino IDE installed
- [X] Arduino UNO Kit
       
</details>
<br>

<details>
<summary><h3>Homework 1 : RGB LED controlled by 3 potentiometers</h3></summary>
<b>Technical Task -></b>
Use a separate potentiometer for controlling each color of the RGB LED: Red, Green, and Blue.
This control must leverage digital electronics. 
Specifically, the potentiometer’s value need to be read with Arduino and then write a mapped value to the LED pins.
<br><img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_1/IMG_6201.jpeg' align="right" height = 300 width = 300>
<br><br><b>To do/use list:</b><br><br>
  
- [X] Arduino UNO Board
- [X] RGB LED
- [X] Potentiometers (3)
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=ooLFFQwXiQg
- [X] Setup photo

</details>
<br>

<details>
<summary><h3>Homework 2 : 3-Floor Elevator Simulator Wannabe</h3></summary>
<b>Technical Task -></b>
Design a control system that simulates a 3-floor elevator using the Arduino platform. Here are the specific requirements:

* LED Indicators: Each of the 3 LEDs should represent one of the 3 floors. The LED corresponding to the current floor should light up. Additionally, another LED should represent the elevator’s operational state.  It should blink when the elevator is moving and remain static when stationary.
* Buttons: Implement 3 buttons that represent the call buttons from the 3 floors. When pressed, the elevator should simulate movement towardsthe floor after a short interval (2-3 seconds).
* Buzzer: The buzzer should sound briefly during the following scenarios:
  
  <ol>- Elevator arriving at the desired floor (something resembling a ”cling”).</ol>
  <ol>- Elevator  doors  closing  and  movement  (pro  tip:  split  them  into  2different sounds)</ol>

* State Change & Timers: If the elevator is already at the desired floor, pressing the button for that floor should have no effect. Otherwise, after a button press, the elevator should ”wait for the doors to close” and then ”move” to the corresponding floor. If the elevator is in movement, it should do nothing. 
* Debounce: Remember to implement debounce for the buttons to avoid unintentional repeated button presses.
  
<br><b>To do/use list:</b><br>
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_2/IMG_6356.jpeg' align="right" width = 300>
  
- [X] Arduino UNO Board
- [X] LEDs (4)
- [X] Buttons (3)
- [X] Buzzer
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=LoeoLPdo4wk
- [X] Setup photo
</details><br>

<details>
<summary><h3>Homework 3 : 7-Segment display drawing</h3></summary>

<b>Technical Task -></b>
The joystick will be used to control the position ofthe segment and ”draw” on the display. The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
The current position always blinks (irrespective of the fact that the segment is on or off).
Short pressing the button toggles the segment state from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving the current position to the decimal point.
<br>

<b>The movement for each LED:</b>

| Current LED | UP  | DOWN | LEFT | RIGHT |
| :---------: | :-: | :--: | :--: | :---: |
|      a      | N/A |  g   |  f   |   b   |
|      b      |  a  |  g   |  f   |  N/A  |
|      c      |  g  |  d   |  e   |  dp   |
|      d      |  g  | N/A  |  e   |   c   |
|      e      |  g  |  d   | N/A  |   c   |
|      f      |  a  |  g   | N/A  |   b   |
|      g      |  a  |  d   | N/A  |  N/A  |
|     dp      | N/A | N/A  |  c   |  N/A  |

<br><b>To do/use list:</b><br>
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_3/IMG_6411.jpeg' align="right" width = 300>

- [X] Arduino UNO Board
- [X] 7-Segment display
- [X] Joystick
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Youtube Link: https://www.youtube.com/watch?v=jwyccp7IVYY
- [X] Setup photo

</details><br>

<details>
<summary><h3>Homework 4 : Stopwatch Timer</h3></summary>

<b>Technical Task -></b>
Using the 4 digit 7 segment display and 3 buttons, a stopwatch timer that counts in 10ths of a second and has a save lap functionality (similar to most basic stopwatch functions on most phones) is implemented.

<br><b>Requirements</b><br>

The starting value of the 4 digit 7 segment display should be ”000.0”. The buttons have the following functionalities:
* Button 1: Start / pause.
* Button 2: Reset (if  in  pause  mode). Reset saved laps (if in lap viewing mode).
* Button 3: Save lap (if in counting mode), cycle through last saved laps (up to 4 laps).

<br><b>Workflow</b><br>

* Display shows "000.0". When pressing the Startbutton, the timer should start.
* During timer counter, each press of the lap button, the timer's value is saved in memory (not persistent, it is OK to be deleted upon reset), up to 4 laps; pressing the 5th time should override  the  1st  saved  one. If the reset button is pressed while timer works, nothing happens. If the pause button is pressed, the timer stops.
* In Pause Mode, the lap flag button doesn’t work anymore. Pressing the reset button sends you to viewing lap times mode.
* After reset, the flag buttons can now be pressed to cycle through the lap times. Each time the flag button is pressed, it takes you to the next saved lap. Pressing it continuously cycle you through it continuously. Pressing the reset button while in this state resets all your flags and takes the timer back to "000.0".

<br><b>To do/use list:</b><br>
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_4/IMG_6471.jpeg' align="right" width = 300>

- [X] Arduino UNO Board
- [X] 4 digit 7 segment display
- [X] 3 buttons
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Youtube Link: https://www.youtube.com/watch?v=I68L3hdWD3I
- [X] Setup photo

</details><br>

<details>
<summary><h3>Homework 5 : Smart Environment Monitor and Logger</h3></summary>

<b>Technical Task -></b>
Developed a ”Smart Environment Monitor and Logger” using Arduino.  This system will utilize various sensors to gather environmental data, log this data into EEPROM, and provide both visual feedback via an RGB LED and user interaction through a Serial Menu. The project focuses on integrating sensor readings, memory management, Serial Communication and the general objective of building a menu.

<br><b>Menu structure</b><br>

* Sensor Settings
 
  <ol><b>1.1 Sensors Sampling Interval -> </b>Here a value between 1 and 10 seconds will be read from the Serial menu. This value is used as a sampling rate for the sensors.</ol><br>
  <ol><b>1.2 Ultrasonic Alert Threshold -> </b>Here a value will be read from the Serial menu. This value is used as a threshold value for the ultrasonic sensor. When sensor value exceeds the threshold value, an alert should be given. If the LED is set to Automatic Mode (see section 4.2), it will also turn red if any of the sensors are outside the value.</ol><br>
  <ol><b>1.3 LDR Alert Threshold -> </b>Here a value will be read from the Serial menu. This value is used as a threshold value for the LDR sensor. When sensor value exceeds the threshold value, an alert should be given. If the LED is set to Automatic Mode (see section 4.2), it will also turn red if any of the sensors are outside the value.</ol><br>
  <ol><b>1.4 Back -> </b>Sending you back to the main menu.</ol><br>

* Reset Logger Data

  Resetting all the sensors readings data stored in EEPROM.

  <ol><b>2.1 Yes</b></ol><br>
  <ol><b>2.2 No</b></ol><br>

* System Status

  <ol><b>3.1 Current Sensor Readings -> </b>Continuously print sensor readings at the set sampling rate, from all sensors. Exiting this submenu will be done by pressing a specific key (mentioned when entering in the submenu).</ol><br>
  <ol><b>3.2 Current Sensor Settings -> </b>Displays  the  sampling rate and threshold value for all sensors.</ol><br>
  <ol><b>3.3 Display Logged Data -> </b>Displays last 10 sensor readings for all sensors.</ol><br>
  <ol><b>3.4 Back -> </b>Sending you back to the main menu.</ol><br>

* RGB LED Control

<ol><b>4.1 Manual Color Control -> </b>Set the RGB colors manually by entering input data for red, green and blue values.</ol><br>
<ol><b>4.2 LED: Toggle Automatic ON/OFF -> </b>If automatic mode is ON, then the led color should be GREEN when all sensors value don't exceed threshold values (no alert) and RED when there is an alert (any sensor value exceeds the specified threshold). When automatic mode is OFF, then the LED should use the last saved RGB values.</ol><br>
<ol><b>4.3 Back -> </b>Sending you back to the main menu.</ol><br>

<br><b>To do/use list:</b><br>
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_5/IMG_6565.jpeg' align="right" width = 300>

- [X] Arduino UNO Board
- [X] Ultrasonic Sensor (HC-SR04)
- [X] LDR (Light-Dependent Resistor)
- [X] RGB LED
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Youtube Link: https://www.youtube.com/watch?v=aZ6Cmcan-4I
- [X] Setup photo
      
</details><br>

<details>
<summary><h3>Homework 6: Mini Matrix Game</h3></summary>
<b>Technical Task -></b>
Small game on the 8x8 matrix. The basic idea of the game is that it generates random walls on the map (50% - 75% of the map) and then you move around with the player and destroy them.

<br><b>Features</b>

<ol><b>Dynamic Difficulty:</b> Choose the difficulty level (1 to 3) at the start to control the complexity of the maze.<br></ol>
<ol><b>Randomly Generated Map:</b> The game generates a unique map on the LED matrix for each playthrough, adding variety and challenge.<br></ol>
<ol><b>Joystick Control:</b> Navigate through the maze using a joystick, providing a responsive and intuitive user experience.<br></ol>
<ol><b>Bombs Away!:</b> Deploy bombs by clicking on the physical button to clear walls and make your way through the maze.<br></ol>
<ol><b>EEPROM High Scores:</b> Your best times are saved in the Arduino's EEPROM. If you beat your previous high score, it will be updated.<br></ol>
<ol><b>Game Reset:</b> After completing the maze, press the physical button to reset the game and start a new challenge.<br></ol>

<br><b>How to Use</b>

<ol><b>Power On:</b> Connect your Arduino Uno and power it on.<br></ol>
<ol><b>Set Username:</b> Choose the username (3 characters maximum) by entering the corresponding username using the keyboard.<br></ol>
<ol><b>Set Difficulty:</b> Choose the difficulty level (1 to 3) by entering the corresponding number using the keyboard.<br></ol>
<ol><b>Navigate the Maze:</b> Use the joystick to move through the maze.<br></ol>
<ol><b>Place Bombs:</b> Click the physical button to place bombs strategically and clear walls.<br></ol>
<ol><b>Beat the High Score:</b> Your best times are saved in EEPROM. Beat your previous high scores!<br></ol>
<ol><b>Game Reset:</b> After completing the maze, press the physical button to reset the game for a new challenge.<br></ol>
  
<br><b>To do/use list:</b><br>
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/matrixGame/IMG_6616.jpeg' align="right" width = 300>
  
- [X] Arduino UNO Board
- [X] Joystick
- [X] 8x8 LED Matrix
- [X] MAX7219
- [X] Resistors, capacitors and wires as needed
- [X] Breadboard 
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=Q00_0WmDssw
- [X] Setup photo
</details><br>


<details>
<summary><h3>Homework 7 : TBA</h3></summary>
  
<b>Technical Task -></b>
TBA

<br><b>To do/use list:</b><br>

- [ ] TBA

</details><br>

## Notes
Please note that these homework assignments are part of my academic coursework at the University of Bucharest. Feel free to explore the code, documentation, and solutions to gain insights into the exciting world of robotics and computer science.

## Disclaimer
This repository is for educational purposes, and you are encouraged to learn from it. However, please avoid direct copying of code for your own coursework.
