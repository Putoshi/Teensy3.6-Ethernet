#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

// Artnet Specific
#define ART_NET_PORT 6454       // UDP specific
#define ART_DMX 0x5000          // Opcode Artnet DMX Paket
#define MAX_BUFFER_ARTNET 530   // Artnet Buffer Size
#define ART_NET_ID "Art-Net\0"  // Artnet Paket ID
#define ART_DMX_START 18        // Artnet LED Offset

EthernetUDP udp;
uint8_t artnetHeader[ART_DMX_START];
uint16_t packetSize;


byte mac[] = {0x00, 0x50, 0xC2, 0x97, 0x2E, 0xFC};
IPAddress ip(192, 168, 11, 222);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(200);
  Ethernet.begin(mac, ip);
  delay(200);
  
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5100) {
    Serial.println("W5100 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5200) {
    Serial.println("W5200 Ethernet controller detected.");
  }
  else if (Ethernet.hardwareStatus() == EthernetW5500) {
    Serial.println("W5500 Ethernet controller detected.");
  }

  delay(1000);
  udp.begin(ART_NET_PORT);
}

void loop () {
  packetSize = udp.parsePacket();
  
  if (packetSize <= MAX_BUFFER_ARTNET && packetSize > 0)
  {
    Serial.println(packetSize);
    
    udp.read(artnetHeader, ART_DMX_START);
    
    // Check that packetID is "Art-Net" else ignore
    if ( artnetHeader[0] == 'A' && artnetHeader[1] == 'r' && artnetHeader[2] == 't' && artnetHeader[3] == '-' 
          && artnetHeader[4] == 'N' && artnetHeader[5] == 'e' && artnetHeader[6] == 't') {
      if ((artnetHeader[8] | artnetHeader[9]) << 8 == ART_DMX)
      {
        Serial.println("packet");
      }
    };
  }
}
