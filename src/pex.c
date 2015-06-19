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

#include "pex.h"
#include "params.h"
#include "packet1.h"

#include <netinet/ether.h>

int main (int argc, char **argv) 
{
  int i;
  int sockfd;
  struct ifreq if_idx;
  struct ifreq if_mac;
  struct ifreq if_ip;
  uint32_t tx_len = 0;
  uint8_t sendbuf[1024];
  struct sockaddr_ll socket_address;
  params par;

  params__test (argc, argv);
  exit (0);

  params__init (&par, argc, argv);
  /* params__info (&par); */ 

  if (!strncmp (par.m_cmd, "help", strlen ("help"))) {
    params__print_version (&par);
    params__print_help (&par);
    exit (0);
  }

  printf ("\nethsr beg\n");

  if (!strcmp (par.m_cmd, "send_wire")) {
    printf ("\ncmd = %s\n", par.m_cmd);

    /*open RAW socket to send on*/
    if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) perror("socket");
  
    /*get index of the interface to send in*/
    memset(&if_idx, 0, sizeof(struct ifreq));
    strncpy(if_idx.ifr_name, par.m_iname, IFNAMSIZ-1);
    if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
      perror("SIOCGIFINDEX");
      exit (1);
    }
    printf ("src interface index = %d\n", if_idx.ifr_ifindex);

    if (par.m_smac_cmd != true) {
      /*get the MAC address of the interface to send on*/
      memset(&if_mac, 0, sizeof(struct ifreq));
      strncpy(if_mac.ifr_name, par.m_iname, IFNAMSIZ-1);
      if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
	perror("SIOCGIFHWADDR");
	exit (2);
      }
      for (i=0; i<sizeof (par.m_smac); i++) par.m_smac[i] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[i];
    }
    printf ("src interface mac = ");
    for (i=0; i<4; i++) printf ("%s%x", (i?":":""), par.m_smac[i]);
    printf ("\n");


    if (par.m_sip_cmd != true) {
      /*get the IP address of the interface to send on*/
      memset(&if_ip, 0, sizeof(struct ifreq));
      strncpy(if_ip.ifr_name, par.m_iname, IFNAMSIZ-1);
      if (ioctl(sockfd, SIOCGIFADDR, &if_ip) < 0) {
	perror("SIOCGIFADDR");
	exit (3);
      }
      par.m_sip = ((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr;
    }
    printf ("src interface ip = %s\n", inet_ntoa(par.m_sip));

    /*generate packet*/
    tx_len = packet1 (sendbuf, sizeof (sendbuf), par.m_smac, par.m_dmac, (uint32_t)par.m_sip.s_addr, (uint32_t)par.m_dip.s_addr);


    /*send the raw Ethernet packet*/
    socket_address.sll_ifindex = if_idx.ifr_ifindex;
    socket_address.sll_halen = ETH_ALEN;
    for (i=0; i< sizeof (par.m_dmac); i++) socket_address.sll_addr[i] = par.m_dmac[i];

    /*send packet */
    if (sendto(
	       sockfd,
	       sendbuf,
	       tx_len,
	       0,
	       (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)
	       ) < 0) {
      printf("Send failed\n");
      exit (4);
    }

  }

  printf ("\nethsr end\n");
  return 0;
}

