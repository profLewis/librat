#ifndef _FRAT
#define _FRAT
#define _ARARAT_
#ifdef RAT_MAIN
#define FRAT_MAIN
#endif
#ifndef Power_Macintosh
#include <limits.h>
#else
#define MAXdouble 1e20
#endif

#include "prat.h"
#include "materials.h"
#ifndef TRANSPARENT 
#define TRANSPARENT 10
#endif
#include <unistd.h>

#ifdef DEBUGGER_
#include <mcheck.h>
#endif
/*########################*/
#ifdef FRAT_MAIN
RATobj *globalRATObj=NULL;
/*########################*/
#else
/*########################*/
extern RATobj *globalRATObj;
#endif
/*########################*/

#endif

#include "prat_wavefront_read.h"



