/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates polling for a button press.
 * 
 * Connect an LED to pin 13 and a button to pin 7.
 * 
 * The LED will fade in and out continuously. Pressing the button will 
 * trigger a specific action, which in this case is printing "Button pressed" 
 * to the Serial Monitor.
 */


#define BUTTON_PIN 7
#define LED_PIN 13

void setup() {
	Serial.begin(115200);

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED_PIN, OUTPUT);
}

void loop() {
	if (isButtonPressed()) {
		do_something();
	}

	for (int i = 0; i <= 255; ++i ) {
		if (isButtonPressed()) {
			do_something();
		}
		
		analogWrite(LED_PIN, i);
		delay(10);
	}
	for (int i = 255; i >= 0; --i ) {
		if (isButtonPressed()) {
			do_something();
		}
		
		analogWrite(LED_PIN, i);
		delay(10);
	}
}

bool isButtonPressed() {
	bool res = (digitalRead(BUTTON_PIN) == LOW);
	while (digitalRead(BUTTON_PIN) == LOW) {
		delay(10);
	}
	return res;
}

void do_something() {
	Serial.println("Button pressed");
}
