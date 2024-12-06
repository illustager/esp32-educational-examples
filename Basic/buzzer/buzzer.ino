/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates how to use a buzzer with ESP32C3.
 * 
 * Connect a buzzer to pin 12 and a button to pin 7.
 * 
 * Press the button to make a sound with a frequency of 523Hz 
 * and a duration of 500ms.
 */


#define BUZZER_PIN 12
#define BUTTON_PIN 7

void setup() {
	pinMode(BUZZER_PIN, OUTPUT);
	pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
	// 当按键按下时，发出一声音，频率 523Hz，持续 500ms
	if (digitalRead(BUTTON_PIN) == LOW) {
		tone(BUZZER_PIN, 523);
		delay(500);
		noTone(BUZZER_PIN);
	}
}
