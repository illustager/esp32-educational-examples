/*
 * This file is part of the ESP32C3 training examples.
 * 
 * Copyright (c) 2024 Yang D.L.
 * 
 * License: GPL v3
 * 
 * This program plays MP3 audio files from an SD card.
 * 
 * To run this program, you need to connect the appropriate I2S and SPI
 * pins to your hardwares and place MP3 files in the specified directory
 * on the SD card.
 */


#include <AudioTools.h>
#include <AudioTools/AudioLibs/AudioSourceSD.h>
#include <AudioTools/AudioCodecs/CodecMP3Helix.h>

// 设置 I2S 输出引脚
#define I2S_DATA_PIN	8 // 连接到 I2S DAC 的 DIN 引脚
#define I2S_BCK_PIN		5 // 连接到 I2S DAC 的 BCLK 引脚
#define I2S_WS_PIN		4 // 连接到 I2S DAC 的 LRC 引脚


// 设置 SPI 引脚
#define SPI_MISO_PIN	10
#define SPI_MOSI_PIN	3
#define SPI_SCK_PIN		2
#define SPI_SS_PIN		7


// 创建音频源对象
// 指定 音频文件目录 和 文件扩展名
AudioSourceSD mp3Source("/music", "mp3", SPI_SS_PIN, SPI);
// 创建 I2S 输出流对象
I2SStream i2s;
// 创建 MP3 解码器对象
MP3DecoderHelix mp3Decoder;
// 创建音频播放器对象
AudioPlayer player;


void setup() {
	Serial.begin(115200);
	AudioToolsLogger.begin(Serial, AudioToolsLogLevel::Warning); // 日志输出

	// SPI 配置
	SPI.begin(SPI_SCK_PIN, SPI_MISO_PIN, SPI_MOSI_PIN, SPI_SS_PIN);

	// I2S 配置
	auto cfg = i2s.defaultConfig(TX_MODE);
	cfg.pin_data	= I2S_DATA_PIN; // 依次设置输出引脚
	cfg.pin_bck		= I2S_BCK_PIN;
	cfg.pin_ws		= I2S_WS_PIN;
	i2s.begin(cfg);

	// 设置音频源、输出流和解码器
	player.setAudioSource(mp3Source);
	player.setOutput(i2s);
	player.setDecoder(mp3Decoder);
	// 开始播放
	player.begin();
}

void loop() {
	player.copy(); // 拷贝音频数据到 I2S 输出缓冲区，需重复调用

	if (player.isActive()) {
		// Serial.println("Playing");
	} else { // 播放结束后，重新开始播放
		player.end();
		player.begin();
	}

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
