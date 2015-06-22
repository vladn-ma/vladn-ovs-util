/*
  06/11/15 12:43
  vladn
  
  pex.c
  source file for the main ethsr program
*/
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

#include "packet_eth.h"
#include "packet_udp.h"
#include "send_kernel.h"

void send_kernel (params *_par) 
{
  int i;
  int sockfd;
  struct ifreq if_idx;
  struct ifreq if_mac;
  struct ifreq if_ip;
  uint32_t tx_len = 0;
  uint8_t sendbuf[1024];
  struct sockaddr_ll socket_address;

  printf ("\nsend_kernel beg\n");
  /* printf ("\ncmd = %s\n", _par->m_cmd); */

  /*open RAW socket to send on*/
  if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
    perror("socket");
    exit (11);
  }
  
  /*index of the interface to send in*/
  if (_par->m_inum_cmd != true) {
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, _par->m_iname, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
      perror("SIOCGIFINDEX");
      exit (12);
    }
    _par->m_inum = if_idx.ifr_ifindex;
  }
  printf ("src interface index = %d\n", _par->m_inum);
   
  /*MAC address of the interface to send on*/
  if (_par->m_smac_cmd != true) {
    memset(&if_mac, 0, sizeof(struct ifreq));
    strncpy(if_mac.ifr_name, _par->m_iname, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
      perror("SIOCGIFHWADDR");
      exit (13);
    }
    for (i=0; i<sizeof (_par->m_smac); i++) _par->m_smac[i] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[i];
  }
  printf ("src interface mac = ");
  for (i=0; i< sizeof (_par->m_smac); i++) printf ("%s%x", (i?":":""), _par->m_smac[i]);
  printf ("\n");

  /*IP address of the interface to send on*/
  if (_par->m_sip_cmd != true && strcmp(_par->m_protocol, "eth")) {
    memset(&if_ip, 0, sizeof(struct ifreq));
    strncpy(if_ip.ifr_name, _par->m_iname, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFADDR, &if_ip) < 0) {
      perror("SIOCGIFADDR");
      /* if we cannot get ip address, it is not an error: it maybe not set for the interface
	 exit (14);*/
    }
    _par->m_sip = ((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr;
  }
  printf ("src interface ip = %s\n", inet_ntoa(_par->m_sip));

  /*generate packet*/
  if (!strcmp (_par->m_protocol, "eth")) {
    tx_len = packet_eth (sendbuf, sizeof (sendbuf), _par->m_smac, _par->m_dmac);
  } else if  (!strcmp (_par->m_protocol, "udp")) {
    tx_len = packet_udp (sendbuf, sizeof (sendbuf), _par->m_smac, _par->m_dmac,
			 (uint32_t)_par->m_sip.s_addr, (uint32_t)_par->m_dip.s_addr);
  }
    
  /*send the raw Ethernet packet*/
  socket_address.sll_ifindex = _par->m_inum;
  socket_address.sll_halen = ETH_ALEN;
  for (i=0; i< sizeof (_par->m_dmac); i++) socket_address.sll_addr[i] = _par->m_dmac[i];

  /*print last message before sending*/
  printf ("\nsending packet with following parameters");
  params__info (_par);

  /*send packet */
  if (sendto (sockfd, sendbuf, tx_len, 0,
	      (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
    printf("Send failed\n");
    exit (15);
  } else {
    printf ("\nsend OK");
  }

  printf ("\n\nsend_kernel end\n");
}

