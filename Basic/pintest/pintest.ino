/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates how to control GPIOs with ESP32C3.
 * 
 * Connect LEDs to pins 12 and 13.
 * 
 * The program will turn LED0 off and gradually turn LED1 on, 
 * then turn LED0 on and gradually turn LED1 off.
 */


// const uint8_t availablePins[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 13, 18, 19};

const byte LED[] = {12, 13}; // 设置 LED0 和 LED1 的引脚

void setup() {
	Serial.begin(115200); // 初始化串口，波特率 115200

	// 设置引脚模式
	pinMode(LED[0], OUTPUT);
	pinMode(LED[1], OUTPUT);
}

void loop() {
	// 打印工作状态
	Serial.println("LED0 OFF, LED1 slowly ON");
	// 关闭 LED0，逐渐打开 LED1
	digitalWrite(LED[0], LOW);
	for (int i = 0; i <= 255; ++i) {
		analogWrite(LED[1], i);
		delay(5);
	}

	delay(1000); // 延时 1 秒

	// 打印工作状态
	Serial.println("LED0 ON, LED1 slowly OFF");
	// 打开 LED0，逐渐关闭 LED1
	digitalWrite(LED[0], HIGH);
	for (int i = 255; i >= 0; --i) {
		analogWrite(LED[1], i);
		delay(5);
	}

	delay(1000); // 延时 1 秒
}
