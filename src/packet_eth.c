/*
  06/11/15 12:43
  vladn
  
  packet_eth.c
  implementation of packet 1 generator
  generates packet eth
*/

#include "packet_eth.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netinet/in.h>
#include <netinet/ether.h>

uint32_t packet_eth (
		  uint8_t *_buf,
		  uint32_t _len,
		  uint8_t *_smac,
		  uint8_t *_dmac
		  )
{
  struct ether_header *eh = (struct ether_header *) _buf;
  uint32_t ind = 0;
  int i = 0;

  /*validate arguments*/

  if (_buf == NULL || _smac == NULL || _dmac == NULL) {
     fprintf (stderr, "error: packet_eth() buf or _smac or _dmac argument is NULL\n");
     exit (21);
  }

  if (_len < (sizeof(struct ether_header) +  68)) {
      fprintf (stderr, "error: packet_eth() argument len is too short\n");
     exit (21);
  }

  /*clean*/
  memset(_buf, 0, _len);

  /*ethernet header*/
  for (i=0; i<sizeof (eh->ether_shost); i++) eh->ether_shost[i] = _smac[i];
  for (i=0; i<sizeof (eh->ether_dhost); i++) eh->ether_dhost[i] = _dmac[i];
  eh->ether_type = htons(ETH_P_802_2);	/*0x0004 802.2 frames*/
  ind += sizeof(struct ether_header);


  /*ethernet payload*/
  _buf[ind++] = 0xde;
  _buf[ind++] = 0xad;
  _buf[ind++] = 0xbe;
  _buf[ind++] = 0xef;
  for (i=0; i<64; i++) _buf[ind++] = i;

  return ind;
}

