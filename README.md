# Arduino Serial to Ethernet gateway

Makes your RS232 devices network-compatible. A simple sketch with several additional features.

Checked to work with different scientific equipment like XGS600 gauges controller, Agilent turbo pump, Pfeifer vacuum gauge, Thermotek-ag chiller, TDK-Lambda power supply and many others.

## Features

- Configure dynamically via telnet on port 24
- Save and load serial port configuration from energy independent memory (EEEPROM)
- Autodetect the device via UDP broadcast messages
- Talk to the serial device via simple TCP/IP (for example with telnet)
- Connect to the serial device from multiple computers (up to 10)

## Quickstart

### Buy parts

To make it work you will need:

- Arduino with Ethernet or Arduino + Ethernet shield (possible options: [Arduino Leonardo ETH](http://www.arduino.org/products/boards/arduino-leonardo-eth) or [Arduino Ethernet](https://www.arduino.cc/en/Main/ArduinoBoardEthernet))
- Serial Shield for Arduino (for instance [this one](https://www.sparkfun.com/products/13029) or [do it yourself](https://robbarnsley.wordpress.com/2013/09/14/controlling-a-serial-rs-232-device-over-tcp-sockets-using-an-arduino-ethernet-board/))

### Prepare sketch and Arduino IDE

#### Ethernet.h or Ethernet2.h?

Depending on the chip that is used in your board / Ethernet shield, you will need to use either `Ethernet.h` or `Ethernet2.h` library. If your ethernet board is based on W5500, you will need `Ethernet2.h`.

If you know what library to use to make network on your board working, just comment/uncomment corresponding lines in the beginning of the sketch.

```arduino
//#include <Ethernet.h>
//#include <EthernetUdp.h>
#include <Ethernet2.h>
#include <EthernetUdp2.h>
```

To figure out which one works for you I would recommend first to upload to your board an example sketch (for example `ChatServer`) from both libraries and check which one will work.

#### Network configuration

Open the sketch a edit your network mac address (printed on the label at the bottom of the board):

```arduino
// replace this with your mac address
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x5C, 0xEE
};
```

Depending on your network, you may need or not need to assign IP address, gateway and subnet. If you have a DHCP server, you probably need to write only your mac address.

```arduino
/*
  For manual configuration of the network uncomment the following lines
  and change the Ethernet.begin arguments in the setup() function
*/
//IPAddress ip(192, 168, 1, 177);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 0, 0);

...

void setup() {
  // For DHCP use only mac. For manual configuration use all parameters.
  Ethernet.begin(mac);
//  Ethernet.begin(mac, ip, gateway, subnet);
...
}
```

#### Serial Shield configuration

This sketch uses a hardware serial port (called Serial1), that uses TX and RX pins (D0, D1) for data transmission. So, you need to configure your serial shield to use the same pins. If you want to use a software serial - you will need to change the sketch and use one of software serial libraries.

#### Serial port configuration

You may need to use a null-modem adapter or a serial cross cable to plug Arduino to your serial device. Just keep it in mind - it may be a reason if it doesn't work.

## Usage

After uploading the sketch to the board and plugging it to the serial device and. If you know the IP address of the board, you can telnet to it and start working.

If you don't know the IP address, run `python udp_scan.py` and it will print IP addresses of all devices like this one. It only works if you are in the same subnet. Another way is to plug your Arduino to the computer with USB cable and open a Serial port from Arduino IDE - it is used for debug information. Type `?` there and you will get all the info on current status of the device, including ip address.

## Remotely configuring the serial port

Telnet to the box on port **24**:

```
telnet <ip-address> 24
```

Type `help` to get all available commands.

Configure the device if necessary: set label, baudrate and other stuff if it differs from defaults. Then don't forget to type `save`. You are done. Now the box is ready for work. Next time it will load the settings from the memory.

Available commands:
- `?` - get `<label>,<baudrate>,<parity>,<wordlength>,<stopbits>`
- `label [string]` - get or set custom label for this box (up to 32 characters)
- `baudrate [value]` - get or set baudrate (300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 28800, 38400, 57600, 115200)
- `parity [value]` - get or set parity (N, E, O)
- `wordlength [value]` - get or set wordlength (5, 6, 7, 8)
- `stopbits [value]` - get or set stopbits (1, 2)
- `save` - saves current settings to EEPROM memory
- `load` - loads settings from EEPROM memory

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

## Troubleshooting

### I can't connect to the device via network

- Check if you are using [correct Ethernet and UDP libraries](#etherneth-or-ethernet2h).
- Try to upload a simple example that comes with the ethernet library (for example ChatServer).
- Check that you are connecting to the right port (23 for communication, 24 for configuration).

### I don't get a responces from my serial device

- Check that you are using a correct serial cable - you may need a cross cable or a straight cable.
- Check that serial port settings are correct (baudrate, parity, stopbits and wordlength have to be the same both in arduino and in the device).
- Try to connect your serial device directly to the computer and talk to it. Maybe you just type wrong commands.
