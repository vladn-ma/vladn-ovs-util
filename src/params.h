/*
  06/11/15 12:43
  vladn

  params.h 
  header file for the global parameters support
*/

#ifndef __progparams_h___DEFINED__
#define __progparams_h___DEFINED__

#include <assert.h>

#define BOOL int
#define TRUE 1
#define FALSE 0
#define OK 1

typedef struct params__ {
  BOOL m_print_version, m_print_help;

  char
  m_pname[256], /*program name*/
  m_if_str[256],/*name of the interface*/
  unsigned char m_dmac[6] /*string with mac address of destanation to send*/
  ;

} params;

/* standard class static functions */
params* params__alloc (void);
int params__test (int argc, char **argv);

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
