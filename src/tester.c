#include <hipl_format.h>

main(argc,argv)
int	argc;
char	*argv[];
{
	struct header hd;

	fp_read_header(&hd);

/*	init_header(&hd, "","",1,"",512,512,8,0,PFBYTE,""); */

	update_header(&hd,argc,argv);

	update_header(&hd,argc,argv);

	write_header(&hd);
}
