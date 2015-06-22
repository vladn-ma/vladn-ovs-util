/*
  06/19/15 9:04
  vladn

  packet_eth.h 
  header file for packet_eth generator
  generate packet eth
*/

#ifndef __packet_eth_h__DEFINED__
#define __packet_eth_h__DEFINED__

#include <stdint.h>

uint32_t packet_eth (
		  uint8_t *_buf,
		  uint32_t _len,
		  uint8_t *_smac,
		  uint8_t *_dmac
		  );

#endif /*__packet_eth_h__DEFINED__*/
