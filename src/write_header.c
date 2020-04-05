#include <string.h>
#include <stdlib.h>
/*	Copyright (c) 1982 Michael Landy, Yoav Cohen, and George Sperling

Disclaimer:  No guarantees of performance accompany this software,
nor is any responsibility assumed on the part of the authors.  All the
software has been tested extensively and every effort has been made to
insure its reliability.   */

/*
 * write_header.c - HIPL image header write
 *
 * Michael Landy - 2/1/82
 * modified to use read/write - 4/26/82
 * modified to pad to N byte boundary JCP UCL P&S 25/2/91
 * This is a big hack ....
 *	Add in a counter that is incremented by the number of bytes
 *	written out.
 *	The padding consists of nulls before the "newline" of the last
 *	string in the header. Therefore a new routine (wstr_n) has
 *	been added (adapted from "wstr") that does not put a "newline"
 *	at the end of the string.
 *	The "last string" is then padded with nulls before the "newline"
 *	and the header is ended with the usual ".\n" marker.
 */

#include <hipl_format.h>
#include <stdio.h>
#include "define_float.h"

#define HIPS "HIPS"
#define UNIT 4

int	hips_count;

/*extern char *halloc();*/

/*void	write_header(hd)
struct header *hd;

{
	void	fwrite_header();
	fwrite_header(1,hd);
}*/

void	___fwrite_header(fd,hd)

struct header *hd;
int fd;

{
	char	*tmp;
	int	i;
	int	write();
	void	wnocr(),dfprintf(),wstr(),wstr_n();

	hips_count = 0;

	if(strncmp(hd->orig_name,HIPS,4) != 0)
	{
		tmp = (char *)calloc(strlen(hd->orig_name)+6,sizeof (char));

		sprintf(tmp,"%s %s",HIPS,hd->orig_name);
		hd->orig_name = tmp;
	}

	wnocr(fd,hd->orig_name);
	wnocr(fd,hd->seq_name);
	dfprintf(fd,hd->num_frame);
	wnocr(fd,hd->orig_date);
	dfprintf(fd,hd->rows);
	dfprintf(fd,hd->cols);
	dfprintf(fd,hd->bits_per_pixel);
	dfprintf(fd,hd->bit_packing);
	dfprintf(fd,hd->pixel_format);
	wstr(fd,hd->seq_history);
	wstr_n(fd,hd->seq_desc);		/* write string with no nl */

	hips_count +=3;				/* account for the "\n.\n" */

	hips_count %= UNIT;

	if(hips_count)				/* pad with nulls */
		for(i=0;i<UNIT-hips_count;i++)
			write(fd,"\0",1);

	write(fd,"\n.\n",3);
}

void	wstr(fd,s)

char *s;
int fd;

{
	int	write();

	write(fd,s,strlen(s));
	hips_count+=strlen(s);

	if(s[0] == 0 || s[strlen(s)-1] != '\n') {
		write(fd,"\n",1);
		hips_count ++;
	}
}

void	wstr_n(fd,s)

char *s;
int fd;

{
	char	eol='\n', nill='\0',tmp_s[2000];
	int	ss,sss,write();

	if(s[0] == nill)
		return;
	strcpy(tmp_s,s);
	sss=strlen(tmp_s);
	ss=sss-1;
	if(tmp_s[ss] == eol)
		tmp_s[ss] = nill;

	write(fd,tmp_s,sss);
	hips_count+=sss;

/*	if(s[0] == nill || s[ss] != eol) {
		write(fd,"\n",1);
		hips_count ++;
	}
*/
}


void	wnocr(fd,s)

char *s;
int fd;

{
	char *t;
	int	write();

	t = s;
	while (*t != '\n' && *t != 0) {
		write(fd,(t++),1);
		hips_count++;
	}
	write(fd,"\n",1);
	hips_count++;
}

void	dfprintf(fd,i)

int fd,i;

{
	char s[50];
	int	write();

	sprintf(s,"%d\n",i);
	write(fd,s,strlen(s));

	hips_count += strlen(s);
}
