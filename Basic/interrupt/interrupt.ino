/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This example demonstrates how to use external and timer interrupts with ESP32C3.
 * 
 * Connect an LED to pin 12 and another LED to pin 13.
 * Connect a button to pin 7.
 * 
 * The LED on pin 12 will toggle its state every second using a timer interrupt.
 * The LED on pin 13 will gradually increase and decrease its brightness in a loop.
 * Pressing the button will print "Button pressed!" to the Serial Monitor.
 */


#define BUTTON_PIN 7
#define LED0_PIN 12
#define LED1_PIN 13

void ARDUINO_ISR_ATTR onTimer(void); // ARDUINO_ISR_ATTR 用于修饰中断处理函数
void ARDUINO_ISR_ATTR onButtonPress(void);

void setup() {
	Serial.begin(115200);

	pinMode(BUTTON_PIN, INPUT_PULLUP);
	pinMode(LED0_PIN, OUTPUT);
	pinMode(LED1_PIN, OUTPUT);

	// 外部中断 对于 ESP32 digitalPinToInterrupt() 可以省略
	attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), &onButtonPress, FALLING);

	// 以下两种定时器中断的写法分别适用于不同版本的 ESP32 库

	// 定时器中断 (3.x)
	// hw_timer_t *timer = timerBegin(1e6); // 1MHz
	// timerAttachInterrupt(timer, &onTimer); // 绑定中断处理函数
	// timerAlarm(timer, 1e6, true, 0); // 1s, true 为重复触发，0 为不限制触发次数

	// 定时器中断 (2.x)
	hw_timer_t *timer = timerBegin(0, 160, true); // 160MHz / 160 = 1MHz
	timerAttachInterrupt(timer, &onTimer, true); // 绑定中断处理函数
	timerAlarmWrite(timer, 1e6, true); // 1s, true 为重复触发
	timerAlarmEnable(timer); // 启用定时器中断
}

void loop() {
	for (int i = 0; i <= 255; ++i ) {
		analogWrite(LED1_PIN, i);
		delay(10);
	}
	for (int i = 255; i >= 0; --i ) {
		analogWrite(LED1_PIN, i);
		delay(10);
	}
}

void ARDUINO_ISR_ATTR onTimer() { // 间隔 1s 触发一次，切换 LED0 状态
	static bool status = LOW;
	status = !status;
	digitalWrite(LED0_PIN, status);
}

void ARDUINO_ISR_ATTR onButtonPress() { // 按下按钮时，打印 "Button pressed!"
	Serial.println("Button pressed!");
}
