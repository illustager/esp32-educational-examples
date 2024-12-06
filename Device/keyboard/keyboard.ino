/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program demonstrates the use of a keypad with the ESP32C3.
 * When a key is pressed, it is printed to the serial monitor.
 */

#include <Keypad.h>

const unsigned ROWS = 4;
const unsigned COLS = 4;

char keys[ROWS][COLS] = {
	{'1', '2', '3', 'A'},
	{'4', '5', '6', 'B'},
	{'7', '8', '9', 'C'},
	{'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {2, 3, 10, 6};
byte colPins[COLS] = {12, 18, 19, 13};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
	Serial.begin(115200);
}
void loop() {
	char key = keypad.getKey();
	if (key != NO_KEY) {
		Serial.println(key);
	}
}