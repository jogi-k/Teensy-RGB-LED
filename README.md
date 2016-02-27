# Teensy-RGB-LED
A simple Teensy-Programm (written as Arduino-Sketch!) to control a WS2812-RGB-Led

## Introduction
This is a very simple Arduino-Sketch for the Teensy - LC with the aim to control a WS2812-RGB-Led from a 
host-computer through a connected Teensy-LC via USB-Serial

## Preconditions
You need 
- Arduino Programming-Environment
- The Teensy-LC-Libraries as provided by PJRC on: https://www.pjrc.com/teensy/teensyLC.html
- for sure: a Teensy LC
- a Adafruit Neopixel WS2812 RGB - LED and (as recommended) a 330 Ohm-Resistor

## Wiring
Wiring is rather simple:

Teensy LC +5 V   ------------------ LED +5v
Teensy LC GND    -------------------LED GND
Teensy LC Pin 17 ----- 330 Ohm ---- LED Data In

## Usage
Just send a simple char via your host-PC to the serial port (on Linux-Machines, if you installed everything correctly a Teensy will show up on /dev/ttyACM0.
On a Linux-Machine you can simply echo to that device, eg: echo 'r' > /dev/ttyACM0
Accepted Characters:
- r => red
- g => green
- b => blue
- p => pink
- t => turquoise
- y => yellow
- w => white
- o => off
- i => idle, thats a rainbow

## Usage in "real life"
We intend to use the Teensy-RGB-LED for the following purposes
- Show the status of a Test-PC in our Test-Farm
- Use the LED as a Pomodoro-Status for the colleagues
- Use it as Wait/Ready - Indicator in Trainings : We currently are holding trainings on computers. With some easy scripts the trainer can set ALL Teensy-RGB-LEDs at all trainees-PCs e.g. to Red. The Trainee can then - when he is finished with his task at his PC - set the LED to Green. The trainer then has an easy control about the status of all trainees. 
