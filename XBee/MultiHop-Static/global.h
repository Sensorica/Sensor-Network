//@@@@@@@@@@@@@@@@@@@@@ Constants @@@@@@@@@@@@@@
#define PREVIOUS_NODE_LSB 0x41565FA6
#define NEXT_NODE_LSB 0x41565FA6

//@@@@@@@@@@@@@@@@@@@@@ General Variables @@@@@@@@@@@@@@
XBee xbee = XBee();

//@@@@@@@@@@@@@@@@@ Transmitting Variables @@@@@@@@@@@@@@@@@@@@@
uint8_t payload[] = { 0, 0 , 0 , 0, 0};

// SH + SL Address of receiving XBee
//XBeeAddress64 addr64 = XBeeAddress64(0x0013a200, 0x41565FA6);
XBeeAddress64 addr64;
ZBTxRequest zbTx = ZBTxRequest(addr64, payload, sizeof(payload));
ZBTxStatusResponse txStatus = ZBTxStatusResponse();


//@@@@@@@@@@@@@@@@  Receiving variables @@@@@@@@@@@@@@
uint8_t data[10];
XBeeAddress64 rx_address;
XBeeResponse response = XBeeResponse();
// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
