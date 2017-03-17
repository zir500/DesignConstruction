#ifndef _SERIAL_H_
#define _SERIAL_H_

void send_packet(char* packet, char length);
void serial_init(void);
void listen_port(void);

#endif /*_SERIAL_H_*/
