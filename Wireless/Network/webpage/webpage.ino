/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program connects to a specified WiFi network and provides a web interface
 * to control an LED.
 * 
 * To run this program, you need to replace the placeholders with your own
 * WiFi SSID and password.
 */


#include <WiFi.h>
#include <WebServer.h>

#define WIFI_SSID		"{{WIFI_SSID}}"
#define WIFI_PASSWORD	"{{WIFI_PASSWORD}}"

WebServer websrv(80); // 创建一个 WebServer 对象，监听 80 端口

#define LED_PIN 12 // 定义 LED 引脚

void setup() {
	Serial.begin(115200);

	pinMode(LED_PIN, OUTPUT);
	digitalWrite(LED_PIN, LOW);

	// 联网
	initWiFi(WIFI_SSID, WIFI_PASSWORD);

	// 设置网页
	websrv.begin();
	websrv.on("/", HTTP_GET, &handleRoot); // 当浏览器请求根目录时，调用 handleRoot 函数
	websrv.on("/", HTTP_POST, &handlePostOnRoot); // 当浏览器提交表单时，调用 handlePostOnRoot 函数
	websrv.onNotFound(&handleNotFound); // 当浏览器请求的资源不存在时，调用 handleNotFound 函数

	Serial.println("HTTP server started");
}

void loop() {
	websrv.handleClient(); // 处理客户端请求，需要重复调用
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

// 处理根目录请求
void handleRoot() {
	static const String htmlCode = 
	"<!DOCTYPE html>                                                 " 
	"<html>                                                          "
	"    <head>                                                      "
	"        <meta charset=\"UTF-8\"/>                               "
	"        <meta name=\"viewport\" content=\"width=device-width\"> "
	"        <title>LED Button Ctrl</title>                          "
	"    </head>                                                     "
	"    <body>                                                      "
	"        <h2 align=\"center\">控制开关</h2>                       "
	"        <p>                                                     "
	"            <form action=\"/\" method=\"POST\" align=\"center\">"
	"                <input type=\"submit\" name=\"button\" value=\"Turn on LED\">"
	"            </form>                                             "
	"        </p>                                                    "
	"        <p>                                                     "
	"            <form action=\"/\" method=\"POST\" align=\"center\">"
	"                <input type=\"submit\" name=\"button\" value=\"Turn off LED\">"
	"            </form>                                             "
	"        </p>                                                    "
	"    </body>                                                     "
	"</html>";

	websrv.send(200, "text/html", htmlCode);
}

// 处理资源未找到
void handleNotFound(){																							// 当浏览器请求的网络资源无法在服务器找到时，
	websrv.send(404, "text/plain", "404: Not found");
}

// 处理根目录 POST 请求
// 当用户点击按钮时，提交表单，触发 POST 请求
// 通过判断按钮的值，控制 LED 的开关
void handlePostOnRoot() {
	if (websrv.hasArg("button")) {
		String button_value = websrv.arg("button");

		if (button_value == "Turn off LED") {
			digitalWrite(LED_PIN, LOW);
		}
		else if (button_value == "Turn on LED") {
			digitalWrite(LED_PIN, HIGH);
		}

		Serial.println(button_value);
	}
	handleRoot();
}
