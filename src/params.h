/*
  06/11/15 12:43
  vladn

  params.h 
  header file for the global parameters support
*/

#ifndef __progparams_h___DEFINED__
#define __progparams_h___DEFINED__

#include <assert.h>
#include <stdio.h>

#define BOOL int
#define TRUE 1
#define FALSE 0
#define OK 1

typedef struct params__ {
  BOOL m_print_version; /*need to print out program version*/
  BOOL m_print_help; /*need to print out program help*/
  char m_pname[256]; /*program name*/
  char m_if_str[256];/*name of the interface*/
  char m_dmac_str[256];/*string with mac address of destanation to send*/
  unsigned char m_dmac[6]; /*dmac as an array*/
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
void params__print_version (params *_p, FILE *_fh);
void params__print_help (params *_p, FILE *_fh);

#endif /* __progparams_h___DEFINED__ */

/* end of file params.h*/
