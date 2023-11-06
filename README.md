# Introduction to Robotics (2023 - 2024)

## Description
Welcome to the repository showcasing my robotics homework assignments completed during my 3rd year at the University of Bucharest, pursuing a degree in Computer Science. In this collection, you'll find a series of projects and assignments that delve into the fascinating world of robotics.

## Homeworks
  
### Homework 0 : Setup
- [X] GitHub repository created
- [X] Arduino IDE installed
<br>

### Homework 1 : RGB LED controlled by 3 potentiometers
- [X] RGB LED
- [X] Potentiometers (3)
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=ooLFFQwXiQg
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_1/IMG_6201.jpeg' width = 300>
<details>
<summary><h4>Technical Task - Homework 1</h4></summary>
Use a separate potentiometer for controlling each color of the RGB LED: Red,Green, and Blue.
This control must leverage digital electronics. 
Specifically, you need to read the potentiometer’s value with Arduino and then write amapped value to the LED pins.
</details>
<br>

### Homework 2 : 3-Floor Elevator Simulator Wannabe
- [X] LEDs (4)
- [X] Buttons (3)
- [X] Buzzer
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=LoeoLPdo4wk
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_2/IMG_6356.jpeg' width = 300>
<details>
<summary><h4>Technical Task - Homework 2</h4></summary>
Design  a  control  system  that  simulates  a  3-floor  elevator  using  the  Arduinoplatform.  Here are the specific requirements:
<li>LED Indicators: Each of the 3 LEDs should represent one of the 3 floors. The LED corresponding to the current floor should light up.  Additionally,another LED should represent the elevator’s operational state.  It should blink when the elevator is moving and remain static when stationary.</li>
<li>Buttons: Implement 3 buttons that represent the call buttons from the3 floors.  When pressed, the elevator should simulate movement towardsthe floor after a short interval (2-3 seconds).</li>
<li>Buzzer: The buzzer should sound briefly during the following scenarios:
  <ol>Elevator arriving at the desired floor (something resembling a ”cling”).</ol>
  <ol>Elevator  doors  closing  and  movement  (pro  tip:  split  them  into  2different sounds)</ol></li>
<li>State Change & Timers :If the elevator is already at the desired floor,pressing the button for that floor should have no effect.  Otherwise, aftera button press, the elevator should ”wait for the doors to close” and then”move”  to  the  corresponding  floor.   If  the  elevator  is  in  movement, it should do nothing. </li>
<li>Debounce: Remember to implement debounce for the buttons to avoidunintentional repeated button presses.</li>
</details><br>

### Homework 3 : 7-Segment display drawing
- [X] 7-Segment display
- [X] Joystick
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Youtube Link: https://www.youtube.com/watch?v=jwyccp7IVYY
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_3/IMG_6411.jpeg' width = 300>
<details>
<summary><h4>Technical Task - Homework 3</h4></summary>
You will use the joystick to control the position ofthe segment and ”draw” on the display. The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
The current position always blinks (irrespective of the fact that the segment is on or off).
Short pressing the button toggles the segmentstate from ON to OFF or from OFF to ON. Long pressing the button resets the entire display by turning all the segments OFF and moving thecurrent position to the decimal point.

The movement for each LED:
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
</details><br>

### Homework 4 : TBA
- [ ] TBA
<details>
<summary><h4>Technical Task - Homework 4</h4></summary>
TBA
</details>
<br>

## Notes
Please note that these homework assignments are part of my academic coursework at the University of Bucharest. Feel free to explore the code, documentation, and solutions to gain insights into the exciting world of robotics and computer science.

## Disclaimer
This repository is for educational purposes, and you are encouraged to learn from it. However, please avoid direct copying of code for your own coursework.
