/*
  06/19/15 9:04
  vladn

  packet1.h 
  header file for packet1 generator
  generate packet eth+ip+udp
*/

#ifndef __packet1_h__DEFINED__
#define __packet1_h__DEFINED__

#include <stdint.h>

uint32_t packet1 (
		  uint8_t *_buf,
		  uint32_t _len,
		  uint8_t *_smac,
		  uint8_t *_dmac,
		  uint32_t _sip,
		  uint32_t _dip
		  );

#endif /*__packet1_h__DEFINED__*/
