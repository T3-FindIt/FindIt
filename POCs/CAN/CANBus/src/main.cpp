#include <SPI.h>
#include <mcp_can.h>

// Set SPI pins and MCP2515 CS pin
const int spiClock = 1000000;
const int spiMOSI = 23;
const int spiMISO = 19;
const int spiSCK = 18;
const int mcp2515CS = 5;

// Set CAN bus parameters
const int canId = 0x123;
const int canSpeed = CAN_500KBPS;

// Create MCP_CAN object
MCP_CAN can(mcp2515CS);

void setup() {
  Serial.begin(115200);

  // Initialize SPI interface
  SPI.begin(spiSCK, spiMISO, spiMOSI);
  SPI.setClockDivider(SPI_CLOCK_DIV4); // Set SPI clock frequency

  // Initialize MCP2515 module
  if(can.begin(canSpeed) == CAN_OK) {
    Serial.println("MCP2515 initialized.");
  } else {
    Serial.println("Failed to initialize MCP2515!");
    while(1);
  }
}

void loop() {
  // Create a CAN message with ID = canId
  CAN_MESSAGE canMsg;
  canMsg.can_id = canId;
  canMsg.can_dlc = 8;
  canMsg.flags.extended = 0;
  canMsg.flags.rtr = 0;
  canMsg.data[0] = 0xAA;
  canMsg.data[1] = 0xBB;
  canMsg.data[2] = 0xCC;
  canMsg.data[3] = 0xDD;
  canMsg.data[4] = 0xEE;
  canMsg.data[5] = 0xFF;
  canMsg.data[6] = 0x11;
  canMsg.data[7] = 0x22;

  // Send CAN message
  if(can.sendMsgBuf(canMsg.can_id, canMsg.flags.extended, canMsg.can_dlc, canMsg.data)) {
    Serial.println("CAN message sent successfully!");
  } else {
    Serial.println("Failed to send CAN message!");
  }

  delay(1000);
}
