/*
  06/11/15 12:43
  vladn
  
  ethsr.c
  source file for the main ethsr program
*/

#include "ethsr.h"

int main (int argc, char **argv) 
{
  char *iname = "tap0";
  int ttl = 60;
  int sockfd;
  struct ifreq if_idx;
  struct ifreq if_mac;
  struct ifreq d_mac;
  struct ifreq if_ip;
  int tx_len = 0;
  char sendbuf[1024];
  struct ether_header *eh = (struct ether_header *) sendbuf;
  struct iphdr *iph = (struct iphdr *) (sendbuf + sizeof(struct ether_header));
  struct udphdr *udph = (struct udphdr *) (sendbuf + sizeof(struct iphdr) + sizeof(struct ether_header));
  struct sockaddr_ll socket_address;

  /*init destination address for tap0
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0] = 0x66;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[1] = 0x4a;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[2] = 0xb8;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[3] = 0x50;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[4] = 0x0d;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[5] = 0xf9;*/

  /*init destination address for tap1*/
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0] = 0xce;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[1] = 0xc3;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[2] = 0x25;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[3] = 0x83;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[4] = 0x3b;
  ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[5] = 0x36;


  printf ("\nethsr beg\n");

  /*open RAW socket to send on*/
  if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) perror("socket");
  
  /*get index of the interface to send in*/
  memset(&if_idx, 0, sizeof(struct ifreq));
  strncpy(if_idx.ifr_name, iname, IFNAMSIZ-1);
  if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0) {
    perror("SIOCGIFINDEX");
     exit (1);
  }

  /*get the MAC address of the interface to send on*/
  memset(&if_mac, 0, sizeof(struct ifreq));
  strncpy(if_mac.ifr_name, iname, IFNAMSIZ-1);
  if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0) {
    perror("SIOCGIFHWADDR");
    exit (2);
  }

  /*get the IP address of the interface to send on*/
  memset(&if_ip, 0, sizeof(struct ifreq));
  strncpy(if_ip.ifr_name, iname, IFNAMSIZ-1);
  if (ioctl(sockfd, SIOCGIFADDR, &if_ip) < 0) {
    perror("SIOCGIFADDR");
    exit (3);
  }

  /*construct the ethernet header*/
  memset(sendbuf, 0, 1024);
  /*ethernet header*/
  eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
  eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
  eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
  eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
  eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
  eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
  eh->ether_dhost[0] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  eh->ether_dhost[1] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[1];
  eh->ether_dhost[2] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[2];
  eh->ether_dhost[3] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[3];
  eh->ether_dhost[4] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[4];
  eh->ether_dhost[5] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[5];
  eh->ether_type = htons(ETH_P_IP);
  tx_len += sizeof(struct ether_header);

  /*construct the IP header*/
  /*IP Header*/
  iph->ihl = 5;
  iph->version = 4;
  iph->tos = 16; // Low delay
  iph->id = htons(54321);
  iph->ttl = ttl; // hops
  iph->protocol = 17; // UDP
  /*source IP address, can be spoofed*/
  iph->saddr = inet_addr(inet_ntoa(((struct sockaddr_in *)&if_ip.ifr_addr)->sin_addr));
  /* iph->saddr = inet_addr("192.168.0.112");*/
  /* Destination IP address */
  iph->daddr = inet_addr("192.168.0.111");
  tx_len += sizeof(struct iphdr);

  /*construct the UDP header*/
  /*UDP Header*/
  udph->source = htons(3423);
  udph->dest = htons(5342);
  udph->check = 0; /*skip*/
  tx_len += sizeof(struct udphdr);

  /*fill in UDP payload*/
  /*packet data*/
  sendbuf[tx_len++] = 0xde;
  sendbuf[tx_len++] = 0xad;
  sendbuf[tx_len++] = 0xbe;
  sendbuf[tx_len++] = 0xef;

  /*fill in remaining header info*/
  /* Length of UDP payload and header */
  udph->len = htons(tx_len - sizeof(struct ether_header) - sizeof(struct iphdr));
  /* Length of IP payload and header */
  iph->tot_len = htons(tx_len - sizeof(struct ether_header));
  /* Calculate IP checksum on completed header */
  iph->check = csum((unsigned short *)(sendbuf+sizeof(struct ether_header)), sizeof(struct iphdr)/2);

  /*send the raw Ethernet packet*/
  /*index of the network device */
  socket_address.sll_ifindex = if_idx.ifr_ifindex;
  /*address length*/
  socket_address.sll_halen = ETH_ALEN;
  /*destination MAC */
  socket_address.sll_addr[0] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  socket_address.sll_addr[1] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  socket_address.sll_addr[2] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  socket_address.sll_addr[3] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  socket_address.sll_addr[4] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];
  socket_address.sll_addr[5] = ((uint8_t *)&d_mac.ifr_hwaddr.sa_data)[0];

  /*send packet */
  if (sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll)) < 0) {
    printf("Send failed\n");
    exit (4);
  }

  printf ("\nethsr end\n");
  return 0;
}

unsigned short csum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--) sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
