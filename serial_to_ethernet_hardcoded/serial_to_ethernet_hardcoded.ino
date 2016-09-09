#include <SPI.h>
/* 
  Uncomment one of the following lines depanding on 
  what ethernet library works with your board.
  For Leonardo ETH from arduino.org you need Ethernet2.
*/
//#include <Ethernet.h>
#include <Ethernet2.h>

/* 
  Enabe debug tracing to usual Serial port
  Comment this for production
*/
#define DEBUG

// telnet port to send commands
#define CMD_PORT 23

// replace this with your mac address
byte mac[] = {
  0x90, 0xA2, 0xDA, 0x10, 0x5C, 0xEE
};
/* 
  For manual configuration of the network uncomment the following lines
  and change the Ethernet.begin arguments in the setup() function
*/
//IPAddress ip(192, 168, 1, 177);
//IPAddress gateway(192, 168, 1, 1);
//IPAddress subnet(255, 255, 0, 0);

// structure to store settings of the serial port
struct ComSettings{
  long baudrate;
  char parity;
  long wordlength;
  long stopbits;
};

// change this for serial port configuration
ComSettings settings = {9600, 'N', 8, 1};
//ComSettings settings = {38400, 'O', 7, 2}; // wierd settings for testing

// ===============================================================
long serialSettings(struct ComSettings s){
  // this function returns serial configuration for Serial1 library
  long conf = 0;
  long wl = 3;
  if(s.wordlength >= 5 && s.wordlength <= 8){
    wl = s.wordlength-5;
  }
  long stp = 0;
  if(s.stopbits==1 || s.stopbits==2){
    stp = s.stopbits-1;
  }
  long p = 0;
  if(s.parity=='E'){
    p=2;
  }
  if(s.parity=='O'){
    p=3;
  }
  conf = (p << 4) | (stp << 3) | (wl << 1);
  return conf;
}

bool alreadyConnected = false;

EthernetServer cmdServer(CMD_PORT);

void printConfig(){
    Serial.print("IP-address: ");
    Serial.println(Ethernet.localIP());
    if(!Serial1){
      Serial.println("Serial port is closed");
    }else{
      Serial.println("Serial port is opened");
    }
    Serial.println("Serial configuration:");
    Serial.print("Baudrate: ");
    Serial.println(settings.baudrate);
    Serial.print("Parity: ");
    Serial.println(settings.parity);
    Serial.print("Wordlength: ");
    Serial.println(settings.wordlength);
    Serial.print("Stopbits: ");
    Serial.println(settings.stopbits);
}

void setup() {
  // For DHCP use only mac. For manual configuration use all parameters.
  Ethernet.begin(mac);
//  Ethernet.begin(mac, ip, gateway, subnet);
  cmdServer.begin();
  Serial1.begin(settings.baudrate, serialSettings(settings));
  #ifdef DEBUG
    Serial.begin(9600);
    printConfig();
  #endif  
}

char c;

void loop() {
  // wait for a new client:
  EthernetClient client = cmdServer.available();

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clean out the input buffer:
      client.flush();
      alreadyConnected = true;
    }

    if (client.available() > 0){
      c = client.read();
      Serial1.write(c);
      #ifdef DEBUG
        Serial.write(c);
      #endif
    }
  }
  if(Serial1.available() > 0){
    c = Serial1.read();
    cmdServer.write(c);
    #ifdef DEBUG
      Serial.write(c);
    #endif
  }
  #ifdef DEBUG
    if(Serial.available() > 0){
      Serial.read();
      printConfig();
    }
  #endif
}
