/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates how to use the ESP32C3 to connect to the Internet 
 * and get weather information from the AMap API.
 * 
 * To run this example, you need to replace the placeholders with your own
 * WiFi SSID, password, and AMap API key.
 * 
 * The ESP32C3 connects to the WiFi network, sends a GET request to the AMap API,
 * and parses the JSON data to get the weather information.
 * 
 * The JSON data returned by the API is as follows:
 * {
 * 	"status": "1",
 * 	"count": "1",
 * 	"info": "OK",
 * 	"infocode": "10000",
 * 	"lives": [
 * 		{
 * 			"province": "湖北",
 * 			"city": "洪山区",
 * 			"adcode": "420111",
 * 			"weather": "晴",
 * 			"temperature": "24",
 * 			"winddirection": "东北",
 * 			"windpower": "≤3",
 * 			"humidity": "39",
 * 			"reporttime": "2024-10-25 16:31:58",
 * 			"temperature_float": "24.0",
 * 			"humidity_float": "39.0"
 * 		}
 * 	]
 * }
 */


#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// 要连接的 WiFi 名称和密码
#define WIFI_SSID		"{{YOUR_WIFI_SSID}}"
#define WIFI_PASSWORD	"{{YOUR_WIFI_PASSWORD}}"

// 这里使用的是高德地图的天气 API，使用其它 API 的流程类似
// 420111 是武汉市洪山区的城市编码
// API_KEY 请替换为你自己的 API Key
// API_URL 为 API 的请求地址
#define API_CITY_CODE	"420111"
#define API_KEY			"{{YOUR_API_KEY}}"
const String API_URL = "https://restapi.amap.com/v3/weather/weatherInfo?" + String("city=") + API_CITY_CODE + "&key=" + API_KEY;

void setup() {
	Serial.begin(115200);
	
	// 连接网络
	initWiFi(WIFI_SSID, WIFI_PASSWORD);
}

void loop() {
	// 每 10 秒获取一次天气信息，向串口输出
	getWeather();
	delay(10000);
}

void getWeather() {
	// 发送 GET 请求获取天气信息，返回的数据是 JSON 格式
	static HTTPClient http;
	http.begin(API_URL);

	int httpCode = http.GET();
	if (httpCode < 0) {
		return;
	}

	String payload = http.getString();
	http.end();

	if (httpCode != HTTP_CODE_OK) { // 请求失败
		Serial.printf("HTTP error: %d with %s", httpCode, payload.c_str());
		return;
	}

	// 解析 JSON 数据
	DynamicJsonDocument doc(512);
	deserializeJson(doc, payload);
	
	String city = doc["lives"][0]["city"];
	String weather = doc["lives"][0]["weather"];
	String temperature = doc["lives"][0]["temperature"];

	// 输出天气信息
	Serial.printf("City: %s, Weather: %s, Temperature: %s\n", city.c_str(), weather.c_str(), temperature.c_str());
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
