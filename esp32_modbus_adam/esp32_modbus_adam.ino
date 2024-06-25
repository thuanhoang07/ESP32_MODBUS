#include "ModbusMaster.h"

#define Slave_ID 1
#define RX_PIN 4 // RX2
#define TX_PIN 5 // TX2

ModbusMaster node;

void setup() {
  // Initialize Serial for communication with PC
  Serial.begin(115200);
  
  // Initialize Serial2 for communication with RS485 to TTL module
  Serial2.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);

  // Initialize Modbus communication
  node.begin(Slave_ID, Serial2);
}

void loop() {
  uint8_t result;
  result = node.readDiscreteInputs(0x0000, 4); // Read input status from address 0x0000
  
  if (result == node.ku8MBSuccess) {
    uint16_t inputStatus = node.getResponseBuffer(0);
    Serial.printf("\nReceived Data: 0x%04X", inputStatus);

    // Print each bit status
    for (uint8_t i = 0; i < 4; i++) {
      bool state = (inputStatus >> i) & 0x01;
      Serial.printf("\nDI_%02d: %s", i + 1, state ? "ON" : "OFF"); // active low
    }
  } else {
    Serial.printf("\nRead Fail, Error: %02X", result); // If reading fails
  }

  delay(1000); // Wait for a second before next read
}
