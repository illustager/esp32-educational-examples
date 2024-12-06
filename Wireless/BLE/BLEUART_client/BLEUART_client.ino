/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This file is a BLE UART client example.
 * 
 * The client connects to a BLE UART server and sends data to the server.
 */


#include <BLEDevice.h>

// 注意 RX 和 TX 与 server 相反
#define BLEUART_UUID	"adc30fb6-0a34-45b5-a4a2-6b28dad3ca44"
#define BLEUART_TX_UUID	"2e5baf26-1de4-4209-8e48-b7b785a88aae"
#define BLEUART_RX_UUID	"84c19098-c1a7-49a3-adca-da9eb7b05c43"

BLEClient* pClient;
BLERemoteCharacteristic* pRx;
BLERemoteCharacteristic* pTx;

void setup() {
	Serial.begin(115200);
	delay(5000);


	// 初始化 BLEDvice
	BLEDevice::init("ESP32C3 BLEUART Client");
	// 创建客户端
	pClient = BLEDevice::createClient();

	// 连接到服务器
	connectToServer();
}

void loop() {
	// 如果连接断开，重新连接
	if (!pClient->isConnected()) {
		Serial.println("Disconnected");
		delay(1000);
		connectToServer();
		return;
	}

	// 从串口读取数据并发送到服务器
	if (Serial.available()) {
		String data = Serial.readString();
		pTx->writeValue(data.c_str(), data.length());
	}
}

void connectToServer() {
	static String BLEUART_server_address = "00:00:00:00:00:00";

	// 从串口读取服务器地址
	BLEUART_server_address = input("Enter BLEUART server address: ");

	// 连接到服务
	Serial.printf("Connecting to BLEUART server on %s", BLEUART_server_address.c_str());
	Serial.println();
	pClient->connect(BLEAddress(BLEUART_server_address.c_str()));
	Serial.println("Connected");

	BLERemoteService* pRemoteService = pClient->getService(BLEUUID(BLEUART_UUID));

	if (!pRemoteService) {
		Serial.println("Failed to find BLEUART service");
		return;
	}

	pRx = pRemoteService->getCharacteristic(BLEUUID(BLEUART_RX_UUID));
	pTx = pRemoteService->getCharacteristic(BLEUUID(BLEUART_TX_UUID));

	if (!pRx || !pTx) {
		Serial.println("Failed to find BLEUART characteristics");
		return;
	}

	// 注册 RX 的回调函数
	pRx->registerForNotify(&onReceived);
}

// RX 的回调函数
// 当接收到数据时，将数据打印到串口
void onReceived( BLERemoteCharacteristic* pRx,
				 uint8_t* pData,
				 size_t length,
				 bool isNotify ) {
	for (int i = 0; i < length; i++) {
		Serial.print((char)pData[i]);
	}
	Serial.flush();
}

String input(const char *prompt) {
	while (Serial.available()) {
		Serial.read();
	}

	Serial.print(prompt);
	Serial.flush();
	while (!Serial.available()) {
		delay(10);
	}
	
	String data = Serial.readString();
	data.trim();

	return data;
}
