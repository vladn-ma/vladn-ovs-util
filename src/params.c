/*
  06/11/15 12:43
  vladn
  
  params.c
  source file for the global parameters support
*/

#include <getopt.h>
#include "params.h"

params* params__alloc (void)
{
    ProgParams* _p = NULL;
    _p = (ProgParams*) calloc (sizeof (ProgParams), 1);
    ASSERT (_p != NULL);
    return _p;
}

/*
 * init params from the command line arguments list
 * if argument is not in the command list, then init default value
 */
params *params__init (params* _p, int _argc, char **_argv)
{
	struct option long_options[] = {
			{"interface", required_argument, 0, 'i'},
			{"dmac",      required_argument, 0, 'd'},
			{"version",   no_argument, 0, 'v'},
			{"help",      no_argument, 0, 'h'},
			{0,           0,           0, 0  }
	};
	char *optstring = "i:d:vh";

	FC_ASSERT (_p!=NULL);
	FC_ASSERT (_argc != 0);
	FC_ASSERT (_argv != NULL);

  memset(_p, 0, sizeof(*_p));

  strncpy(_p->m_pname, "ethsr", sizeof "_p->m_pname);
  strncpy(_p->m_if_str, "eth0", sizeof "_p->m_if_str);
  _p->m_dmac = 0x00000000;

  _p->m_print_version = FALSE;
  _p-> m_print_help = FALSE;

	strncpy (&_p->m_pname, _argv[0], sizeof _p->m_pname);

	while(1) {
		c = getopt_long(_argc, _argv, optstring, long_options, NULL);
		if (c < 0) break;

		/*printf("\n--- c = %d (%c); optarg = %s", c, c, optarg);*/
		switch (c) {
		case 'i': strncpy (_p->m_if_str, optarg, sizeof (_p->m_if_str)); break;
		case 'd': _p->m_dmac = atoi(optarg); break;
		case 'v': _p->m_print_version = FC_TRUE; break;
		case 'h': _p->m_print_help = FC_TRUE; break;
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

ProgParams *params__destroy (params* _p)
{
  ASSERT (_p!=NULL);
  return _p;
}

void params__free (params* _p)
{
  ASSERT (_p!=NULL);
  return ;
}

void params__print_version (params *_p, FILE *_fh)
{
  ASSERT (_p != NULL);
  ASSERT (_fh != NULL);
  ASSERT (_p->m_pname != NULL);

  fprintf (_fh, "\n%s version = 1.1.1\n", _p->m_pname);
}

int params__print_help (params *_p, FILE *_fh)
{
  FC_ASSERT (_p!=NULL);
  FC_ASSERT (_fh!=NULL);
  FC_ASSERT (_p->m_pname != NULL);

  params__PrintVersion (_p, _fh);
	fprintf (_fh, "\nusage:");
	fprintf (_fh, "\n  %s [options]", _p->m_pname);
	fprintf (_fh, "\noptions:");
	fprintf (_fh, "\n  -h || --help                /*print short help then exit*/");
	fprintf (_fh, "\n  -v || --version             /*print short version information then exit*/");
	fprintf (_fh, "\n  -a || --interface INTERFACE /*interface from which to send packet, default is \"eth0\"*/");
	fprintf (_fh, "\n  -l || --dmac DMAC           /*destanation mac address, default is \"000000000000\"*/");
	fprintf (_fh, "\nif no arguments, then default values are used");
	fprintf (_fh, "\n");

	return FC_OK;
}

static params *params__InitDefaults (params* _p)
{
  FC_ASSERT (_p!=NULL);
  return _p;
}

/******************************************************************************
 * info and test class functions
 *****************************************************************************/
void params__info (params *_p)
{
    FC_ASSERT (_p!=NULL);

    printf ("\nparams: ");
    printf ("\n m_pname = \"%s\"", _p->m_pname);
    printf ("\n m_if_str = \"%s\"", _p->m_if_str);
    printf ("\nm_dmac = %d", _p->m_dmac);

    printf ("\nm_print_version = %s", ((_p->m_print_version != 0)?"TRUE":"FALSE"));
    printf ("\nm_print_help = %s", ((_p->m_print_help != 0)?"TRUE":"FALSE"));

    return FC_OK;
}

/*test funcitons*/
void params__test (int _argc, char **_argv)
{
    printf("\n--beg params__test--");

    params *p = params__init (params__alloc(), _argc, _argv);

    if (p->m_print_version == FC_TRUE)
    	params__PrintVersion (p, stdout);

    if (p->m_print_help == FC_TRUE)
    	params__PrintHelp (p, stdout); ;

    params__Info(p);

    params__free(params__destroy(p));

    printf("\n--end params__test--\n");
    return FC_OK;
}

/* end of file params.c*/
