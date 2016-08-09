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
 
// continuously reads packets, looking for ZB Receive or Modem Status
	uint8_t get_packet() {
    
	
	xbee.readPacket();
    
    if (xbee.getResponse().isAvailable()) {
      // got something
      Serial.write("got something\n");
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
        // got a zb rx packet
        Serial.write("got a zb rx packet\n");  
        // now fill our zb rx class
        xbee.getResponse().getZBRxResponse(rx);
            
        if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
            // the sender got an ACK
            Serial.print ("the sender got an ACK \n");
        } else {
            // we got it (obviously) but sender didn't get an ACK
            Serial.print ("we got it (obviously) but sender didn't get an ACK  \n");
        }
        // set dataLed PWM to value of the first byte in the data
        Serial.print ("data = ");
        Serial.print ('\n');
        Serial.print (rx.getData(0));
        Serial.print ('\n');
        Serial.print (rx.getData(1));
        Serial.print ('\n');
        //adding received data to the global array data
		data[0]= rx.getData(0);
		data[1]= rx.getData(1);
		rx_address = rx.getRemoteAddress64();
	   return(1);
        
      } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
        xbee.getResponse().getModemStatusResponse(msr);
        // the local XBee sends this response on certain events, like association/dissociation
        
        if (msr.getStatus() == ASSOCIATED) {
          // yay this is great.  flash led
          Serial.print("yay this is great.  flash led");
		  Serial.print('\n');
		  
        } else if (msr.getStatus() == DISASSOCIATED) {
          // this is awful.. flash led to show our discontent
          Serial.print("this is awful.. flash led to show our discontent");
		  Serial.print('\n');
        } else {
          // another status
          Serial.print("another status");
		  Serial.print('\n');
        }
      } else {
        // not something we were expecting
		Serial.print("not something we were expecting");
		Serial.print('\n');        
      }
    } else if (xbee.getResponse().isError()) {
      Serial.print("Error reading packet.  Error code: ");  
	  Serial.print(xbee.getResponse().getErrorCode());
    }
	Serial.print("nothing received\n");
    return (0);
}
