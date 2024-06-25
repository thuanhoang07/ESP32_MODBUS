#include "ModbusMaster.h"
#define Slave_ID    1   //xY-MD02 = 1
#define RX_PIN      16 //RX2 
#define TX_PIN      17  //TX2 

ModbusMaster modbus;

void preTransmission() {
  //digitalWrite(MAX485_RE_NEG, 1);
  //digitalWrite(MAX485_DE, 1);
}

void postTransmission() {
  //digitalWrite(MAX485_RE_NEG, 0);
  //digitalWrite(MAX485_DE, 0);
}

void setup() {
  Serial.begin(115200, SERIAL_8N1);
  Serial2.begin(9600, SERIAL_8N2, RX_PIN, TX_PIN);
  modbus.begin(Slave_ID, Serial2);
  modbus.preTransmission(preTransmission); 
  modbus.postTransmission(postTransmission); 
}

unsigned long lastMillis = 0;

void loop()  {
  long currentMillis = millis();
  if (currentMillis - lastMillis > 1000) {
    Serial.println("Slave_ID = " + (String)Slave_ID);
    
    uint8_t result = modbus.readInputRegisters(0x01, 2);
    if (getResultMsg(&modbus, result)) {
      Serial.println();
      double res_dbl = modbus.getResponseBuffer(0) / 10;
      String res = "Temperature: " + String(res_dbl) + " C\r\n";
      res_dbl = modbus.getResponseBuffer(1) / 10;
      res += "Humidity: " + String(res_dbl) + " %";
      Serial.println(res);
    }
    lastMillis = currentMillis;
  }
}

bool getResultMsg(ModbusMaster *node, uint8_t result) {
  String tmpstr2 = "\r\n";
  switch (result) {
    case node->ku8MBSuccess:
      return true;
      break;
    case node->ku8MBIllegalFunction:
      tmpstr2 += "Illegal Function";
      break;
    case node->ku8MBIllegalDataAddress:
      tmpstr2 += "Illegal Data Address";
      break;
    case node->ku8MBIllegalDataValue:
      tmpstr2 += "Illegal Data Value";
      break;
    case node->ku8MBSlaveDeviceFailure:
      tmpstr2 += "Slave Device Failure";
      break;
    case node->ku8MBInvalidSlaveID:
      tmpstr2 += "Invalid Slave ID";
      break;
    case node->ku8MBInvalidFunction:
      tmpstr2 += "Invalid Function";
      break;
    case node->ku8MBResponseTimedOut:
      tmpstr2 += "Response Timed Out";
      break;
    case node->ku8MBInvalidCRC:
      tmpstr2 += "Invalid CRC";
      break;
    default:
      tmpstr2 += "Unknown error: " + String(result);
      break;
  }
  Serial.println(tmpstr2);
  return false;
}