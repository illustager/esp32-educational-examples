# ESP32 教育示例 (Arduino IDE)

[English](README.md) | 简体中文

## 目录

- [介绍](#介绍)
- [示例](#示例)
  - [Basic](#Basic)
  - [Device](#Device)
  - [Wireless](#Wireless)
  - [ADC](#ADC)
  - [Audio](#Audio)

## 介绍

今年秋天，我参加了我们社团组织的一次志愿活动，为我的同学们讲授了关于单片机开发的基础知识。我编写了这些示例，希望它们也能对你有所帮助——无论你是老师，还是不知如何开始的初学者。

ESP32 是一系列低成本、低功耗的片上系统微控制器，集成了 Wi-Fi 和蓝牙。我选择 ESP32C3 作为项目的主要芯片，因为它小巧、便宜且功能强大。选择 Arduino IDE 是因为它足够简单，学生可以专注于通用概念和高层次解决方案，而非细节。

所有示例在 ESP32 板包的 2.x 版本上运行良好，但我不能保证它们在其他版本上也能正常工作。如果你遇到任何问题，请告诉我。

我让我的 AI 助手为每个示例用英文写了简短的介绍，在源代码的开头。但其他注释是用中文写的（UTF-8 编码）。

让我们按顺序浏览这些示例。



## 示例

### Basic

这些示例出现在课程的开始。它们涵盖了 MCU 的基础知识，如 GPIO、PWM、中断等。

#### [pintest](Basic/pintest/)

学生将了解单片机的 GPIO 引脚，学习高低电平和 PWM，并使用 Arduino IDE 提供的 `pinMode`、`digitalWrite`、 `analogWrite` 等函数来控制特定引脚上 LED 灯的亮灭。使用 `delay` 函数来实现延迟。使用 `Serial` 类向串口监视器打印信息。

注意：ESP32C3 没有内置的 DAC，`analogWrite` 输出的是 PWM 信号，而不是真正的模拟信号。

#### [ugly_polling](Basic/ugly_polling/)

学生将学习轮询的方法来响应外设。这是一种简单但有些繁琐的方法。向学生解释清楚：单片机作为系统的控制中心，可能会连接多个外设，需要实时处理不同的情况，并在多种不同的状态间转移。

在此之前，学生应当先了解如何使用单片机获取输入。除了简单的使用 `digitalRead` 以外，更重要的是学习上拉电阻和下拉电阻。当单片机的输入引脚悬空时，使用上/下拉电阻可以将引脚电平钳制在高/低电平，避免不确定的状态。

#### [interrupt](Basic/interrupt/)

中断提供了另一种实时响应的方法。向学生澄清中断的优点和缺点，并强调中断处理函数应该尽量简短，以及重入问题。

#### [interrupt_polling](Basic/interrupt_polling/)

当中断处理函数不可避免地过长时，可以将处理函数的主体部分放在轮询中，而中断处理函数只负责标记需要处理的事件。这种做法在一些特定的情况下会被使用。

向学生解释并强调 `volatile` 关键字的作用——它保证每次对变量的修改都会被立即写入内存，以确保主程序和中断处理函数之间的数据一致性。

可以额外补充一些知识，比如有限状态机等，配合轮询和中断，使学生更好地处理复杂的情况。

#### [exception](Basic/exception/)

这不是一个可以工作的程序（除非你能找到一个内存如此大的单片机）。通过这个程序，我希望学生好好观察一下异常发生后单片机的行为，并熟悉芯片向串口监视器打印的 coredump 信息。

在计算机上编程时，开发者很容易知道程序是否发生异常；而在单片机上，缺乏经验的开发者可能会为机器突然的怪异行为感到困惑。

#### [buzzer](Basic/buzzer/)

通过 Arduino IDE 提供的 `tone` 函数输出特定频率的方波，以驱动扬声器。

结合上面的其它示例，我期望学生使用单片机、按钮和扬声器制作一个简单的电子琴，作为本节课的作业。



### Device

向单片机连接一些更复杂的外设。

#### [keyboard](Device/keyboard/)

依赖：Keypad

使用 Keypad 库来读取矩阵键盘的按键。

如果你觉得有必要，可以向学生讲解一下矩阵键盘的工作原理。但这个示例的真正目的是让学生学会使用库，比如检索需要的库、安装库、阅读库的文档等。当面对一些常用或更复杂的设备时，我们没必要事事亲为，而是应该学会利用现有的资源。

#### [oled](Device/oled/)

依赖：[U8g2](https://github.com/olikraus/u8g2)

这里我讲解了 SPI、I2C 和 UART 三种常用的通信协议，并介绍了关于通信协议的一些基本概念。我选择了一款 I2C 协议的 OLED 显示屏让同学们练习；当然，我没有让学生造轮子，而是推荐了 U8g2 库。同时鼓励学生使用示波器或逻辑分析仪来观察数据线上传输的信号。

此外，向学生介绍缓冲区的概念。缓冲区避免大量的输入输出操作影响程序的效率；而当开发者忽视缓冲区时，可能会对程序的 I/O 行为感到困惑。

到这里，可以让学生写一个由键盘控制、屏幕显示的交互设备，作为这节课的作业。



### Wireless

使用 Wi-Fi 和 BLE 模块。

#### [BLE](Wireless/BLE/)

作为示例，我本意想让学生们使用经典蓝牙的 SPP 协议来实现串口通信；但课程前我惊讶地发现 ESP32C3 只内置了 BLE，而没有经典蓝牙——这对我是全新的东西，我一时也没能找到现成的实现方案。所以我索性开始学习它，并自己写一个 BLE 模拟串口的库供学生使用。

具体来说，在服务端，我创建了一个 *Service*，包含两个 *Characterstic*（*TX* 和 *RX*），*RX* 用于接受客户端的更改，*TX* 用于向客户端推送数据。这些都被封装在库中。而对于客户端，我故意将扫描设备和连接并交换数据分成两个示例，要求学生理解并自行写**一个**程序实现它们。

这里很适合强调一下回调函数的使用。

#### [Network](Wireless/Network/)

在示例 [wifitest](Wireless/Network/wifitest/) 中，学生将学习如何连接 Wi-Fi 并获取 IP 地址。此外，鉴于我们学校的校园网需要登录认证，我还提供了进行登录认证的库。

连上网络之后可以干非常多的事情，我随便选择了两个典型的作为例子。[webpage](Wireless/Network/webpage/) 示例中，单片机作为网页服务器供用户连接，用户可以通过网站控制单片机。[online_api](Wireless/Network/online_api/) 示例中，单片机访问一个在线 API，获取并解析 JSON 数据，得到实时天气信息。

[MQTT](https://en.wikipedia.org/wiki/MQTT) 搭配一个物联网平台也是一个非常棒的东西，但由于时间限制，我没有向我的听众介绍。



### ADC

数模转换是数字系统和真实的物理世界之间的桥梁，它使电路中变化的信号能够被单片机感知，也使单片机能驱动扬声器发出美妙动听的乐曲。

我本想就 ADC(Analog-to-Digital Converter) 和 DAC(Digital-to-Analog Converter) 专门讲讲信号的采集和发生，但学生们似乎更喜欢做一些有趣的小玩意，而非对着波源和示波器工作。所以我只是简单地介绍了 ADC 和采样的一些概念。 

#### [adc_oneshot](ADC/adc_oneshot/)

此前主要是我在讲解，而这里，我鼓励学生自行阅读[官方文档](https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html)了解使用方法。

此外，我推荐更好用的上位机软件 [VOFA+](https://www.vofa.plus/)来替代 Arduino IDE 的串口监视器。VOFA+ 提供了更多的功能，包括根据串口数据绘制波形图。[这张图](ADC/adc_oneshot/results.jpg)就是它绘制的，绿色是 ADC 输出滤波后的结果，粉色是原始输出。




### Audio

课程最后，我们希望学生使用我们提供的知识和器件，制作一个智能无线音箱。这是一个非常有趣的项目，也是一个很好的综合实践。作品完成后，学生可以将其免费带回家，作为结课的礼物。

我作为课程中单片机部分的讲师，提供了使用 I2S 通信的 MAX98357A 芯片作为音频输出，以及两个简单的示例代码。

这里我想向 [Arduino Audio Tools](https://github.com/pschatzmann/arduino-audio-tools) 项目及其贡献者表达万分感谢，他们优秀的工作使得 ESP32 芯片上的音频项目开发变得如此轻松和令人愉快。我本以为需要自己写一些底层代码，比如更易用的 I2S 接口和音频解码函数，但这个库已经提供了我所需要的一切。