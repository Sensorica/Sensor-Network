This codes receives data from a specific node (identified by PREVIOUS_NODE_LSB) and forwards the data to the next node (identified by NEXT_NODE_LSB)
The code uses the following files. This means that the route is statically identified (i.e. each node knows the next node towards the destination)

- rx.h : contains receiving functions
- tx.h : contains sending functions
- global.h: contains global variables and constants
- xbee_communication.ino: a sample Arduino sketch

For example:
if we have 3 nodes; x, y, & z. 
node x <--> node y <--> node z

If node x wants to send data to destination Z through node y, then the address settings should be:
- Node x: NEXT_NODE_LSB = address of node y 
- Node y: NEXT_NODE_LSB = address of destination z , PREVIOUS_NODE_LSB = address of node x
- Node z: PREVIOUS_NODE_LSB = address of node y
 

