/*
**	perr : print error message and exit
**
**	usage : perr(prog_name, format [ , arg ] ... )
**		char *prog_name, *format;
**
**	Prints out an error message on stderr and exiting. Used in much
**	the same way as sprintf etc.
**
**	Uses "varargs" to accept varible number of arguments.
**
**	James Pearson UCL P&S 4/6/91
**	Added simple licence code to get_prog JCP UCL P&S 16/5/94
*/
#include <stdlib.h>
#include <stdio.h>
#ifdef VA_ALIST
#include <varargs.h>
#endif
#ifdef USE_LIC
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/param.h>

#define HLEN 128

#ifndef LIC_MASK
#define LIC_MASK 0xffff0000
#endif

#ifndef LIC_VAL
#define LIC_VAL 0x80281900
#endif

#define SLEEP 6

char	*getenv();

char	bd[] = {'\020','\005','\001','\022','\023','\017','\016','\000'};
char	mess[] = {'\013','\011','\014','\014','\005','\004','\000'};

static void
fail(sec)
unsigned int	sec;
{
	int	i = 0;
	unsigned int	out;
	unsigned char	*s, *o;

	s = (unsigned char *)&sec;
	o = (unsigned char *)&out + 3;

	sleep(sec%SLEEP);
	while(mess[i] != 0)
		fprintf(stderr,"%c",mess[i++]+96);
	for(i=0;i<3;i++)
		*o-- = *s++;
	i = time(0);
	*o = *((unsigned char *)&i + 3);
	fprintf(stderr," %u\n",out);
	exit(1);
}
#endif /* USE_LIC */

char	*rindex();

char *
get_prog(a)
char	*a;
{
	char	*b,*strrchr();
#ifdef USE_LIC
	char	name[HLEN];
	struct hostent *host;
	unsigned int	addr;
	int	i, nok = 1;
	
	i = -1;
	while(bd[++i] != 0)
		bd[i] = bd[i] + 64;

	if (getenv(bd) == NULL)
	{
		gethostid();
		getpid();
		getppid();

		if(gethostname(name, HLEN))
			fail(SLEEP/2);
		if((host = gethostbyname(name)) == NULL)
			fail(SLEEP/2);

		i = 0;
		while (i < MAXHOSTNAMELEN && host->h_addr_list[i] != 0)
		{
			memcpy(&addr,host->h_addr_list[i++],sizeof(int));
			if ((addr&LIC_MASK) == (LIC_VAL&LIC_MASK))
				nok = 0;
		}

		if (nok)
			fail(addr);
	}
#endif  /* USE_LIC */

	/* strip off any leading pathname components */
	if((b = (char *)strrchr(a,(int)'/')) == 0)
		return(a);
	else
		return(++b);
}
#ifdef VA_ALIST
void
perr(va_alist)
va_dcl
{
	va_list args;
	char *fmt, *a, *b;

	va_start(args);

	/* get first argument */
	a = va_arg(args, char *);

	/* strip off any leading pathname components */
	b = get_prog(a);

	/* print routine name */
	fprintf(stderr,"%s: ",b);

	/* get rest of the arguments */
	fmt = va_arg(args, char *);

	/* print error message */
	vfprintf(stderr, fmt, args);

	fputc('\n',stderr);

	va_end(args);

	exit (1);
}
warn(va_alist)
va_dcl
{
	va_list args;
	char *fmt, *a, *b, *rindex();

	va_start(args);

	/* get first argument */
	a = va_arg(args, char *);

	/* strip off any leading pathname components */
/*	if((b = rindex(a,'/')) == 0)
		b = a;
	else
		b++;
*/
	b = get_prog(a);

	/* print routine name */
	fprintf(stderr,"%s: ",b);

	/* get rest of the arguments */
	fmt = va_arg(args, char *);

	/* print error message */
	vfprintf(stderr, fmt, args);

	fputc('\n',stderr);

	va_end(args);

	return (0);
}
#else
void perr(char *s1,char *s2){
fprintf(stderr,"%s %s\n",s1,s2);
exit(1);
}
#endif
