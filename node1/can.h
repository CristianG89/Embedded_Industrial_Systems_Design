#ifndef CAN_H
#define CAN_H

#define TXREQ 3
#define TXERR 4
#define MLOA 5


//CAN priority ID list
#define CAN_SCORE_ID 		0x01
#define CAN_END_GAME_ID		0x02
#define CAN_START_GAME_ID 	0x03
#define CAN_INPUT_ID 		0x04
#define CAN_SHOOT_ID		0x05


//Format of the CAN packages
typedef struct {
	uint8_t id;
	uint8_t length;	
	uint8_t data[8];
} packet;


/****************************************************************************
CAN module (MCP2515) initialization
****************************************************************************/
void CAN_init();


/****************************************************************************
Paramater message is sent as a new package through CAN communication
****************************************************************************/
void CAN_send(packet* message);


/****************************************************************************
Reads an incoming package through CAN communication of a type struct packet
****************************************************************************/
packet CAN_read();


/****************************************************************************
Reports any error in CAN communication
****************************************************************************/
int CAN_error();


/****************************************************************************
Use interrupt function to tell if recieved a message or not by assigning 
1 or 0 to flag variabel
****************************************************************************/
uint8_t CAN_message_recieved();

#endif // CAN_H