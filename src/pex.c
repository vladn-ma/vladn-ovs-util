/*
  06/11/15 12:43
  vladn
  
  pex.c
  source file for the main ethsr program
*/

#include "pex.h"
#include "params.h"
#include "send_wire.h"

#include "stdio.h"
#include "string.h"

int main (int argc, char **argv) 
{  
  params par;

  params__init (&par, argc, argv);
  /* params__info (&par); */
  printf ("\nethsr beg\n");

  if (!strncmp (par.m_cmd, "help", strlen ("help"))) {
    params__print_version (&par);
    params__print_help (&par);
  } else if (!strcmp (par.m_cmd, "send_wire")) {
    send_wire (&par);
  }

  printf ("\nethsr end\n");
  return 0;
}

