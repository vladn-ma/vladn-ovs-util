/*
  06/11/15 12:43
  vladn

  params.h 
  header file for the global parameters support
*/

#ifndef __progparams_h___DEFINED__
#define __progparams_h___DEFINED__

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

typedef struct params__ {
  char m_cmd[32];
  char m_pname[32]; /*program name*/
  char m_iname[32];  /*name of the interface*/
  bool m_smac_cmd;
  uint8_t m_smac[6];
  bool m_dmac_cmd;
  uint8_t m_dmac[6];
  bool m_sip_cmd;
  struct in_addr m_sip; 
  bool m_dip_cmd;
  struct in_addr m_dip;
} params;

/* standard class static functions */
params* params__alloc (void);
void params__test (int argc, char **argv);

/*standard class member funcitons*/
params *params__init (params* _p, int _argc, char **_argv);
params *params__destroy (params* _p);
void params__free (params* _p);
void params__info (params *_p);

/* class specific member functions */
void params__print_version (params *_p);
void params__print_help (params *_p);

#endif /* __progparams_h___DEFINED__ */

/* end of file params.h*/
