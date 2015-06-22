/*
  06/11/15 12:43
  vladn
  
  params.c
  source file for the global parameters support
*/

#include <getopt.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <libgen.h>

#include "params.h"

params* params__alloc (void)
{
  params* _p = NULL;
  _p = (params*) calloc (sizeof (params), 1);
  assert (_p != NULL);
  return _p;
}

/*
 * init params from the command line arguments list
 * if argument is not in the command list, then init default value
 */
params *params__init (params* _p, int _argc, char **_argv)
{
  struct option long_options[] = {
    {"help",      no_argument,       0, 'h'},
    {"cmd",       required_argument, 0, 'c'},    
    {"protocol",  required_argument, 0, 'p'},    
    {"iname",     required_argument, 0, 'e'},
    {"inum",      required_argument, 0, 'n'},
    {"smac",      required_argument, 0, 's'},
    {"dmac",      required_argument, 0, 'd'},
    {"sip",       required_argument, 0, 'i'},
    {"dip",       required_argument, 0, 'j'},
    {0,           0,           0, 0  }
  };
  char *optstring = "vhc:p:e:n:s:d:i:j:";
  
  int c, i, t[6];

  assert (_p!=NULL);
  assert (_argc != 0);
  assert (_argv != NULL);

  memset(_p, 0, sizeof(*_p));

  strncpy (_p->m_pname, basename (_argv[0]), sizeof (_p->m_pname));
  
  strncpy (_p->m_cmd,      "help", sizeof (_p->m_cmd));  
  strncpy (_p->m_protocol, "eth",  sizeof (_p->m_protocol));  
  strncpy (_p->m_iname,    "eth0", sizeof (_p->m_iname));
  _p->m_smac_cmd = false;
  _p->m_iname_cmd = false;
  _p->m_inum_cmd = false;
  _p->m_dmac_cmd = false;
  _p->m_sip_cmd = false;
  _p->m_dip_cmd = false;

  while(1) {
    c = getopt_long(_argc, _argv, optstring, long_options, NULL);
    if (c < 0) break;

    /*printf("\n--- c = %d (%c); optarg = %s", c, c, optarg);*/
    switch (c) {
    case 'h': strncpy (_p->m_cmd, "help", sizeof (_p->m_cmd)); break;
    case 'c': strncpy (_p->m_cmd, optarg, sizeof (_p->m_cmd)); break;
    case 'p': strncpy (_p->m_protocol, optarg, sizeof (_p->m_protocol)); break;
    case 'e': strncpy (_p->m_iname, optarg, sizeof (_p->m_iname)); _p->m_iname_cmd = true; break;
    case 'n': _p->m_inum = atoi (optarg); _p->m_inum_cmd = true; break;
    case 's':
      if (sscanf(optarg, "%02x:%02x:%02x:%02x:%02x:%02x", &t[0], &t[1], &t[2], &t[3], &t[4], &t[5]) != 6) {
	perror("error: smac format");
	exit (31);
      }
      _p->m_smac_cmd = true;
      for (i=0; i<sizeof (_p->m_smac); i++) _p->m_smac[i] = t[i];
      break;
    case 'd':
      if (sscanf(optarg, "%02x:%02x:%02x:%02x:%02x:%02x", &t[0], &t[1], &t[2], &t[3], &t[4], &t[5]) != 6) {
	perror("error: dmac format");
	exit (32);
      } 
      _p->m_dmac_cmd = true;
      for (i=0; i<sizeof (_p->m_dmac); i++) _p->m_dmac[i] = t[i];
      break;
    case 'i':
      if (inet_aton(optarg, &_p->m_sip) == 0) {
	perror("error: sip format");
	exit (33);
      } else {
	_p->m_sip_cmd = true;
      }
      break;
    case 'j':
      if (inet_aton(optarg, &_p->m_dip) == 0) {
	perror("error: dip format");
	exit (34);
      } 
      _p->m_dip_cmd = true;
      break;
    default:  break;
    }
  }

  if (optind < _argc) {
    printf("\nnon-option ARGV-elements: ");
    while (optind < _argc)
      printf("%s ", _argv[optind++]);
    printf("\n");
  }

  return _p;
}

