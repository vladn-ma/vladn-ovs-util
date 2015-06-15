/*
  06/11/15 12:43
  vladn
  
  ethsr.c
  source file for the main ethsr program
*/

#include "ethsr.h"
#include "params.h"

int main (int argc, char **argv) 
{
  char *iname;
  int sockfd;
  struct ifreq if_idx;
  struct sockaddr_ll socket_address;
  unsigned char udp_payload [] = {0xde, 0xad, 0xbe, 0xef};

  params par;

  /* params__test (argc, argv); */
  /* exit (0); */

  params__init (&par, argc, argv);
  /* params__info (&par); */ 

  if (par.m_print_version == TRUE)
    params__print_version (&par, stdout);

  if (par.m_print_help == TRUE) {
    params__print_help (&par, stdout);
    exit (0);
  }

  printf ("\nethsr beg\n");

  iname = par.m_if_str;
  printf ("iname = \"%s\"\n", iname);

  /*open socket to send on*/
  if ((sockfd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ALL))) == -1) perror("socket");
  
  /*get index of the interface to send in*/
  memset(&if_idx, 0, sizeof(struct ifreq));
  strncpy(if_idx.ifr_name, iname, IFNAMSIZ-1);
  if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
    perror("SIOCGIFINDEX");
    exit (1);
  }
  printf("if_idx = %d\n", if_idx.ifr_ifindex);

  /*fill out socket address structure*/
  socket_address.sll_family   = AF_PACKET;
  socket_address.sll_halen    = ETH_ALEN;
  socket_address.sll_ifindex  = if_idx.ifr_ifindex;   
  socket_address.sll_protocol = htons(ETH_P_ALL);
  socket_address.sll_hatype   = 0;
  socket_address.sll_pkttype  = 0;
 
  /*send packet */
  if (sendto(sockfd, udp_payload, sizeof (udp_payload), 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
    printf("Send failed\n");
    exit (4);
  }

  printf ("\nethsr end\n");
  return 0;
}

