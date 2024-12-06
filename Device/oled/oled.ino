/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program initializes a 4-pin OLED display using the U8g2 library.
 * It sets up the display with specified SDA and SCL pins, and displays 
 * the text "Hello World!" on the screen.
 * 
 * To run this program, you need to connect a 4-pin OLED display to the 
 * specified SDA and SCL pins on your ESP32C3 board.
 */


#include <U8g2lib.h>

#define SDA 12
#define SCL 13

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA);

void setup(void) {
	u8g2.setFont(u8g2_font_ncenB08_tr); // 设置字体
	u8g2.begin(); // 初始化
	u8g2.clearBuffer(); // 清空缓冲区

	// 显示字符串, 位置(0,10)
	// 字符串不会立即显示在屏幕上, 而是暂存在缓冲区中
	u8g2.drawStr(0, 10, "Hello World!");

	// 发送缓冲区内容到显示屏，使之前的输出语句生效
	u8g2.sendBuffer();
}

void loop(void) {
}