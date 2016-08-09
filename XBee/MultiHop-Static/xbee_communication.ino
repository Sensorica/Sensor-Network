/**
 * Copyright (c) 2009 Andrew Rapp. All rights reserved.
 *
 * This file is part of XBee-Arduino.
 *
 * XBee-Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XBee-Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XBee-Arduino.  If not, see <http://www.gnu.org/licenses/>.
 */
 #include <XBee.h>
 #include "global.h"
 #include "rx.h"
 #include "tx.h"

void setup() {
  
  // start serial
  Serial.begin(9600);
  Serial1.begin(9600);
  xbee.setSerial(Serial1);
  
}


void loop() {
	uint8_t received;
    // continuously reads packets, looking for ZB Receive or Modem Status packet
	
	received = get_packet();
	if(received == 1)
  {
    Serial.print("main_loop printing");
	Serial.print(data[0]);
    Serial.print('\n');
    Serial.print(data[1]);
    Serial.print('\n');
	// sending the reply
	payload[0]= data[0];
	payload[1]= data[1];
	if(rx_address.getLsb()== uint32_t (PREVIOUS_NODE_LSB))
	{	
		send_packet(0x0013a200, uint32_t (NEXT_NODE_LSB) );
	}
    delay(1000);
  }
	
 
 
    
    
}
