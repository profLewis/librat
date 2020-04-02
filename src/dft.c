/* fft/dft.f -- translated by f2c (version of 8 February 1991  13:22:30).
   You must link the resulting object file with the libraries:
	-lF77 -lI77 -lm -lc   (in that order)
*/

#include "f2c.h"

/* Subroutine */ int dft_(n, imode, ip, op)
integer *n, *imode;
doublecomplex *ip, *op;
{
    /* System generated locals */
    integer i__1, i__2, i__3, i__4, i__5;
    doublereal d__1, d__2, d__3;
    doublecomplex z__1, z__2, z__3, z__4, z__5, z__6;

    /* Local variables */
    static integer i, k;
    static doublecomplex w;
    extern /* Double Complex */ int cdexp_();

    /* Parameter adjustments */
    --op;
    --ip;

    /* Function Body */
    d__1 = (doublereal) (*n);
    z__1.r = (float)0. / d__1, z__1.i = (float)6.2831853071959998 / d__1;
    w.r = z__1.r, w.i = z__1.i;
    i__1 = *n;
    for (k = 1; k <= i__1; ++k) {
	i__2 = k;
	op[i__2].r = (float)0., op[i__2].i = (float)0.;
	i__2 = *n;
	for (i = 1; i <= i__2; ++i) {
/* L1: */
	    i__3 = k;
	    i__4 = k;
	    i__5 = i;
	    d__1 = (doublereal) (*imode);
	    z__6.r = d__1 * w.r, z__6.i = d__1 * w.i;
	    d__2 = (doublereal) (i - 1);
	    z__5.r = d__2 * z__6.r, z__5.i = d__2 * z__6.i;
	    d__3 = (doublereal) (k - 1);
	    z__4.r = d__3 * z__5.r, z__4.i = d__3 * z__5.i;
	    cdexp_(&z__3, &z__4);
	    z__2.r = ip[i__5].r * z__3.r - ip[i__5].i * z__3.i, z__2.i = ip[
		    i__5].r * z__3.i + ip[i__5].i * z__3.r;
	    z__1.r = op[i__4].r + z__2.r, z__1.i = op[i__4].i + z__2.i;
	    op[i__3].r = z__1.r, op[i__3].i = z__1.i;
	}
	if (*imode == 1) {
	    i__3 = k;
	    i__4 = k;
	    d__1 = (doublereal) (*n);
	    z__1.r = op[i__4].r / d__1, z__1.i = op[i__4].i / d__1;
	    op[i__3].r = z__1.r, op[i__3].i = z__1.i;
	}
/* L2: */
    }
    return 0;
} /* dft_ */

