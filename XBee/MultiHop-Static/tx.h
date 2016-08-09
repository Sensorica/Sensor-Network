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



void send_packet(uint32_t address_MSB, uint32_t address_LSB) {   
  
  //XBeeAddress64 addr64;
  addr64 = XBeeAddress64(address_MSB, address_LSB);
  xbee.send(zbTx);

  // flash TX indicator
  Serial.print ("sending");
  Serial.print('\n');
  
  // after sending a tx request, we expect a status response
  // wait up to half second for the status response
  if (xbee.readPacket(500)) {
    // got a response!
	Serial.print ("got a response!");
	Serial.print('\n');

    // should be a znet tx status            	
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE) {
      xbee.getResponse().getZBTxStatusResponse(txStatus);

      // get the delivery status, the fifth byte
      if (txStatus.getDeliveryStatus() == SUCCESS) {
        // success.  time to celebrate
		Serial.print ("success.  time to celebrate");
		Serial.print('\n');
        
      } else {
        // the remote XBee did not receive our packet. is it powered on?
		Serial.print ("the remote XBee did not receive our packet. is it powered on?");
		Serial.print('\n');
        
      }
    }
  } else if (xbee.getResponse().isError()) {
		Serial.print ("Error reading packet.  Error code: ");
		Serial.print('\n');
		Serial.print (xbee.getResponse().getErrorCode());
		Serial.print('\n');
    
  } else {
    // local XBee did not provide a timely TX Status Response -- should not happen
	Serial.print ("local XBee did not provide a timely TX Status Response -- should not happen");
	Serial.print('\n');
    
  }

  delay(1000);
}
