# Arduino Serial to Ethernet gateway

Makes your RS232 devices network-compatible. A simple sketch with several additional features.

Checked to work with different scientific equipment like XGS600 gauges controller, Agilent turbo pump, Pfeifer vacuum gauge, Thermotek-ag chiller and many others.

## Features

- Configure dynamically via telnet on port 24
- Save and load serial port configuration from energy independent memory (EEEPROM)
- Autodetect the device via UDP broadcast messages
- Talk to the serial device via simple TCP/IP (for example with telnet)
- Connect to the serial device from multiple computers (up to 10)

## What sketch should I choose?

A [hardcoded](./serial_to_ethernet_hardcoded/) version is much simpler, so for debugging it is easier. [Full](./serial_to_ethernet/) (not hardcoded) version has much more features like dynamic configuration, saving configuration and UDP autodetect. I would recommend first to try with hardcoded version, and if everything works - switch to the full version.

## How to make it work

### Buy parts

- Arduino with Ethernet or Arduino + Ethernet shield (possible options: [Arduino Leonardo ETH](http://www.arduino.org/products/boards/arduino-leonardo-eth) or [Arduino Ethernet](https://www.arduino.cc/en/Main/ArduinoBoardEthernet))
- Serial Shield for Arduino (for instance [this one](https://www.sparkfun.com/products/13029) or [do it yourself](https://robbarnsley.wordpress.com/2013/09/14/controlling-a-serial-rs-232-device-over-tcp-sockets-using-an-arduino-ethernet-board/))

### Prepare sketch and Arduino IDE

#### EEPROMAnything library

There are two sketches in the folder: `serial_to_ethernet` and `serial_to_ethernet_hardcoded`.

A full (not hardcoded) version uses [this library](https://github.com/stepansnigirev/ArduinoEEPROMAnything) as a dependency, as my Arduino IDE does not have a EEPROM.put and EEPROM.get methods (If you are unlucky to use Arduino board from arduino.org, you will have the same issue). So you need to download this library and put it to your `Documents/Arduino/libraries/` folder.

A hardcoded version does not use any dependencies but also can not be remotely re-configured. So choose which one you want to use, but I would recommend the full configurable version.

#### Ethernet.h or Ethernet2.h?

Depending on the chip that is used in your board / Ethernet shield, you will need to use either `Ethernet.h` or `Ethernet2.h` library. 

If you know what library to use to make network on your board working, just comment/uncomment corresponding lines in the begining of the sketch.

To figure out which one works for you I would recommend first to upload to your board an example sketch (for example `ChatServer`) from both libraries and check wich one will work.

#### Network configuration

First, you need to write in the sketch a correct mac address (printed on the label at the bottom of the board).

Depending on your network, you may need or not need to assign IP address, gateway and subnet. If you have a DHCP server, you probably need to write only your mac address.

#### Serial Shield configuration

This sketch uses a hardware serial port (called Serial1), that uses TX and RX pins (D0, D1) for data transmission. So you need to configure your shield to use with the same pins. If you want to use a software serial - you will need to change the sketch and use one of software serial libraries.

#### Serial port configuration

You may need to use a null-modem adapter or a serial cross cable to plug Arduino to your serial device. Just keep it in mind - it is another issue that you can encounter.

## Usage

After uploading the sketch to the board and plugging it to the serial device and. If you know the IP address of the board, you can telnet to it and start working.

If you don't know the IP address, run `python3 udp_scan.py` and it will write you IP addresses of all devices like this one. It only works if you are in the same subnet. Another way is to plug your Arduino to the computer with USB cable and open a Serial port from Arduino IDE - it is used for debug information. Type `?` there and you will get all the info on current status of the device, including ip address.

## Configuring the serial port

Telnet to the box on port **24**:

```
telnet <ip-address> 24
```

Type `help` to get all available commands.

Configure the device if necessary: set label, baudrate and other stuff if it differs from defaults. Then don't forget to type `save`. You are done. Now the box is ready for work. Next time it will load the settings from the memory.

For example:
```
baudrate 9600
parity N
stopbits 1
wordlength 8
save
?
```

## Plugging the box to the device

Once again, you will probably need a null-modem and gender-changer adapters. Then you can test if everything works by connecting to the device via telnet on port **23**:

```
telnet <ip-address> 24
```

and start typing stuff that you want to send to your serial device. All the data that you send to port 23 will be sent to the serial port and visa versa.

## Extra

There is a python script, `dummy_device.py`. It can be used for debugging.