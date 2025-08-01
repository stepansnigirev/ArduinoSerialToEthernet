#include <W5500lwIP.h>

#if defined(ARDUINO_WIZNET_5500_EVB_PICO) || defined(ARDUINO_WIZNET_5500_EVB_PICO2)
// To use Interrupt-driven mode, pass in an SPI object and an IRQ pin like so:
Wiznet5500lwIP eth(17, SPI, 21);
#else
Wiznet5500lwIP eth(1 /* chip select */);
#endif

int port = 23;
WiFiServer server(port);
bool already_connected = false;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  delay(5000);
  Serial.println();
  Serial.println();
  Serial.println("Starting Ethernet port");

  // Start the Ethernet port
  if (!eth.begin()) {
    Serial.println("No wired Ethernet hardware detected. Check pinouts, wiring.");
    while (1) {
      delay(1000);
    }
  }

  while (!eth.connected()) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("Ethernet connected");
  Serial.println("IP address: ");
  Serial.println(eth.localIP());

  server.begin();
  Serial.print("Telnet server started on port ");
  Serial.println(port);
}

void loop() {
  // If not connected, wait for new client
  if (!client || !client.connected()) {
    client = server.accept();
    already_connected = false;
  }

  if (client && client.connected()) {
    if (!already_connected) {
      client.flush(); // Clean input buffer on first connect
      already_connected = true;
      client.println("Type something and hit return");
    }

    while (client.available()) {
      unsigned char c = client.read();
      if (c < 128){
        Serial1.write(c);
      } else {
        Serial1.print("0x");
        Serial1.print(c, HEX);
      }
    }
    while (Serial1.available()) {
      char c = Serial1.read();
      client.write(c);
    }
  }
}
