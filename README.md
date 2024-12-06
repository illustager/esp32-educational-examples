# ESP32 Educational Examples (Arduino IDE)

English | [简体中文](README_CN.md)

## Table of Contents

- [Introduction](#Introduction)
- [Examples](#Examples)
  - [Basic](#Basic)
  - [Device](#Device)
  - [Wireless](#Wireless)
  - [ADC](#ADC)
  - [Audio](#Audio)

## Introduction

This fall, I participated in a volunteer activity organized by our club, where I gave classes on the basics of microcontroller development. I wrote these examples, and I hope they are also helpful to you, wheather you're a teacher or a beginner unsure of how to get started.

ESP32 is a series of low-cost, low-power system-on-chip (SoC) microcontrollers with integrated Wi-Fi and Bluetooth. I chose ESP32C3 as the main chip for this project because it is small, inexpensive, and powerful. Arduino IDE was chosen for its simplicity, allowing students to focus on general concepts and high-level solutions instead of details.

Everything works well on the 2.x version of the ESP32 board package, but I cannot guarantee that it will work on other versions. If you encounter any issues, please let me know.

I asked my AI assistant to provide a brief introduction in English for each example at the start of the source code. However, other comments are written in Chinese (UTF-8 encoding).

Let's go through these examples in order.



## Examples

### Basic

These examples are introduced at the beginning of the course. They cover basic knowledge of microcontroller programming, such as GPIO, PWM (Pulse-Width Modulation), interrupts, etc.

#### [pintest](Basic/pintest/)

Students will learn about the GPIO pins of a microcontroller, high and low voltage levels in a digital system and PWM, and how to control an LED on a specific pin using functions like `pinMode`, `digitalWrite`, and `analogWrite` provided by the Arduino IDE. They will also use the `delay` function for delay and the `Serial` class to print information to the serial monitor.

Note that the ESP32C3 does not have a built-in DAC, and `analogWrite` outputs a PWM signal, not an actual analog signal.

#### [ugly_polling](Basic/ugly_polling/)

Students will learn the polling method for responding to peripherals. It’s a simple but somewhat cumbersome approach. It’s important to explain that the microcontroller, as the control center of the system, might be connected to multiple peripherals and needs to handle different situations in real-time, transitioning between various states.

Before this, students should first understand how to get input from the microcontroller. Besides using `digitalRead`, it’s crucial to understand pull-up and pull-down resistors. When the  input pin is floating, using pull-up or pull-down resistors ensures a defined high or low level, preventing indeterminate states.

#### [interrupt](Basic/interrupt/)

Interrupts provide another way to respond in real-time. Clarify the pros and cons of interrupts to students and emphasize that interrupt service routines (ISRs) should be as short as possible, as well as the potential for reentrancy issues.

#### [interrupt_polling](Basic/interrupt_polling/)

When ISRs are lengthy, the main processing can be moved to polling, with the ISR only marking events that need processing. This method is used in specific cases.

Explain and emphasize the role of the `volatile` keyword, which ensures that every modification to a variable is immediately written to memory, maintaining data consistency between the main program and ISRs.

You may also add extra knowledge, such as finite state machines, in combination with polling and interrupts to help students manage more complex scenarios.

#### [exception](Basic/exception/)

This is not a functional program (unless you find a microcontroller with such large memory). However, the goal here is for students to observe the microcontroller's behavior when an exception occurs and learn how to interpret the coredump information printed to the serial monitor.

On a computer, developers can easily detect when an exception occurs; however, on a microcontroller, inexperienced developers may be puzzled by the sudden erratic behavior.

#### [buzzer](Basic/buzzer/)

Use the `tone` function provided by the Arduino IDE to output a square wave at a specific frequency to drive a speaker.

Along with the other examples above, students are expected to create a simple electronic keyboard using a microcontroller, buttons, and a speaker as their first assignment.



### Device

Connecting the microcontroller to more complex peripherals.

#### [keyboard](Device/keyboard/)

Dependency: Keypad

Use the Keypad library to detect keypresses on a matrix keypad.

If needed, you can explain to students how a matrix keypad works. However, the main purpose of this example is to teach students how to use libraries—finding, installing, reading documentation, etc. When working with commonly used or complex devices, there’s no need to "reinvent the wheel"—students should learn to utilize existing resources.

#### [oled](Device/oled/)

Dependency: [U8g2](https://github.com/olikraus/u8g2)

Here, I explained three common communication protocols—SPI, I2C, and UART—and introduced some basic concepts about communication protocols. I chose an I2C OLED display for students to practice with. Of course, I recommended the U8g2 library instead of asking students to reinvent the wheel. I encouraged students to use an oscilloscope or logic analyzer to observe the signals transmitted on the data bus.

Additionally, I introduced the concept of buffers. Buffers help avoid efficiency problems caused by excessive I/O (Input/Output) operations. When developers overlook buffers, they might be confused by the program’s I/O behavior.

At this point, students can create an interactive device controlled by a keyboard and displayed on a display as an assignment for the lesson.



### Wireless

Using Wi-Fi and BLE modules.

#### [BLE](Wireless/BLE/)

At first, I intended to have students use the classic Bluetooth SPP protocol for serial communication. However, I was surprised to find that the ESP32C3 only has BLE, not classic Bluetooth. BLE was new to me at that time, and I couldn’t find an existing solution. So, I decided to learn it myself and create a BLE serial port simulation library for students to use.

On the server side, I created a *Service* with two *Characteristics* (*TX* and *RX*), where *RX* is used to receive changes from the client and *TX* is used to push data to the client. These are encapsulated in the library. On the client side, I deliberately separated the device scanning, connection, and data exchange into two examples, then asked students to combine them into a single program.

This is a good opportunity to emphasize the use of callback functions.

#### [Network](Wireless/Network/)

In the [wifitest](Wireless/Network/wifitest/) example, students learn how to connect to Wi-Fi and retrieve the IP address. Additionally, since our campus network requires authentication, I also provide a library to handle it.

Once connected to the Internet, a wide range of tasks can be accomplished. I picked two typical examples. In the [webpage](Wireless/Network/webpage/) example, the microcontroller acts as a web server, allowing users to control it through a website. In the [online_api](Wireless/Network/online_api/) example, the microcontroller accesses an online API, retrieves and parses JSON data to obtain real-time weather information.

[MQTT](https://en.wikipedia.org/wiki/MQTT) with an IoT platform is also fantastic, but due to time constraints, I didn’t introduce it to my audience.



### ADC

Converter between digital and analog serves as the bridge between digital systems and the physical world. It enables the microcontroller to perceive varying signals in the circuit and allows it to drive a speaker to produce beautiful melodies.

I originally intended to specifically discuss signal acquisition and generation with ADC (Analog-to-Digital Converter) and DAC (Digital-to-Analog Converter). However, students seemed more interesting in building fun little projects than working with signal generators and oscilloscopes. As a result, I just briefly introduced the concepts of ADC and sampling.

#### [adc_oneshot](ADC/adc_oneshot/)

Previously, I mostly explained things; here, I encourage students to read the [official documentation](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html) to learn how to use it.

Additionally, I recommend using the more feature-rich PC software [VOFA+](https://www.vofa.plus/) as a replacement for the Arduino IDE serial monitor. VOFA+ provides additional features, such as plotting waveforms based on serial data. [This image](ADC/adc_oneshot/results.jpg), created with VOFA+, shows green for the filtered ADC output and pink for the raw output.



### Audio

At the end of the course, we hope students will apply the knowledge and components we provided to build a smart wireless speaker. This project is both interesting and a great comprehensive exercise. After completing the project, students can take it home for free as a gift.

As the instructor for the microcontroller portion of the course, I provided the MAX98357A chip for audio output via I2S communication, along with two simple example codes.

I sincerely thank the [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools) project and its contributors. Their excellent work has made audio development on ESP32 chips so exciting and enjoyable. I initially thought I would need to write some low-level code, such as a more user-friendly I2S interface and audio decoding functions, but this library provided everything I needed.
