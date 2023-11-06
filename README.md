# Introduction to Robotics (2023 - 2024)

## Description
Welcome to the repository showcasing my robotics homework assignments completed during my 3rd year at the University of Bucharest, pursuing a degree in Computer Science. In this collection, you'll find a series of projects and assignments that delve into the fascinating world of robotics.

## Homeworks
  
<details>
<summary><h3>Homework 0: Setup</h3></summary>
<br><b>To do/use list:</b><br><br>
  
- [X] GitHub repository created
- [X] Arduino IDE installed
</details>
<br>

<details>
<summary><h3>Homework 1 : RGB LED controlled by 3 potentiometers</h3></summary>
<b>Technical Task -></b>
Use a separate potentiometer for controlling each color of the RGB LED: Red, Green, and Blue.
This control must leverage digital electronics. 
Specifically, you need to read the potentiometer’s value with Arduino and then write a mapped value to the LED pins.
<br><br><b>To do/use list:</b><br><br>
  
- [X] RGB LED
- [X] Potentiometers (3)
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=ooLFFQwXiQg
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_1/IMG_6201.jpeg' width = 300>
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
  
- [X] LEDs (4)
- [X] Buttons (3)
- [X] Buzzer
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Yotube Link: https://www.youtube.com/watch?v=LoeoLPdo4wk
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_2/IMG_6356.jpeg' width = 300>
</details><br>

<details>
<summary><h3>Homework 3 : 7-Segment display drawing</h3></summary>

<b>Technical Task -></b>
You will use the joystick to control the position ofthe segment and ”draw” on the display. The movement between segmentsshould be natural, meaning they should jump from the current positiononly to neighbors, but without passing through ”walls”.
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

- [X] 7-Segment display
- [X] Joystick
- [X] Resistors and wires as needed
- [X] Arduino Code
- [X] Youtube Link: https://www.youtube.com/watch?v=jwyccp7IVYY
- [X] Setup photo
<img src = 'https://github.com/leviaici/IntroductionToRobotics/blob/main/Homework_3/IMG_6411.jpeg' width = 300>
</details><br>

<details>
<summary><h3>Homework 4 : TBA</h3></summary>

<b>Technical Task -></b>
TBA

<br><b>To do/use list:</b><br>

- [ ] TBA
<br>

## Notes
Please note that these homework assignments are part of my academic coursework at the University of Bucharest. Feel free to explore the code, documentation, and solutions to gain insights into the exciting world of robotics and computer science.

## Disclaimer
This repository is for educational purposes, and you are encouraged to learn from it. However, please avoid direct copying of code for your own coursework.