params *params__destroy (params* _p)
{
  assert (_p!=NULL);
  return _p;
}

void params__free (params* _p)
{
  assert (_p!=NULL);
  return ;
}

void params__print_help (params *_p)
{
  assert (_p!=NULL);
  assert (_p->m_pname != NULL);

  printf ("\n%s version = 1.1.1\n", _p->m_pname);
  printf ("\n port explorer: can send and receive packets to the port interfaces from both ends");
  printf ("\nusage:");
  printf ("\n  %s [options]", _p->m_pname);
  printf ("\noptions:");
  printf ("\n  -h || --help                /*print short help then exit*/");
  printf ("\n  -c || --cmd                 /*command: help, send_kernel, send_wire, default: help*/");
  printf ("\n  -p || --protocol            /*protocol: eth, udp;  default: eth*/");
  printf ("\n  -e || --iname INAME         /*name of interface from which to send packet, default is \"eth0\"*/");
  printf ("\n  -n || --iname INUM          /*number of interface from which to send packet, default is \"2\" (usualy it is eth0)*/");
  printf ("\n  -s || --smac NN:NN:NN:NN:NN /*source mac address, default: will try to get it from intervace*/");
  printf ("\n  -d || --dmac NN:NN:NN:NN:NN /*destanation mac address, default is \"00:00:00:00:00:00\"*/");
  printf ("\n  -i || --sip M.M.M.M         /*source ip address, default: will try to get it from intervace*/");
  printf ("\n  -j || --dip M.M.M.M         /*destanation ip address, default is \"0.0.0.0\"*/");
  printf ("\n  if no arguments, then default values are used");
  printf ("\n  if inum is given in command line, iname or smac,sip should be given too");
  printf ("\n");
}

void params__info (params *_p)
{
  assert (_p!=NULL);

  printf ("\n_p->m_cmd      = %s", _p->m_cmd);
  printf ("\n_p->m_protocol = %s", _p->m_protocol);
  printf ("\n_p->m_pname    = %s", _p->m_pname);
  printf ("\n_p->m_iname_cmd= %s", ((_p->m_iname_cmd==true)?"true":"false"));
  printf ("\n_p->m_iname    = %s", _p->m_iname);
  printf ("\n_p->m_inum_cmd = %s", ((_p->m_inum_cmd==true)?"true":"false"));
  printf ("\n_p->m_inum     = %d", (int)_p->m_inum);
  printf ("\n_p->m_smac_cmd = %s", ((_p->m_smac_cmd==true)?"true":"false"));
  printf ("\n_p->m_smac     = %02x:%02x:%02x:%02x:%02x:%02x", _p->m_smac[0], _p->m_smac[1], _p->m_smac[2], _p->m_smac[3], _p->m_smac[4], _p->m_smac[5]);
  printf ("\n_p->m_dmac_cmd = %s", ((_p->m_dmac_cmd==true)?"true":"false"));
  printf ("\n_p->m_dmac     = %02x:%02x:%02x:%02x:%02x:%02x", _p->m_dmac[0], _p->m_dmac[1], _p->m_dmac[2], _p->m_dmac[3], _p->m_dmac[4], _p->m_dmac[5]);
  printf ("\n_p->m_sip_cmd  = %s", ((_p->m_sip_cmd==true)?"true":"false"));
  printf ("\n_p->m_sip      = %s", inet_ntoa(_p->m_sip));
  printf ("\n_p->m_dip_cmd  = %s", ((_p->m_dip_cmd==true)?"true":"false"));
  printf ("\n_p->m_dip      = %s", inet_ntoa(_p->m_dip));
  printf ("\n");
}

void params__test (int _argc, char **_argv)
{
  printf("\n--beg params__test--");
  params *p = params__init (params__alloc(), _argc, _argv);
  printf ("\n--------------------\n");
  params__info(p);
  params__free(params__destroy(p));
  printf("\n--end params__test--\n");
}

/* end of file params.c*/
