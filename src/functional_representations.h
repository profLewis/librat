#ifndef _FUNCTIONAL_REPRESENTATIONS_H
#define _FUNCTIONAL_REPRESENTATIONS_H

/*
**	this file represents the permitted functional representations
**	
*/


float sine_representation(),		/* 2-parm sine function */
	pl1_representation(),           /* 2-parm linear function */
        pl2_representation(),           /* 2-parm piecewise linear function */
	pln_representation(),		/* n-stage piecewise linear function */
	Fourier_cosine_series_representation(),
	power_series_representation();
/*
**	phase #2 functions
*/
float	function_calc();

#endif /* _FUNCTIONAL_REPRESENTATIONS_H */
