#ifndef _SERIAL_H_
#define _SERIAL_H_


#define MAX_PACKET_SIZE 100

struct packetNode{
	char packetContents[MAX_PACKET_SIZE];
	int packetLength;
	struct packetNode *next;	
} ;



void send_packet(char* packet, char length);
void serial_init(void);
void listen_port(void);
void receive(char* receivedPacket, int bufferLength, int* packetLength);


#endif /*_SERIAL_H_*/
