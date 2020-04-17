%define DOCSTR
"Python wrapper module for the librat C library
librat performs Monte Carlo Ray Tracing for 3D optical/lidar simulations
Courtesy of Philip Lewis, University College London."
%enddef


%module(docstring=DOCSTR) librat
%{

#include "rat.h"
#include "ratFront.h"

/*
Need to define these dummy functions and a global var, because the library assumes 
they exist, but the librat library configuration has them in the main program file (start.c). 
Since Python has to be the main program and these functions don't do anything we would want from Python,
it seemed safest just to define dummies which do nothing. 
*/
RATobj *globalRATObj=NULL;
void RATuserPrintOptions(RATobj *ratObj){}
int RATuserParse(RATobj *ratObj,int thisarg,int argc,char **argv,void *info){}
void userSignals(){}
void RATuserInterrupt(RATobj *ratObj,int sig){}
int RATtraceRays(RATobj *ratObj,double *from,double *direction,void *nothing){}

#define SWIG_FILE_WITH_INIT

%}


%feature("autodoc", "1");

%include "numpy.i"
%init %{
import_array();
%}

%include "rat_typemaps.i"

/* Major routines - do we need to include more? */
%include "rat.h"
%include "ratFront.h"

/* Major data types - do any more need to be exposed? */
%include "bags.h"
%include "reflectance_functions.h"
%include "wavefront.h"
%include "sensor_rsr.h"

/*
Need to redefine this here because its original definition is found in frat.h,
which also contains a bunch of contradictory definitions of standard functions
such as getenv(). These cause the compiler to complain if we just include frat.h, 
so I have just copied this line in here, so that SWIG will know that a RATobj 
is really the same as a BigBag. 
*/
typedef BigBag RATobj;
