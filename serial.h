#ifndef _SERIAL_H_
#define _SERIAL_H_


#define RECIEVE_BUFFER_SIZE 100


void send_packet(char* packet);
void serial_init(void);
void listen_port(void);
void receiveOLD(char* receivedPacket, int bufferLength, int* packetLength);
void receive(void);



#endif /*_SERIAL_H_*/
