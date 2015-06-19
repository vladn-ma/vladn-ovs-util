/*
  06/19/15 9:04
  vladn

  packet_udp.h 
  header file for packet_udp generator
  generate packet eth+ip+udp
*/

#ifndef __packet_udp_h__DEFINED__
#define __packet_udp_h__DEFINED__

#include <stdint.h>

uint32_t packet_udp (
		  uint8_t *_buf,
		  uint32_t _len,
		  uint8_t *_smac,
		  uint8_t *_dmac,
		  uint32_t _sip,
		  uint32_t _dip
		  );

#endif /*__packet_udp_h__DEFINED__*/
