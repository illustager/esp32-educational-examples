/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This header file provides a function to log in to the Wuhan University 
 * campus network.
 * 
 * The login function takes the username and password as arguments and 
 * returns a boolean value indicating whether the login was successful.
 * 
 * The login function also takes an optional reference to a string argument, 
 * which will be used to store the error message if the login fails.
 */


#ifndef WHUWIFI_h
#define WHUWIFI_h 20231129

#include <HTTPClient.h>
#include <ArduinoJson.h>

namespace whu {

const String headers[][2] = {
  {"Host","portal.whu.edu.cn:8080"},
  {"Content-Type","application/x-www-form-urlencoded; charset=UTF-8"},
  {"Content-Length","922"},
  {"Connection","close"},
  {"Origin","http://portal.whu.edu.cn:8080"},
  {"Referer","http://portal.whu.edu.cn:8080/eportal/index.jsp?wlanuserip=4624731a0b4b510c8bf69424df4b3602&wlanacname=29185648f4390d796f547ee673c1581a&ssid=&nasip=7c5eb3e356fdfc14806593293102756d&snmpagentip=&mac=c66a5d88ab865b984ddc3cf60d3f7684&t=wireless-v2&url=096e8e7059e430e05980c3c547fed4ab7ccaeae90dfffda4ace4874ac81c5ad69031883e2cfd5141&apmac=&nasid=29185648f4390d796f547ee673c1581a&vid=4a271faa0aa0da72&port=6b91e5d679d68d6b&nasportid=ac41d60d7f1382081362a1ed29e6af27f8be24a7bb56d0d19c3342ac86d029f5"},
};
const byte hCnt = 6;

const String others = "&service=Internet&queryString=wlanuserip%253D4624731a0b4b510c8bf69424df4b3602%2526wlanacname%253D29185648f4390d796f547ee673c1581a%2526ssid%253D%2526nasip%253D7c5eb3e356fdfc14806593293102756d%2526snmpagentip%253D%2526mac%253Dc66a5d88ab865b984ddc3cf60d3f7684%2526t%253Dwireless-v2%2526url%253D096e8e7059e430e05980c3c547fed4ab7ccaeae90dfffda4ace4874ac81c5ad69031883e2cfd5141%2526apmac%253D%2526nasid%253D29185648f4390d796f547ee673c1581a%2526vid%253D4a271faa0aa0da72%2526port%253D6b91e5d679d68d6b%2526nasportid%253Dac41d60d7f1382081362a1ed29e6af27f8be24a7bb56d0d19c3342ac86d029f5&operatorPwd=&operatorUserId=&validcode=&passwordEncrypt=false";

const String url = "http://portal.whu.edu.cn:8080/eportal/InterFace.do?method=login";

String msg = "";

bool login(const String& userId, const String& password, String& message=msg)
{
	String request = "userId=" + userId + "&password=" + password + others;

	HTTPClient http;
	http.begin(url);

	for( int i = 0; i < hCnt; ++i )
		http.addHeader( headers[i][0], headers[i][1] );

	bool success = false;

	if( http.POST( request ) > 0 ) {
		DynamicJsonDocument response(512);
		deserializeJson(response, http.getString());
		// Serial.println( response["message"].as<String>() );
		message = response["message"].as<String>();
		success = response["result"].as<String>() == "success";
	}
	else
		success = false;
	
	http.end();
    return success;
}

} // namespace whu

#endif // WHUWIFI_h
