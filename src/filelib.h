#ifndef FILELIB_H
#define FILELIB_H
#include <strings.h>
#include <string.h>

char *getFilenameExtension(),*prependEnv(),*prependDIR();
int	get_mmap_flag();
FILE	*openFile();

#ifndef MAX
#define MAX(a,b) ((a>b)?(a):(b))
#endif
#ifndef MIN
#define MIN(a,b) ((a<b)?(a):(b))
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#endif
