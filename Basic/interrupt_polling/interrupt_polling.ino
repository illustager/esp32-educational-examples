/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates how to use interrupts and polling with ESP32C3.
 * 
 * Connect an LED to pin 12 and a button to pin 7.
 * 
 * Press the button to trigger an interrupt and change the state of the LED.
 */


#define BUTTON_PIN 7
#define LED_PIN 12

void onButtonPress();
volatile bool isButtonPressed = false; //! volatile 保证变量的一致性，防止编译器优化

void setup() {
	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), &onButtonPress, FALLING);

	mode1();

	digitalWrite(LED_PIN, HIGH);
}

void loop() {
	;
}

void mode1() {
	while (true) {
		// do something

		if (isButtonPressed) {
			// do something
			return;
		}
	}
}

void onButtonPress() { // 中断处理函数
	isButtonPressed = true;
}
