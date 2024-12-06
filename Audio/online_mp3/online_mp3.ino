/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program connects to a specified WiFi network and streams MP3 audio
 * from a list of URLs.
 * 
 * To run this program, you need to replace the placeholders with your own
 * WiFi SSID, password, and MP3 URLs.
 */

#include <AudioTools.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h>

// ----------------------------------------------------------------------------
// 设置 I2S 输出引脚
#define I2S_DATA_PIN	8 // 连接到 I2S DAC 的 DIN 引脚
#define I2S_BCK_PIN		6 // 连接到 I2S DAC 的 BCLK 引脚
#define I2S_WS_PIN		7 // 连接到 I2S DAC 的 LRC 引脚

// ----------------------------------------------------------------------------
// wifi 配置
#define WIFI_SSID		"{{YOUR_WIFI_SSID}}"
#define WIFI_PASSWORD	"{{YOUR_WIFI_PASSWORD}}"

// ----------------------------------------------------------------------------
// 在线 mp3 音频列表
const char *urls[] = {
	"{{YOUR_MP3_URL_1}}",
	"{{YOUR_MP3_URL_2}}",
	"{{YOUR_MP3_URL_3}}"
};

// ----------------------------------------------------------------------------
// 创建在线音频流对象
URLStream urlStream(WIFI_SSID, WIFI_PASSWORD);
AudioSourceURL audioSource(urlStream, urls, "audio/mp3");
// 创建 I2S 输出流对象
I2SStream i2s;
// 创建 MP3 解码器对象
MP3DecoderHelix mp3Decoder;
// 用 音频流、I2S 输出流、MP3 解码器 创建音频播放器对象
// 一切播放操作依赖于这个对象
AudioPlayer player(audioSource, i2s, mp3Decoder);

// ----------------------------------------------------------------------------
void setup() {
	Serial.begin(115200);
	AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning); // 日志输出

	// i2s 配置
	auto cfg = i2s.defaultConfig(TX_MODE);
	cfg.pin_data	= I2S_DATA_PIN; // 依次设置输出引脚
	cfg.pin_bck		= I2S_BCK_PIN;
	cfg.pin_ws		= I2S_WS_PIN;
	
	i2s.begin(cfg); // 使用配置初始化 i2s

	// 设置音频缓冲区大小
	player.setBufferSize(1024);
	// 开始播放
	player.begin();
}

void loop() {
	player.copy(); // 拷贝音频数据到 I2S 输出缓冲区，需重复调用

	// 从串口获得控制命令
	if (Serial.available()) {
		int c = Serial.read();

		switch (c) {
			case 'p': // 播放
				player.play();
				break;
			case 's': // 暂停
				player.stop();
				break;
			case 'n': // 下一首
				player.next();
				break;
			case '+': // 音量增加 (0.0~1.0)
				player.setVolume(player.volume() + 0.1);
				break;
			case '-': // 音量减小 (0.0~1.0)
				player.setVolume(player.volume() - 0.1);
				break;
			
			default:
				break;
		}
	}
}