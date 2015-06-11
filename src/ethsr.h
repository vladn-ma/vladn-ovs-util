/*
  06/11/15 12:43
  vladn

  ethsr.h 
  header file for the main ethsr program
  
  ethsr program send and receive file to the interface both given by command line
*/

#ifndef ethsr_h__DEFINED
#define ethsr_h__DEFINED

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <netinet/ether.h>
unsigned short csum(unsigned short *buf, int nwords);

#endif /*ethsr_h__DEFINED*/
