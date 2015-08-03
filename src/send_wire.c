/*
  06/11/22 12:47
  vladn
  
  pexsend_wire.c
  implementaiton of the send_wire support
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <net/if.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/if_tun.h>
#include <unistd.h>

#include "params.h"
#include "send_wire.h"
#include "packet_eth.h"
#include "packet_udp.h"

/*for the struct sockaddr_ll*/
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h> /* the L2 protocols */


void send_wire (params *_par)
{
  /* int i; */
  int tap_fd;
  struct ifreq ifr;
  char *clonedev = "/dev/net/tun";
  uint32_t tx_len = 0;
  uint8_t sendbuf[1024];
  /* struct sockaddr_ll socket_address; */
   
  printf ("\nsend_wire beg\n");
  /* printf ("\ncmd = %s\n", _par->m_cmd); */
 
  /*allocates or reconnects to a tun/tap device*/ 
  if( (tap_fd = open(clonedev , O_RDWR)) < 0 ) {
    perror("Opening /dev/net/tun");
    exit (51);
  }

  memset(&ifr, 0, sizeof(ifr));
  ifr.ifr_flags =  IFF_TAP | IFF_NO_PI /* | IFF_MULTI_QUEUE */;
  strncpy(ifr.ifr_name, _par->m_iname, IFNAMSIZ);

  if( ioctl(tap_fd, TUNSETIFF, (void *)&ifr) < 0 ) {
    perror("ioctl(TUNSETIFF)");
    close(tap_fd);
    exit(52);
  }

  /*set interface persistant*/
  /* if(ioctl(tap_fd, TUNSETPERSIST, 0) < 0) { */
  /*   perror("TUNSETPERSIST"); */
  /*   close(tap_fd); */
  /*   exit (53); */
  /* }  */

  strcpy(_par->m_iname, ifr.ifr_name);
  printf("successfully connected to interface %s\n", _par->m_iname);

  /*generate packet*/
  if (!strcmp (_par->m_protocol, "eth")) {
    tx_len = packet_eth (sendbuf, sizeof (sendbuf), _par->m_smac, _par->m_dmac);
  } else if  (!strcmp (_par->m_protocol, "udp")) {
    tx_len = packet_udp (sendbuf, sizeof (sendbuf), _par->m_smac, _par->m_dmac,
			 (uint32_t)_par->m_sip.s_addr, (uint32_t)_par->m_dip.s_addr);
  }

  /*print  message before sending*/
  printf ("\nsending packet with following parameters");
  params__info (_par);

  if (write(tap_fd, sendbuf, tx_len) < tx_len) {
    perror("error of write to the tap");
    close(tap_fd);
    exit(54);
  }else {
    printf ("\nsend OK");
  }

  printf ("\n\nsend_wire end\n");  
}

