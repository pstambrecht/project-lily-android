# Project Lily: Arduino
## About
Project Lily is a project about monitoring and watering flowers. This project consist of Android application, Android Things application, Sprint application and Arduino application which communicate with each other.
This repository contains Arduino part (other applications will be published later - maybe).

## Main functionality:
* Measuring temperature via DTH 22 sensor
* Measuring intensity of light via TEMT6000 light sensor
* Measuring humidity via soil hygrometer (from wire)
* Serial port communication
* Synchronization bytes on start (sometimes not fully data has been sent via serial port on start)
