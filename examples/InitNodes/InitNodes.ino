/*
   Copyright (c) 2019 Boot&Work Corp., S.L. All rights reserved
*/

#include <DALI.h>

DALI dali(2, 3);

////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600UL);

  dali.begin();
  Serial.println("Initialize DALI nodes...");
  dali.initNodes();

  Serial.println("Search DALI nodes...");
  uint32_t addresses[64];
  int numAddresses = dali.searchNodes(addresses, 64);

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
