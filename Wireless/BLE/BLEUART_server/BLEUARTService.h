/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This header file defines the BLEUARTService class, which provides a BLE UART service
 * for communication over BLE using the ESP32C3.
 * 
 * The BLEUARTService class allows for the creation of a BLE UART service with specified
 * UUIDs for the service, RX characteristic, and TX characteristic. It provides methods
 * to write data to the TX characteristic and attach or detach callbacks for the RX 
 * characteristic.
 * 
 * The BLEUARTServiceRxCallbacks class is a helper class that handles the onWrite event
 * for the RX characteristic and invokes a user-provided callback function with the 
 * received data.
 * 
 * The MAX_DATA_SIZE constant defines the maximum size of data that can be sent in a 
 * single write operation.
 */


#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLE2902.h>

class BLEUARTServiceRxCallbacks: public BLECharacteristicCallbacks {
public:
	BLEUARTServiceRxCallbacks(std::function<void(const std::string &)> callbackfunc) {
		this->callbackfunc = callbackfunc;
	}

	void onWrite(BLECharacteristic *pCharacteristic) {
		this->rxValue = pCharacteristic->getValue().c_str();
		if (this->callbackfunc) {
			this->callbackfunc(this->rxValue);
		}
	}

private:
	std::function<void(const std::string &)> callbackfunc;
	std::string rxValue;
};

class BLEUARTService {
public:
	BLEUARTService() = delete;
	BLEUARTService(	BLEServer *pServer,
					const char *serviceUUID,
					const char *rxCharacteristicUUID,
					const char *txCharacteristicUUID );
	BLEUARTService(const BLEUARTService &) = delete;

	~BLEUARTService();

public:
	void write(const std::string &data);
	void write(const char *data);
	void write(const char *data, size_t size);
	void write(uint8_t *data, size_t size);

	void attach(std::function<void(const std::string &)> callbackfunc);
	void attach(BLECharacteristicCallbacks *pCallbacks);
	void detach();

	BLEService *getService() const;
	
private:
	BLEService *pService;
	BLECharacteristic *pTxCharacteristic;
	BLECharacteristic *pRxCharacteristic;

public:
	static const size_t MAX_DATA_SIZE;
};

const size_t BLEUARTService::MAX_DATA_SIZE = 20;

BLEUARTService::BLEUARTService(	BLEServer *pServer,
								const char *serviceUUID,
								const char *rxCharacteristicUUID,
								const char *txCharacteristicUUID ) {
	// Create the BLE UART service
	this->pService = pServer->createService(serviceUUID);

	// Create the BLE UART TX characteristic
	pTxCharacteristic = pService->createCharacteristic(
		txCharacteristicUUID,
		BLECharacteristic::PROPERTY_NOTIFY
	);
	pTxCharacteristic->addDescriptor(new BLE2902());

	// Create the BLE UART RX characteristic
	pRxCharacteristic = pService->createCharacteristic(
		rxCharacteristicUUID,
		BLECharacteristic::PROPERTY_WRITE
	);
}

BLEUARTService::~BLEUARTService() {
	;
}

BLEService *BLEUARTService::getService() const {
	return this->pService;
}

void BLEUARTService::attach(std::function<void(const std::string &)> callbackfunc) {
	this->pRxCharacteristic->setCallbacks(new BLEUARTServiceRxCallbacks(callbackfunc));
}

void BLEUARTService::attach(BLECharacteristicCallbacks *pCallbacks) {
	this->pRxCharacteristic->setCallbacks(pCallbacks);
}

void BLEUARTService::detach() {
	this->pRxCharacteristic->setCallbacks(nullptr);
}

void BLEUARTService::write(const std::string &data) {
	this->pTxCharacteristic->setValue(data.c_str());
	this->pTxCharacteristic->notify();
}

void BLEUARTService::write(const char *data) {
	this->pTxCharacteristic->setValue(data);
	this->pTxCharacteristic->notify();
}

void BLEUARTService::write(const char *data, size_t size) {
	this->pTxCharacteristic->setValue((uint8_t *)data, size);
	this->pTxCharacteristic->notify();
}

void BLEUARTService::write(uint8_t *data, size_t size) {
	this->pTxCharacteristic->setValue(data, size);
	this->pTxCharacteristic->notify();
}
