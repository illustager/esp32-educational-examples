/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This file demonstrates the use of BLESerial for serial communication over BLE.
 * 
 * The code initializes a BLE serial service with specified UUIDs and sets up
 * the BLE serial communication.
 * 
 * The setup function initializes the serial communication and the BLE serial service.
 * 
 * The loop function checks if the BLE serial service is connected, and if so, reads
 * data from the BLE serial service and prints it to the serial monitor. It also reads
 * data from the serial monitor and sends it to the BLE serial service.
 */


#include "BLESerial.h"

// 串口透传服务 UUID
// 串口透传服务 RX 特征 UUID
// 串口透传服务 TX 特征 UUID
// Online UUID Generator: https://www.uuidgenerator.net/version4
#define BLEUART_SERVICE_UUID		"adc30fb6-0a34-45b5-a4a2-6b28dad3ca44"
#define BLEUART_RXCHARACTER_UUID	"2e5baf26-1de4-4209-8e48-b7b785a88aae"
#define BLEUART_TXCHARACTER_UUID	"84c19098-c1a7-49a3-adca-da9eb7b05c43"

void setup() {
	Serial.begin(115200);
	delay(5000);

	// 初始化 BLE 串口透传服务
	SerialBLE.init(	"ESP32C3 BLESerial",
					BLEUART_SERVICE_UUID,
					BLEUART_RXCHARACTER_UUID,
					BLEUART_TXCHARACTER_UUID );
	
	// 开启 BLE 串口透传
	SerialBLE.begin();

	Serial.printf("BLESerial address: %s", SerialBLE.address().toString().c_str());
	Serial.println();
}

void loop() {
	static String data;

	// 如果未连接，直接返回
	if (!SerialBLE.isConnected()) {
		return;
	}

	// 如果串口透传服务有数据可读，读取数据并打印
	if (SerialBLE.available()) {
		data = SerialBLE.readString();
		Serial.print(data);
		Serial.flush();
	}

	// 如果串口有数据可读，读取数据并发送到串口透传服务
	if (Serial.available()) {
		data = Serial.readString();
		SerialBLE.print(data);	// 发送数据，数据暂存于缓冲区
		SerialBLE.flush();		// 刷新缓冲区，发送数据到 BLE 设备
	}
}
