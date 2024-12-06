/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program connects to a specified WiFi network and attempts to log in 
 * to the Wuhan University campus network.
 * 
 * To run this program, you need to replace the placeholders with your own
 * WiFi SSID, password, and Wuhan University student ID and password.
 */


#include <WiFi.h>

#include "whuWiFi.h"

// 要连接的 WiFi 名称和密码
#define WIFI_SSID		"WHU-STU"
#define WIFI_PASSWORD	"" // 武大 WiFi 无需密码，留空即可

// 认证登录武大校园网的用户名和密码（如果选择连接武大 WiFi 的话）
#define WHU_USERNAME	"{{YOUR_STUDENT_ID}}"
#define WHU_PASSWORD	"{{YOUR_PASSWORD}}"

void setup() {
	Serial.begin(115200);

	initWiFi(WIFI_SSID, WIFI_PASSWORD); // 连接到 WiFi

	// whu::login() 函数尝试登录武大校园网（如果连接到武大 WiFi 的话）
	// 连接成功返回 true，否则返回 false，且 whu::msg 会包含错误信息
	if( !whu::login(WHU_USERNAME, WHU_PASSWORD) ) {
		Serial.println(whu::msg);
		return;
	}

	Serial.println("Login successfully");
}

void loop() {
}

void initWiFi(const String& ssid, const String& password) {
	Serial.println("Connecting to Wi-Fi");
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) { // 等待连接成功
		delay(500);
		Serial.print(".");
	}
	// 连接成功后打印 IP 地址
	Serial.printf("\nConnected to %s with IP address: %s\n", 
					ssid.c_str(),
					WiFi.localIP().toString().c_str());
}
