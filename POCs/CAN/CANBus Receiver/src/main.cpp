#include <mcp_can.h>
#include <SPI.h>

// Set CS pin of MCP2515 module
const int CS_PIN = 10;

MCP_CAN CAN(CS_PIN);  // Initialize MCP_CAN object

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if(CAN.begin(MCP_ANY, CAN_125KBPS, MCP_16MHZ) == CAN_OK) {
    Serial.println("MCP2515 Initialized Successfully!");
  } else {
    Serial.println("Error Initializing MCP2515...");
  }

  // Enable receiving on CAN bus
  CAN.setMode(MCP_NORMAL);
}

void loop() {
  if(CAN_MSGAVAIL == CAN.checkReceive()) { // Check for new messages
    CAN.readMsgBuf(&len, buf);  // Read message into buffer

    Serial.print("ID: ");
    Serial.print((buf[0]<<3)+(buf[1]>>5),HEX);  // Extract message ID
    Serial.print(" Data: ");
    for(int i = 0; i<len-1; i++) { // Print message data
      Serial.print(buf[i+2],HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}
