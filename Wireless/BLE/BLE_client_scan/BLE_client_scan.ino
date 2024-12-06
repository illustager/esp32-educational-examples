/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This file demonstrates the use of BLE scanning for discovering BLE devices.
 * 
 * The code initializes a BLE client and starts scanning for nearby BLE devices.
 * 
 * The setup function initializes the serial communication and the BLE client,
 * then starts scanning.
 * 
 * The loop function is empty as the scanning is handled by the BLE library.
 * 
 * The onScanComplete function is a callback that gets called when the scanning
 * is complete. It processes the results and prints information about the
 * discovered devices.
 * 
 * The printAdvertisedDevice function prints detailed information about each
 * discovered device.
 */


#include <BLEDevice.h>

void setup() {
	Serial.begin(115200);
	delay(5000);

	BLEDevice::init("ESP32C3 BLE Client");
	BLEScan *pBLEScan = BLEDevice::getScan();

	pBLEScan->setActiveScan(true); // 能耗高，速度快
	
	Serial.println("Scanning for BLE devices...");
	pBLEScan->start(10, &onScanComplete); // 扫描时间，扫描完成回调
}

void loop() {

}

// 扫描完成回调
void onScanComplete(BLEScanResults results) {
	Serial.println("Scan complete");
	int count = results.getCount(); // 扫描到的设备数量
	Serial.print("Devices found: ");
	Serial.println(count);

	// 遍历扫描到的设备
	for (int i = 0; i < count; ++i) {
		BLEAdvertisedDevice device = results.getDevice(i);
		
		Serial.printf("[%d]:\r\n", i);
		
		// 打印设备信息
		printAdvertisedDevice(device);
	}
}

void printAdvertisedDevice(BLEAdvertisedDevice& device) {
	// 打印设备地址
	Serial.print("Address: ");
	Serial.println(device.getAddress().toString().c_str());

	Serial.print("Address type: ");
	Serial.println(device.getAddressType());

	// 打印广播数据（如果有）
	if (device.haveAppearance()) {
		Serial.print("Appearance: ");
		Serial.println(device.getAppearance());
	}

	// 打印制造商数据（如果有）
	if (device.haveManufacturerData()) {
		Serial.print("Manufacturer data: ");
		Serial.println(device.getManufacturerData().c_str());
	}

	if (device.haveName()) {
		Serial.print("Name: ");
		Serial.println(device.getName().c_str());
	}

	if (device.haveRSSI()) {
		Serial.print("RSSI: ");
		Serial.println(device.getRSSI());
	}


	// 打印所有服务 UUID
	int serviceCount = device.getServiceUUIDCount();
	for (int i = 0; i < serviceCount; ++i) {
		Serial.printf("Service(%d) UUID: ", i);
		Serial.println(device.getServiceUUID(i).toString().c_str());
	}

	int serviceDataCount = device.getServiceDataCount();
	for (int i = 0; i < serviceDataCount; ++i) {
		Serial.printf("Service(%d) data: ", i);
		Serial.println(device.getServiceData(i).c_str());
	}

	int serviceDataUUIDCount = device.getServiceDataUUIDCount();
	for (int i = 0; i < serviceDataUUIDCount; ++i) {
		Serial.printf("Service(%d) data UUID: ", i);
		Serial.println(device.getServiceDataUUID(i).toString().c_str());
	}

	if (device.haveTXPower()) {
		Serial.print("TX power: ");
		Serial.println(device.getTXPower());
	}

	// 打印 payload
	size_t payloadLength = device.getPayloadLength();
	uint8_t* payload = device.getPayload();
	Serial.print("Payload: ");
	for (int i = 0; i < payloadLength; ++i) {
		Serial.printf("%02X ", payload[i]);
	}
	Serial.println();

	Serial.println();
}
