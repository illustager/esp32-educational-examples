/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program reads analog values from a specified input pin and processes
 * average filtering.
 * 
 * To run this program, you need to connect an analog sensor to the defined
 * input pin (IN_PIN) and pull it down to GND.
 */


#define IN_PIN 2

void setup() {
	Serial.begin(115200);
	pinMode(IN_PIN, INPUT);

	analogSetAttenuation(ADC_0db); // 0dB 衰减，输入范围 0~750mV
	analogReadResolution(12); // 12 位 ADC 分辨率，亦为默认值
}

void loop() {
	static uint16_t val[3];

	val[0] = val[1];
	val[1] = val[2];
	val[2] = analogRead(IN_PIN);
	// analogReadMilliVolts(IN_PIN); // 读取电压值，单位 mV

	Serial.printf("%d, ", val[1]); // 原始值

	// 三点均值滤波
	val[1] = (val[0] + val[1] + val[2]) / 3;
	Serial.println(val[1]);

	// delay(10);
}
