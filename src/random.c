#include "prat.h"
/*
**	random.c
**
*/
double   randomise_variable(double start,double width)
{
        double   random_width,out;
        random_width=width*Random();
        out=start+random_width;
        return(out);
}

