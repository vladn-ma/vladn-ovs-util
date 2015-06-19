/*
  06/11/15 12:43
  vladn
  
  packet1.c
  implementation of packet 1 generator
  generates packet eth+ip+udp
*/

#include "packet1.h"

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

static uint16_t csum(uint16_t *buf, uint32_t nwords);

uint32_t packet1 (
		  uint8_t *_buf,
		  uint32_t _len,
		  uint8_t *_smac,
		  uint8_t *_dmac,
		  uint32_t _sip,
		  uint32_t _dip
		  )
{
  struct ether_header *eh = (struct ether_header *) _buf;
  struct iphdr *iph = (struct iphdr *) (_buf + sizeof(struct ether_header));
  struct udphdr *udph = (struct udphdr *) (_buf + sizeof(struct iphdr) + sizeof(struct ether_header));
  uint8_t ttl = 60;
  uint32_t ind = 0;

  /*validate arguments*/

  if (_buf == NULL || _smac == NULL || _dmac == NULL) {
     fprintf (stderr, "error: packet1() buf or _smac or _dmac argument is NULL\n");
     exit (21);
  }

  if (_len < (sizeof(struct ether_header) +  sizeof(struct iphdr) +  sizeof(struct udphdr) + 4)) {
      fprintf (stderr, "error: packet1() argument len is too short\n");
     exit (21);
  }

  /*clean*/
  memset(_buf, 0, _len);

  /*ethernet header*/
  eh->ether_shost[0] = _smac[0];
  eh->ether_shost[1] = _smac[1];
  eh->ether_shost[2] = _smac[2];
  eh->ether_shost[3] = _smac[3];
  eh->ether_shost[4] = _smac[4];
  eh->ether_shost[5] = _smac[5];
  eh->ether_dhost[0] = _dmac[0];
  eh->ether_dhost[1] = _dmac[1];
  eh->ether_dhost[2] = _dmac[2];
  eh->ether_dhost[3] = _dmac[3];
  eh->ether_dhost[4] = _dmac[4];
  eh->ether_dhost[5] = _dmac[5];
  eh->ether_type = htons(ETH_P_IP);
  ind += sizeof(struct ether_header);

  /*IP Header*/
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 16; // Low delay
  iph->id = htons(54321);
  iph->ttl = ttl; // hops
  iph->protocol = 17; // UDP
  iph->saddr = _sip;
  iph->daddr = _dip;
  ind += sizeof(struct iphdr);

  /*UDP Header*/
  udph->source = htons(33423);
  udph->dest = htons(35342);
  udph->check = 0; /*skip*/
  ind += sizeof(struct udphdr);

  /*UDP payload*/
  _buf[ind++] = 0xde;
  _buf[ind++] = 0xad;
  _buf[ind++] = 0xbe;
  _buf[ind++] = 0xef;
  /* for (i=0; i<64; i++) buf[tx_len++] = i; */

  /*again UDP header */
  udph->len = htons(ind - sizeof(struct ether_header) - sizeof(struct iphdr));
  
  /*again IP header */
  iph->tot_len = htons(ind - sizeof(struct ether_header));
  iph->check = csum((uint16_t *)(_buf+sizeof(struct ether_header)), (uint16_t)sizeof(struct iphdr));

  return ind;
}

uint16_t csum(uint16_t *_buf, uint32_t _nwords)
{
    unsigned long sum;
    for(sum=0; _nwords>0; _nwords--) sum += *_buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (uint16_t)(~sum);
}

