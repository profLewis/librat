#include "prat.h"
/*
**	random.c
**
*/
double Random();

double   randomise_variable(start,width)
double   start,width;
{
        double   random_width,out;
        random_width=width*Random();
        out=start+random_width;
        return(out);
}

