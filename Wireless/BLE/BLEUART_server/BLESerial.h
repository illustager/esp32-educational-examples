/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This header file provides the BLESerial class which implements a BLE UART 
 * service for serial communication over BLE.
 * 
 * The BLESerial class is a singleton and provides methods to initialize, 
 * begin, and end the BLE service, as well as methods to read from and write 
 * to the BLE serial connection.
 * 
 * The class also includes callbacks for handling BLE server connections and 
 * disconnections, and for handling received data.
 * 
 * The BLESerial class uses a queue to manage incoming and outgoing data, and 
 * provides methods to check the availability of data, read data, peek at the 
 * next available data, and flush the outgoing data queue.
 */


#include "BLEUARTService.h"

#include <queue>

class BLESerial: public Stream {
	friend class BLESerialServerCallbacks;
	friend class BLESerialRXCallbacks;

public:
	static BLESerial& getInstance(	const char *deviceName,
									const char *serviceUUID,
									const char *rxCharacteristicUUID,
									const char *txCharacteristicUUID );

public:
	void init(	const char *deviceName,
				const char *serviceUUID,
				const char *rxCharacteristicUUID,
				const char *txCharacteristicUUID );

	void begin();
	void end();

	bool isConnected();
	void disableReconnect();
	void enableReconnect();

	BLEAddress address();

	BLEAdvertising *getAdvertising() {
		return pAdvertising;
	}
	BLEUARTService *getUartService() {
		return pUartService;
	}

	virtual int available() override;
	virtual int read() override;
	virtual int peek() override;
	virtual void flush() override;

	virtual size_t write(uint8_t c) override;
	virtual size_t write(const uint8_t *buffer, size_t size) override;

	using Print::write;

private:
	BLESerial();
	BLESerial(	const char *deviceName,
				const char *serviceUUID,
				const char *rxCharacteristicUUID,
				const char *txCharacteristicUUID );
	virtual ~BLESerial();
	BLESerial(const BLESerial&) = delete;
	BLESerial& operator=(const BLESerial&) = delete;

protected:
	BLEUARTService *pUartService;
	BLEAdvertising *pAdvertising;
	bool isReconnect;

private:
	bool connected;
	std::queue<uint8_t> rxQueue;
	std::queue<uint8_t> txQueue;

public:
	static const size_t MAX_DATA_SIZE;
	static BLESerial INSTANCE;
};

class BLESerialServerCallbacks: public BLEServerCallbacks {
public:	
	BLESerialServerCallbacks(BLESerial *pBLESerial): pBLESerial(pBLESerial) {
		pBLESerial->connected = false;
	}

	void onConnect(BLEServer* pServer) {
		pBLESerial->connected = true;
	};

	void onDisconnect(BLEServer* pServer) {
		pBLESerial->connected = false;
		if (pBLESerial->isReconnect) {
			pServer->startAdvertising();
		}
	}

private:
	BLESerial *pBLESerial;
};

class BLESerialRXCallbacks: public BLECharacteristicCallbacks {
public:
	BLESerialRXCallbacks(BLESerial *pBLESerial): pBLESerial(pBLESerial) {
		while (!pBLESerial->rxQueue.empty()) {
			pBLESerial->rxQueue.pop();
		}
	}

	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string rxValue = pCharacteristic->getValue().c_str();
		for (size_t i = 0; i < rxValue.size(); ++i) {
			pBLESerial->rxQueue.push(rxValue[i]);
		}
	}

private:
	BLESerial *pBLESerial;
};

#define SerialBLE BLESerial::INSTANCE


const size_t BLESerial::MAX_DATA_SIZE = BLEUARTService::MAX_DATA_SIZE;
BLESerial BLESerial::INSTANCE;

BLESerial::BLESerial()
	: pUartService(nullptr), pAdvertising(nullptr), isReconnect(true), connected(false) {
	;
}

BLESerial::BLESerial(	const char *deviceName,
						const char *serviceUUID,
						const char *rxCharacteristicUUID,
						const char *txCharacteristicUUID ) 
	: isReconnect(true), connected(false) {
	this->init(deviceName, serviceUUID, rxCharacteristicUUID, txCharacteristicUUID);
}

BLESerial::~BLESerial() {
	delete pUartService;
}

void BLESerial::init(	const char *deviceName,
						const char *serviceUUID,
						const char *rxCharacteristicUUID,
						const char *txCharacteristicUUID ) {
	// Create the BLE Device
	BLEDevice::init(deviceName);

	// Create the BLE Server
	BLEServer *pServer = BLEDevice::createServer();
	pServer->setCallbacks(new BLESerialServerCallbacks(this));
	
	// Create the BLE Service
	this->pUartService = new BLEUARTService(pServer, serviceUUID, rxCharacteristicUUID, txCharacteristicUUID);
	this->pUartService->attach(new BLESerialRXCallbacks(this));

	// Get Advertising
	this->pAdvertising = pServer->getAdvertising();
	this->pAdvertising->setScanResponse(true);
	this->pAdvertising->addServiceUUID(BLEUUID(serviceUUID));
}

void BLESerial::begin() {
	// Start the service
	pUartService->getService()->start();

	// Start advertising
	pAdvertising->start();
}

void BLESerial::end() {
	pAdvertising->stop();
	pUartService->getService()->stop();
}

bool BLESerial::isConnected() {
	return connected;
}

void BLESerial::disableReconnect() {
	isReconnect = false;
}

void BLESerial::enableReconnect() {
	isReconnect = true;
}

BLEAddress BLESerial::address() {
	return BLEDevice::getAddress();
}

int BLESerial::available() {
	return rxQueue.size();
}

int BLESerial::read() {
	if (rxQueue.empty()) {
		return -1;
	}

	uint8_t c = rxQueue.front();
	rxQueue.pop();
	return c;
}

int BLESerial::peek() {
	if (rxQueue.empty()) {
		return -1;
	}

	return rxQueue.front();
}

void BLESerial::flush() {
	uint8_t data[ESP_GATT_MAX_ATTR_LEN];
	size_t size = 0;

	// Serial.println(txQueue.size());

	while (!txQueue.empty()) {
		data[size++] = txQueue.front();
		txQueue.pop();

		// Serial.print(size);
		// Serial.print(" ");

		if (size >= MAX_DATA_SIZE || txQueue.empty()) {
			// Serial.println("flush");
			pUartService->write(data, size);
			size = 0;
		}
	}
}

size_t BLESerial::write(uint8_t c) {
	txQueue.push(c);
	return 1;
}

size_t BLESerial::write(const uint8_t *buffer, size_t size) {
	for (size_t i = 0; i < size; i++) {
		txQueue.push(buffer[i]);
	}
	return size;
}
