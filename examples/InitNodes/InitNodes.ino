/*
   Copyright (c) 2019 Boot&Work Corp., S.L. All rights reserved
*/

#include "DALI.h"

// DALI interface
DALI dali(18, 19);

// List of addresses to assign to the connected DALI devices
const uint8_t addresses[] = { 10, 7, 18, 34 };

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600UL);
  dali.begin();

  Serial.println("Initialize DALI nodes...");
  int numAddresses = dali.initNodes(addresses, 64);

  Serial.println("Found nodes:");
  for (int i = 0; i < numAddresses; ++i) {
    Serial.print("Node number: ");
    Serial.print(i, DEC);
    Serial.print(", node address: ");
    Serial.println(addresses[i], HEX);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  // Nothing to do
}
