/* fft.c

        Copyright (c) Kapteyn Laboratorium Groningen 1992
        All Rights Reserved.
                
        This program does a two-dimensional in place Fourier transform on
        a pair of maps.  The data for the inplace transform are stored in
        a scratch file.  Special routines have been written to access this
        file by row number.  A row consists of a data string of two times
        the length of a map row, the first half containing the real part
        of the data, the second half the imaginary part. The FFT
        subroutine FFTX is partly based on the subroutine FOUR1 by Norman
        Brenner (MIT, 1971), and adapted for the GIPSY data structure by
        K. Begeman.  The subroutine FFTY is also based on FOUR1.

        Since in the GIPSY data-base the real and imaginary parts are
        stored in different parts of the file (set), the standard FORTRAN
        definition of complex data is not handy, because then we need to
        do lots of - complicated - time consuming transpose operations on
        the data.  In order to avoid this, we defined a complex array as
        follows:

        complex CBUF(N)

        standard FORTRAN: real1,imag1,real2,imag2,...........,realN,imagN

        here:             real1,real2,.....,realN,imag1,imag2,.....,imagN


        NOTES:    To change the maximum map size this program can handle,
                  simply change all parameter statements for MAXP2.


        routines: BITREV   calculates the bit-reversed integer.
                  CLOSEF   closes and removes scratch file.
                  FFTX     does the transform in X direction.
                  FFTY     does the transform in Y direction.
                  FXCX     fixes up complex rows for FFTY.
                  FXRL     fixes up real <--> complex transform.
                  GETROW   reads a row from scratch file.
                  INPMAP   loads data into scratch file and does the
                           FFT in x (if FORM=-1 or 1).
                  MIRROR   reverse order conjugates the FFT image.
                  OPENF    creates scratch file.
                  OUTMAP   gets result of FFT from scratch file, and
                           does the FFT in x (if FORM=0).
                  PUTROW   writes a row to scratch file.
                  READXY   reads from GIPSY subset.
                  SHIFTR   function which converts  FFT  coordinates
                           into map coordinates.
                  SHIFTX   function which converts  FFT  coordinates
                           with respect to a centre offset.
                  VECTOR   routine which does  the  conversion  from
                           amplitude and phase  to  cosine and sine,
                           or the other way around.
                  WRITXY   writes to GIPSY subset.

        other routines referenced are standard GIPSY routines

#>            fft.dc1

Program:      FFT

Purpose:      This program does a two-dimensional Fast Fourier Transform
              on a (pair of) map(s). Maximum map size is 2048 x 2048.

Category:     MANIPULATION

File:         fft.c

Author:       K.G. Begeman

Keywords:

   RINSET=    set and subset(s) input map(s) which contain the real part
              or the amplitude. Maximum number of maps is 256.

   IINSET=    set and subsets(s) input map(s) which contain the imaginairy
              part or phase [no imaginairy part].

   ROUTSET=   set and subset(s) output map(s) which will contain the real
              part or amplitude.

   IOUTSET=   set and subset(s) output map(s) which will contain the
              imaginairy part or phase [no imaginairy part].
              

** SIGN=      sign of transform (1 or -1). If it is not clear from
              the number of input and output sets, this keyword
              will appear.
              Example:
                1 input set, 1 output set   ---> SIGN=+1 (for AP's)
                1 input set, 2 output sets  ---> SIGN=-1
                2 input sets, 1 output set  ---> SIGN=+1
                2 input sets, 2 output sets ---> will be asked

              SIGN=-1     sky to visibility
              SIGN=+1     visibility to sky

** PHASE=     input/output maps in amplitude and phase [Y]?
              Note that if only one input/output set is wanted, it is
              assumed by this program that the input/output set
              is COS and the SIN part is zero.

** VALUE=     Replace BLANKs in input sets by value [0.0].

Notes:        1) This program can only handle maps where the number
                 of pixels in X and Y are a power of two! The minimum
                 map size this program can handle is 16x16.
              2) This program is optimized for real to complex or
                 complex to real transforms.
              3) The program tries to allocate as much memory as needed.
                 If the allocation fails, it will create a scratch file.

Updates:      Aug 23, 1992: KGB, Document created.

#<

*/


/*
 * includes:
 */

#include	"math.h"		/* <math.h> */
#include	"stddef.h"		/* <stddef.h> */
#include	"stdio.h"		/* <stdio.h> */
#include	"stdlib.h"		/* <stdlib.h> */
#include	"string.h"		/* <string.h> */
#include	"gipsyc.h"		/* GIPSY definitions */
#include	"cmain.h"		/* programme in C */
#include	"anyout.h"		/* anyout_c */
#include	"error.h"		/* error_c */
#include	"fextend.h"		/* fextend_c */
#include	"finis.h"		/* finis_c */
#include	"gdsasn.h"		/* gdsasn_c */
#include	"gdsc_fill.h"		/* gdsc_fill_c */
#include	"gdsc_grid.h"		/* gdsc_grid_c */
#include	"gdsc_ndims.h"		/* gdsc_ndims_c */
#include	"gdsc_range.h"		/* gdsc_range_c */
#include	"gdsi_read.h"		/* gdsi_read_c */
#include	"gdsi_write.h"		/* gdsi_write_c */
#include	"gdsinp.h"		/* gdsinp_c */
#include	"gdsout.h"		/* gdsout_c */
#include	"init.h"		/* init_c */
#include	"minmax3.h"		/* minmax3_c */
#include	"nelc.h"		/* nelc_c */
#include	"setfblank.h"		/* setfblank_c */
#include	"status.h"		/* status_c */
#include	"userint.h"		/* userint_c */
#include	"userlog.h"		/* userlog_c */
#include	"userreal.h"		/* userreal_c */
#include	"wminmax.h"		/* wminmax_c */


/*
 * Defines:
 */

#define	ABS(x)		(x<0?-x:x)			/* absolute value */
#define	CLASSDIM	2				/* wanted subset dim */
#define	MAXAXES		10				/* max. number of axis */
#define	MAX(x,y)	(x>y?x:y)			/* maximum */
#define	MIN(x,y)	(x<y?x:y)			/* minimum */
#define	MAXP2		11				/* max. power of two */
#define	MAXSET		2				/* for real and imag */
#define	MAXSETNAMLEN	80				/* max. length of set name */
#define	MAXSUB		256				/* max. number of subsets */
#define	PI		(3.1415926535897893238462643)	/* pi */
#define	RAD		(57.2957795130823230876798155)	/* degrees in radian */
#define	SIZE		(1<<MAXP2)			/* max. size of fft */
#define	VERSION		"1.0"				/* version/subversion */


/*
 * Keywords and messages:
 */

#define	KEY_RINSET	tofchar("RINSET=")
#define	KEY_IINSET	tofchar("IINSET=")
#define	KEY_ROUTSET	tofchar("ROUTSET=")
#define	KEY_IOUTSET	tofchar("IOUTSET=")
#define	KEY_PHASE	tofchar("PHASE=")
#define	KEY_SIGN	tofchar("SIGN=")
#define	KEY_VALUE	tofchar("VALUE=")
#define	MES_RINSET	tofchar("Set (and subsets) containing real part")
#define	MES_IINSET	tofchar("Set (and subsets) containing imag. part [no imag part]")
#define	MES_ROUTSET	tofchar("Set (and subsets) containing real part")
#define	MES_IOUTSET	tofchar("Set (and subsets) containing imag. part [no imag. part]")
#define	MES_VALUE	tofchar("Replace BLANKs with VALUE [0.0]")


/*
 * The globals:
 */

static	FILE	*SCRATCH = NULL;		/* sratch file descriptor */
static	char	TMPNAM[FILENAME_MAX];		/* name of scratch file */
static	fint	FORM;				/* type of FFT */
static	fint	LOGP2X;				/* LOG (base 2) of X size */
static	fint	LOGP2Y;				/* LOG (base 2) of Y size */
static	fint	NPTX;				/* points in X */
static	fint	NPTY;				/* points in Y */
static	fint	SIGN;				/* sign of transform */
static	fint	POWTAB[MAXP2];			/* table with powers of 2 */
static	fint	XOFTAB[SIZE];			/* table with X offsets */
static	fint	REVTAB[SIZE];			/* bit reverse table */
static	float	COSTAB[SIZE/2];			/* table with cosines */
static	float	SINTAB[SIZE/2];			/* table with sines */
static	float	FACTAB;				/* factor */
static	float	*MEMORY = NULL;			/* internal memory */
static	float	BLANK;				/* universal blank */
static	float	VALUE = 0.0;			/* replace blanks with value */


/*
 * readxy reads an integer number of lines from a GIPSY subset.
 * It is comparable to the old VMS routine READXY.
 */

static	void	readxy( fchar set ,	/* name of GIPSY set */
                        fint  sub ,	/* level of GIPSY subset */
                        fint  xst ,	/* start x */
                        fint  yst ,	/* start y */
                        float dat[] ,	/* array to receive data */
                        fint  xsz ,	/* number of x pixels */
                        fint  ysz )	/* number of y pixels */
{
   fint	buflen;				/* length of read buffer */
   fint	cwlo, cwup;			/* coordinate words */
   fint	done;				/* number of pixels read */
   fint	k;				/* loop counter */
   fint	tid = 0;			/* transfer id */
   fint	flo[CLASSDIM], fup[CLASSDIM];	/* grid positions start and end */

   flo[0] = xst;
   flo[1] = yst;
   fup[0] = xsz + xst - 1;
   fup[1] = ysz + yst - 1;
   cwlo = gdsc_fill_c( set, &sub, flo );
   cwup = gdsc_fill_c( set, &sub, fup );
   buflen =  xsz * ysz;
   gdsi_read_c( set, &cwlo, &cwup, dat, &buflen, &done, &tid );
   for ( k = 0; k < done; k++ ) {	/* check for BLANKS */
      if ( dat[k] == BLANK ) dat[k] = VALUE;
   }
}


/*
 * writxy writes an integer number of lines to a GIPSY subset.
 * It is comparable to the old VMS routine WRITXY.
 */

static	void	writxy( fchar set ,	/* name of GIPSY set */
                        fint  sub ,	/* level of GIPSY subset */
                        fint  xst ,	/* start x */
                        fint  yst ,	/* start y */
                        float dat[] ,	/* array contains data */
                        fint  xsz ,	/* number of x pixels */
                        fint  ysz )	/* number of y pixels */
{
   fint	buflen;				/* length of write buffer */
   fint	cwlo, cwup;			/* coordinate words */
   fint	done;				/* number of pixels written */
   fint	tid = 0;			/* transfer id */
   fint	flo[CLASSDIM], fup[CLASSDIM];	/* grid positions start and end */

   flo[0] = xst;
   flo[1] = yst;
   fup[0] = xsz + xst - 1;
   fup[1] = ysz + yst - 1;
   cwlo = gdsc_fill_c( set, &sub, flo );
   cwup = gdsc_fill_c( set, &sub, fup );
   buflen =  xsz * ysz;
   gdsi_write_c( set, &cwlo, &cwup, dat, &buflen, &done, &tid );
}


/*
 * openf creates the scratch file for internal storage. It first tries
 * to allocate the memory needed by the programme. If this does not
 * succeed, it will create a scratch file.
 */

static	void	openf( fint lpt ,	/* size of FFT in X */
                       fint mpt )	/* size of FFT in Y */
{
   fint		usrmem;
   int		i = 0;

   usrmem = lpt * mpt * ( 1 + MAX( 0, FORM ) ) + 2 * ( 1 - ABS( FORM ) ) * lpt;
   usrmem *= sizeof( float );
   MEMORY = malloc( usrmem );
   if ( MEMORY != NULL ) return;	/* we're done */
   do {
      sprintf( TMPNAM, "FFT.SCRATCH.%5.5d", i++ );
      SCRATCH = fopen( TMPNAM, "rb" );
      if ( SCRATCH == NULL ) {
         SCRATCH = fopen( TMPNAM, "wb" );
      } else {
         fclose( SCRATCH );
         SCRATCH = NULL;
      }
   } while ( SCRATCH == NULL );
   fclose( SCRATCH );
   if ( fextend_c( tofchar( TMPNAM ), &usrmem ) ) {
      fint	error_level = 4;

      error_c( &error_level, tofchar( "Cannot create scratch file" ) );
   }
   SCRATCH = fopen( TMPNAM, "r+b" );
}


/*
 * closef closes and deletes the scratch file.
 */

static	void	closef( void )
{
   if ( SCRATCH != NULL ) {
      fclose( SCRATCH );
      remove( TMPNAM );
   } else if ( MEMORY != NULL ) {
      free( MEMORY );
   }
   SCRATCH = NULL;
   MEMORY = NULL;
}


/*
 * getrow reads a row from scratch file.
 */

static	void	getrow( float data[] , 	/* array to read data in */
                        fint  n ,	/* number of elements in data */
                        fint  row )	/* row number to read */
{
   fint	nbytes = n * sizeof( float );

   if ( MEMORY != NULL ) {
      memmove( data, &MEMORY[row * n], nbytes );
   } else if ( SCRATCH != NULL ) {
      fseek( SCRATCH, row * nbytes, SEEK_SET );
      fread( data, sizeof( float ), n, SCRATCH );
   }
}


/*
 * putrow writes a row to scratch file.
 */

static	void	putrow( float data[] , 	/* array with data to be written */
                        fint  n ,	/* number of elements in data */
                        fint  row )	/* row number to write */
{
   fint	nbytes = n * sizeof( float );

   if ( MEMORY != NULL ) {
     memmove( &MEMORY[row * n], data, nbytes );
   } else if ( SCRATCH != NULL ) {
      fseek( SCRATCH, row * nbytes, SEEK_SET );
      fwrite( data, sizeof( float ), n, SCRATCH );
   }
}


/*
 * bitrev does the bit reversal. It returns bit reversed iarg of
 * bits bits.
 */

static	fint	bitrev( fint iarg ,	/* bit reversed image is returned */
                        fint bits )	/* number of bits to reverse */
{
   fint	i;					/* loop counter */
   fint	r = 0;					/* return value */

   for ( i = 0; i < bits; i++ ) {		/* loop */
      if ( iarg & ( 1 << i ) ) {		/* bit test, is on */
         r |= ( 1 << ( bits - i - 1 ) );	/* put out reversed bit */
      }
   }
   return( r );					/* return to caller */
}


/*
 * mirror reverse order conjugates complex data around the FFT centre.
 */

static	void	mirror( float cbuf[] ,	/* array containing complex data */
                        fint  ipix ,	/* number of points in cbuf */
                        fint  icen )	/* rotation centre offset */
{
   fint		k, k1, k2, irot;
   float	tr, ti;

   irot = ipix / 2 - 1;
   k1 = icen;
   k2 = icen;
   cbuf[k1+ipix] = -cbuf[k2+ipix];
   for ( k = 0; k < irot; k++ ) {
      k1 = k1 - 1;
      if ( k1 < 0 ) k1 = ipix - 1;
      k2 = k2 + 1;
      if ( k2 >= ipix ) k2 = 0;
      tr = cbuf[k1];
      ti = cbuf[k1+ipix];
      cbuf[k1]      =  cbuf[k2];
      cbuf[k1+ipix] = -cbuf[k2+ipix];
      cbuf[k2]      =  tr;
      cbuf[k2+ipix] = -ti;
   }
}


/*
 * shiftr calculates a line number in the map corresponding with the FFT
 * row number.
 */

static	fint	shiftr( fint ioff ,	/* bottom line number */
                        fint ipix ,	/* number of pixels (flip over border) */
                        fint irow )	/* FFT row number */
{
   fint	idum;

   idum = ipix + ioff - 1;		/* idum is map largest line number */
   if ( irow > idum ) {
      return( irow - ipix );
   } else {
      return( irow );
   }
}


/*
 * shiftx calculates the position in an array corresponding with the FFT
 * address.
 */

static	fint	shiftx( fint ioff ,	/* offset from centre */
                        fint ipix ,	/* number of pixels (flip over border) */
                        fint ifft )	/* FFT address */
{
   fint	idum;

   idum = ifft + ioff;			/* idum is shifted FFT address */
   if ( idum < ipix ) {
      return( idum );
   } else {
      return( idum - ipix );
   }
}


/*
 * vector converts cos. and sin. into amp. and pha., or the other way around.
 */

static	void	vector( float cbuf[] ,	/* complex array */
                        fint  n ,	/* number of elements cbuf */
                        fint  m1 ,	/* input type of data */
                        fint  m2 )	/* output type of data */
{
   fint		k;
   float	a, b;

   if ( m1 == m2 ) return;		/* quick solution possible ? */
   if ( m2 == 1 ) {			/* from amp. and pha. to cos. and. sin. */
      for ( k = 0;  k < n; k++ ) {
         a = cbuf[k];
         b = cbuf[k+n] / RAD;
         cbuf[k]   = a * cos( b );
         cbuf[k+n] = a * sin( b );
      }
   } else {				/* from cos. and. sin. to amp. and pha. */
      for ( k = 0; k < n; k++ ) {
         a = cbuf[k];
         b = cbuf[k+n];
         cbuf[k] = sqrt( a * a + b * b );
         if ( cbuf[k] == 0.0 ) {	/* zero amplitude ? */
            cbuf[k+n] = 0.0;
         } else {
            cbuf[k+n] = RAD * atan2( b , a );
         }
      }
   }
}


/*
 * fftx does a one dimensional in place FT.
 */

static	void	fftx( float cbuf[] )	/* array with complex data */
{
   fint	i, ii, l;

   for ( ii = 0; ii < NPTX; ii++ ) {
      fint	ir = REVTAB[ii];

      /*
       * to prevent re-exchanging, i must not be greater than than ir
       */
      if ( ir >= ii ) {
         fint	jj, jr, jjn, jrn;
         float	tr, ti;

         jj  = XOFTAB[ii];
         jr  = XOFTAB[ir];
         jjn = jj + NPTX;
         jrn = jr + NPTX;
         tr = cbuf[jj];
         ti = cbuf[jjn];
         cbuf[jj]  = FACTAB * cbuf[jr];
         cbuf[jjn] = FACTAB * cbuf[jrn];
         cbuf[jr]  = FACTAB * tr;
         cbuf[jrn] = FACTAB * ti;
      }
   }
   /*
    * separate the data into even and odd subsequences,
    * transform each, phase shift the odd transform and
    * add them. this recursive formulation of the FFT is
    * here written iteratively from the innermost subtransform
    * outward.
    */
   for ( l = 0; l < LOGP2X; l++ ) {	
      fint	i2l, istep, q;

      i2l = POWTAB[l];
      istep = 2 * i2l;
      q   = POWTAB[ MAXP2 - l - 1 ];
      for ( i = 0; i < i2l; i++ ) {
         fint	ia, itab;
         float	wr, wi;

         itab = i * q;			/* get index of table entry */
         wr = COSTAB[itab];		/* get cosine from table */
         wi = SINTAB[itab] * SIGN;	/* get sine from table */
         for ( ia = i; ia < NPTX; ia += istep ) {
            fint	ib, ja, jb, jan, jbn;
            float	tr, ti, ur, ui, vr, vi;

            ib = ia + i2l;
            ja = XOFTAB[ia];
            jb = XOFTAB[ib];
            jan = ja + NPTX;
            jbn = jb + NPTX;
            ur = cbuf[jb];
            ui = cbuf[jbn];
            vr = cbuf[ja];
            vi = cbuf[jan];
            tr = wr * ur - wi * ui;
            ti = wr * ui + wi * ur;
            cbuf[ja]  = vr + tr;
            cbuf[jan] = vi + ti;
            cbuf[jb]  = vr - tr;
            cbuf[jbn] = vi - ti;
         }
      }
   }
}


/*
 * fxcx combines two complex arrays for subroutine ffty.
 */

static	void	fxcx( float wr ,	/* cosine term */
                      float wi ,	/* sine term */
                      float row1[] ,	/* first row (complex array) */
                      float row2[] ,	/* second row (complex array) */
                      fint n )		/* number of points */
{
   fint		lx, ly;
   float	i1, i2, r1, r2, tr, ti;

   for ( lx = 0; lx < n; lx++ ) {
      ly = lx + n;
      r1 = row1[lx];
      i1 = row1[ly];
      r2 = row2[lx];
      i2 = row2[ly];
      tr = wr * r2 - wi * i2;
      ti = wr * i2 + wi * r2;
      row1[lx] = r1 + tr;
      row1[ly] = i1 + ti;
      row2[lx] = r1 - tr;
      row2[ly] = i1 - ti;
   }
}


/*
 * ffty does a one dimensional in place Fourier tansform in the Y direction.
 * The bit-reversal is done by inpmap.
 */

static	void	ffty( void )
{
   fint		l, lp2;
   float	row1[2*SIZE], row2[2*SIZE];

   lp2 = 2 * NPTX;			/* number of points in row */
   /*
    * separate the data into even and odd subsequences,
    * transform each, phase shift the odd transform and
    * add them. this recursive formulation of the FFT is
    * here written iteratively, from the innermost
    * subtransform outward.
    */
   for  ( l = 0; l < LOGP2Y; l++ ) {
      fint	i, i2l, istep, q;

      i2l = POWTAB[l];
      istep = 2 * i2l;
      q = POWTAB[ MAXP2 - l - 1 ];
      for ( i = 0; i < i2l; i++ ) {
         fint	ia, ib, itab;
         float	wr, wi;

         itab = i * q;			/* get table entry */
         wr = COSTAB[itab];		/* get cosine from table */
         wi = SINTAB[itab] * SIGN;	/* get sine from table */
         for ( ia = i; ia < NPTY; ia += istep ) {
            ib = ia + i2l;
            getrow( row1, lp2, ia );
            getrow( row2, lp2, ib );
            fxcx( wr, wi, row1, row2, NPTX );
            putrow( row1, lp2, ia );
            putrow( row2, lp2, ib );
         }
      }
   }
}


/*
 * fxrl fixes up a real or half-size complex array for 2 complex arrays (=rows).
 */

static	void	fxrl( float zr ,	/* real factor */
                      float zi ,	/* imaginary factor */
                      float row1[] ,	/* complex array (data from row r1) */
                      float row2[] ,	/* complex array (data from row r2) */
                      fint  r1 ,	/* first row number */
                      fint  r2 ,	/* second row number */
                      fint  n )		/* number of complex numbers in rows */
{
   fint		i, j;				/* counters */
   float	di, dr, ti, tr;

   for ( i = 0; i < n; i++ ) {
      j = i + n;
      dr = row1[i] - row2[i];
      di = row1[j] + row2[j];
      tr = zr * dr - zi * di;
      ti = zr * di + zi * dr;
      row1[i] = row1[i] - tr;
      row1[j] = row1[j] - ti;
      if ( r1 < r2 ) {
         row2[i] = row2[i] + tr;
         row2[j] = row2[j] - ti;
      }
   }
}


/*
 * inpmap loads the data into the scratch file, and does the FFT in x
 * if IFORM=-1 or 1.
 */

static	void	inpmap( fint  lpt ,	/* number of points in X */
                        fint  mpt ,	/* number of points in Y */
                        fint  lmin ,	/* left most x map coordinate */
                        fint  mmin ,	/* lower most y map coordinate */
                        fchar set[] ,	/* input set name */
                        fint  sub[] ,	/* subsets */
                        fint  nset ,	/* number of sets */
                        fint  modi )	/* input mode, amp./pha. or cos./sin. */
{
   fint		i, itab, k, m, md, mi, mj, mph, mphh, ms;
   fint		q, r, ri, rj, row, rs, wor;
   float	rbuf[2*SIZE], cbuf[2*SIZE];
   float	fr, fi, zr, zi;

   if ( FORM == -1 ) {			/* complex ---FFT---> real */
      q = POWTAB[ MAXP2 - LOGP2Y - 1 ];	/* table offset factor */
      mph = mpt / 2;			/* number of rows to load */
      mphh = mph / 2 + 1;
      fr = -0.5 * SIGN;			/* calculate factors */
      fi = -0.5;
      for ( ri = 0 ; ri < mphh; ri++ ) {	/*  loop to load data */
         rj = mph - ri;
         itab = ri * q;			/*  get table position */
         zr = 0.5 - fr * SINTAB[itab];
         zi =       fi * COSTAB[itab];
         mi = shiftr( mmin, mpt, ri );
         for ( k = 0; k < nset; k++ ) {	/* get data */
            i= k * lpt;
            readxy( set[k], sub[k], lmin, mi, &cbuf[i], lpt, 1 );
         }
         mj = shiftr( mmin, mpt, rj );
         for ( k = 0; k < nset; k++ ) {	/*  get data */
            i = k * lpt;
            readxy( set[k], sub[k], lmin, mj, &rbuf[i], lpt, 1 );
         }
         vector( cbuf, lpt, modi, 1 );
         vector( rbuf, lpt, modi, 1 );
         fftx( cbuf );			/* do the FFT */
         fftx( rbuf );
         /* fixup for complex to real transform */
         fxrl( zr, zi, cbuf, rbuf, ri, rj, lpt );
         /* store in scratch file */
         wor = bitrev( ri, LOGP2Y );
         putrow( cbuf, 2 * lpt, wor );
         if ( ( ri != rj ) && ( ri != 0 ) ) {
            wor = bitrev( rj, LOGP2Y );
            putrow( rbuf, 2 * lpt, wor );
         }
      }
   } else if ( FORM == 0 ) {		/* real ---FFT---> complex */
      rs = MIN( mpt, ( 2 * SIZE ) / lpt );	/* number of rows in one pass */
      k = 0;				/* subset number */
      for  ( r = 0; r < mpt; r += rs ) {/* loop to load data */
         md = 0;
         do {
            m = shiftr( mmin, mpt, r + md );
            ms = MIN( rs - md, mpt + mmin - m );
            i = md * lpt;
            readxy( set[k], sub[k], lmin, m, &cbuf[i], lpt, ms );
            md = md + ms;
         } while ( md != rs );
         for ( row = r; row < ( r + rs ); row += 2 ) {
            i = ( row - r ) * lpt;
            wor = bitrev( row / 2, LOGP2Y );
            putrow( &cbuf[i], 2 * lpt, wor );
         }
      }
   } else if ( FORM == 1 ) {		/* complex ---FFT---> complex */
      for  ( row = 0; row < mpt; row++ ) {
         m = shiftr( mmin, mpt, row );	/* get data from disk */
         for ( k = 0; k < nset; k++ ) {
            i = k * lpt;
            readxy( set[k], sub[k], lmin, m, &cbuf[i], lpt, 1 );
         }
         vector( cbuf, lpt, modi, 1 );
         fftx( cbuf );
         wor = bitrev( row, LOGP2Y );
         putrow( cbuf, 2 * lpt, wor );
      }
   }
}


/*
 * outmap saves the result of the Fourier transform back on disk and finds
 * the minimum and maximum.  If FORM=0  then the FFT in x is done.
 */

static	void	outmap( fint  lpt ,	/* number of points in X */
                        fint  mpt ,	/* number of points in Y */
                        fint  lmin ,	/* lower x map coordinate */
                        fint  mmin ,	/* lower y map coordinate */
                        fchar set[] ,	/* set name */
                        fint  sub[] ,	/* subset levels */
                        fint  nset ,	/* number of sets */
                        fint  modo )	/* output mode, amp./pha. or cos./sin. */
{
   fint		i, itab, k, m, md, mi, mj, ms, mph, mphh;
   fint		q, r, ri, rj, rs, row;
   fint		n;
   fint		count[MAXSET], nblank[MAXSET];
   float	rbuf[2*SIZE], cbuf[2*SIZE];
   float	datamax[MAXSET], datamin[MAXSET];
   float	fr, fi, zr, zi;

   for ( k = 0; k < nset; count[k++] = 0 );
   if ( FORM == -1 ) {			/* complex ---FFT---> real */
      k = 0;				/* set number */
      rs = MIN( mpt, ( 2 * SIZE ) / lpt );	/* number of rows per pass */
      for ( r = 0 ; r < mpt; r += rs ) {/* loop to get data from scratch file */
         getrow( rbuf, rs * lpt, r / rs );
         md = 0;
         do {
            m = shiftr( mmin, mpt, r + md );
            ms = MIN( rs - md, mpt + mmin - m );
            i = md * lpt;
            writxy( set[k], sub[k], lmin, m, &rbuf[i], lpt, ms );
            md = md + ms;
         } while ( md != rs );
         n = lpt * rs;
         minmax3_c( rbuf, &n, &datamin[k], &datamax[k], &nblank[k], &count[k] );
      }
   } else if ( FORM == 0 ) {		/* real ---FFT---> complex */
      q = POWTAB[ MAXP2 - LOGP2Y - 1 ];	/* table offset factor */
      mph = mpt / 2;			/* y-dimension of transform */
      mphh = mph / 2 + 1;
      getrow( cbuf, 2 * lpt, 0 );	/* create row MPH */
      putrow( cbuf, 2 * lpt, mph );
      fr = 0.5 * SIGN;
      fi = 0.5;
      for  ( ri = 0; ri < mphh; ri++ ) {
         rj = mph - ri;
         itab = ri * q;			/* calculate table position */
         zr = 0.5 - fr * SINTAB[itab];
         zi =       fi * COSTAB[itab];
         getrow( cbuf, 2 * lpt, ri );
         getrow( rbuf, 2 * lpt, rj );
         fxrl( zr, zi, cbuf, rbuf, ri, rj, lpt );	/* fixup real ---> complex */
         fftx( cbuf );			/* do the FFT in x-direction */
         vector( cbuf, lpt, 1, modo );
         mi = shiftr( mmin, mpt, ri );
         for ( k = 0; k < nset; k++ ) {
            i = k * lpt;
            writxy( set[k], sub[k], lmin, mi, &cbuf[i], lpt , 1 );
            minmax3_c( &cbuf[i], &lpt, &datamin[k], &datamax[k], &nblank[k],
               &count[k] );
         }
         if ( ri > 0 ) {		/* do not double lines */
            mirror( cbuf, lpt, -lmin );
            mi = shiftr( mmin, mpt, mpt - ri );
            for ( k = 0; k < nset; k++ ) {
               i = k * lpt;
               writxy( set[k], sub[k], lmin, mi, &cbuf[i], lpt, 1 );
               minmax3_c( &cbuf[i], &lpt, &datamin[k], &datamax[k], &nblank[k],
                  &count[k] );
            }
         }
         if ( ri == rj ) break;		/* we've done it */
         fftx( rbuf );			/* do the FFT in x-direction */
         vector( rbuf, lpt, 1, modo );
         mj = shiftr( mmin, mpt, rj );
         for ( k = 0; k < nset; k++ ) {
            i = k * lpt;
            writxy( set[k], sub[k], lmin, mj, &rbuf[i], lpt, 1 );
            minmax3_c( &rbuf[i], &lpt, &datamin[k], &datamax[k], &nblank[k],
               &count[k] );
         }
         if ( rj < mph ) {		/* do not double lines */
            mirror( rbuf, lpt, -lmin );
            mj = shiftr( mmin, mpt, mpt - rj );
            for ( k = 0; k < nset; k++ ) {
               i = k * lpt;
               writxy( set[k], sub[k], lmin, mj, &rbuf[i], lpt, 1 );
               minmax3_c( &rbuf[i], &lpt, &datamin[k], &datamax[k], &nblank[k],
                  &count[k] );
            }
         }
      }
   } else if ( FORM == 1 ) {		/* complex ---FFT---> complex */
      for ( row = 0; row < mpt; row++ ) {
         getrow( cbuf, 2 * lpt, row );
         vector( cbuf, lpt, 1, modo );
         m = shiftr( mmin, mpt, row );
         for ( k = 0; k < nset; k++ ) {
            i = k * lpt;
            writxy( set[k], sub[k], lmin, m, &cbuf[i], lpt, 1 );
            minmax3_c( &cbuf[i], &lpt, &datamin[k], &datamax[k], &nblank[k],
               &count[k] );
         }
      }
   }
   {
      fint	change = 1;
      fint	one = 1;

      for ( k = 0; k < nset; k++ ) {
         wminmax_c( set[k], &sub[k], &datamin[k], &datamax[k],
            &nblank[k], &one, &change );
      }
   }
}


/*
 * set2str converts set name to string.
 */

static	fint	set2str( char  str[] ,	/* string */
                         fchar set )	/* name of set */
{
   fint	l, n;

   n = nelc_c( set );
   l = n - 1;
   while ( l >= 0 && set.a[l] != '/' ) l--;
   strncpy( str, &set.a[l+1], n - l - 1 );
   str[n-l-1] = 0;
   return( n - l - 1 );
}


/*
 * sub2str converts the coordinate word of subset to string.
 */

static	fint	sub2str( char  str[] ,	/* string */
                         fchar set ,	/* set name */
                         fint  sub )	/* subset level */
{
   fint	m, n;
   fint	naxis;
   fint	gerror = 0;

   naxis = gdsc_ndims_c( set, &gerror );
   strcpy( str, "(" );
   m = 1;
   for ( n = 0; n < naxis; n++ ) {
      char	s[10];
      fint	axnum = n + 1;
      fint	grid;

      gerror = 0;
      grid = gdsc_grid_c( set, &axnum, &sub, &gerror );
      if ( gerror < 0 ) {
         m += sprintf( s, "-," );
      } else {
         m += sprintf( s, "%d,", grid );
      }
      strcat( str, s );
   }
   str[m-1] = ')';
   return( m );
}


/*
 * message generates a status message for the user.
 */

static	void	message( fchar iset[] ,	/* input sets */
                         fint  isub[] , /* input subsets */
                         fint  inset ,	/* number of input sets */
                         fchar oset[] ,	/* output sets */
                         fint  osub[] ,	/* output subsets */
                         fint  onset )	/* number of output sets */
{
   char	m[1024];
   fint	k, l = 0;

   for ( k = 0; k < inset; k++ ) {
      if ( k ) {
         strcat( m, " + " );
         l += 3;
      }
      l += set2str( &m[l], iset[k] );
      l += sub2str( &m[l], iset[k], isub[k] );
   }
   strcat( m, " --> " );
   l += 5;
   for ( k = 0; k < onset; k++ ) {
      if ( k ) {
         strcat( m, " + " );
         l += 3;
      }
      l += set2str( &m[l], oset[k] );
      l += sub2str( &m[l], oset[k], osub[k] );
   }
   status_c( tofchar( m ) );
}


/*
 * main programme.
 */

MAIN_PROGRAM_ENTRY
{
   char		isetb[MAXSET][MAXSETNAMLEN];
   char		osetb[MAXSET][MAXSETNAMLEN];
   fchar	iset[MAXSET], oset[MAXSET];
   fint		lmin, lpt, mmin, mpt, modi, modo;
   fint		isubr[MAXSUB], isubi[MAXSUB];
   fint		osubr[MAXSUB], osubi[MAXSUB];
   fint		inset, onset;
   fint		nsub;
   fint		k;

   for ( k = 0; k < MAXSET; k++ ) {
      fint	i;

      iset[k].a = isetb[k];
      iset[k].l = MAXSETNAMLEN;
      oset[k].a = osetb[k];
      oset[k].l = MAXSETNAMLEN;
      for ( i = 0; i < MAXSETNAMLEN; i++ ) {
         iset[k].a[i] = ' ';
         oset[k].a[i] = ' ';
      }
   }
   setfblank_c( &BLANK );		/* get BLANK vlaue */
   init_c( );
   IDENTIFICATION( "FFT", VERSION );
   /*
    * get the input from user.
    */
   {
      char	mes[80];
      fint	axperm[MAXAXES];
      fint	axcount[MAXAXES];
      fint	class = 1;
      fint	classdim = CLASSDIM;
      fint	cwlo, cwup;
      fint	error_level = 4;
      fint	gerror = 0;
      fint	input_level;
      fint	itab;
      fint	items;
      fint	maxaxes = MAXAXES;
      fint	output_level;

      items = MAXSUB;
      input_level = 0;
      output_level = 11;
      nsub = gdsinp_c( iset[0], isubr, &items, &input_level, KEY_RINSET,
         MES_RINSET, &output_level, axperm, axcount, &maxaxes, &class,
         &classdim );
      gdsc_range_c( iset[0], isubr, &cwlo, &cwup, &gerror );
      lmin = gdsc_grid_c( iset[0], &axperm[0], &cwlo, &gerror );
      mmin = gdsc_grid_c( iset[0], &axperm[1], &cwlo, &gerror );
      lpt = axcount[0];
      mpt = axcount[1];
      if ( ( lpt > SIZE ) || ( mpt > SIZE ) ) {
         sprintf( mes, "maps are too big, maximum size is %d", SIZE );
         error_c( &error_level, tofchar( mes ) );
      }
      if ( ( lpt < 16 ) || ( mpt < 16 ) ) {
         sprintf( mes, "maps are too small, minimum size is 16" );
         error_c( &error_level, tofchar( mes ) );
      }
      if ( ( SIZE / lpt * lpt != SIZE ) || ( SIZE / mpt * mpt != SIZE ) ) {
         sprintf( mes, "size of maps must be power of two" );
         error_c( &error_level, tofchar( mes ) );
      }
      if ( ( lmin > 0 ) || (( lpt + lmin - 1) < 0 ) || ( mmin > 0 ) || ( ( mpt + mmin - 1 ) < 0 ) ) {
         sprintf( mes, "map does not contain 0,0" );
         error_c( &error_level, tofchar( mes ) );
      }
      items = nsub;
      input_level = 5;
      if (gdsinp_c( iset[1], isubi, &items, &input_level, KEY_IINSET,
         MES_IINSET, &output_level, axperm, axcount, &maxaxes, &class,
         &classdim )) {
         inset = 2;
         if ( lpt != axcount[0] || mpt != axcount[1] ) {
            sprintf( mes, "Unequal subsets sizes!" );
            error_c( &error_level, tofchar( mes ) );
         }
         if ( lmin != gdsc_grid_c( iset[1], &axperm[0], &cwlo, &gerror ) ) {
            sprintf( mes, "Unequal grid ranges!" );
            error_c( &error_level, tofchar( mes ) );
         }
      } else {
         inset = 1;
      }
      gdsasn_c( KEY_RINSET, KEY_ROUTSET, &class );
      gdsasn_c( KEY_RINSET, KEY_IOUTSET, &class );
      items = nsub;
      input_level = 4;
      (void) gdsout_c( oset[0], osubr, &items, &input_level, KEY_ROUTSET,
         MES_ROUTSET, &output_level, axperm, axcount, &maxaxes );
      input_level = 5;
      if ( gdsout_c( oset[1], osubi, &items, &input_level, KEY_IOUTSET,
         MES_IOUTSET, &output_level, axperm, axcount, &maxaxes ) ) {
         onset = 2;
      } else {
         onset = 1;
      }
      modi = modo = 0;			/* reset */
      /*
       * Now we try to find the sign of the transform.
       */
      input_level = 2;			/* hidden keyword */
      if ( inset == onset ) {
         if ( inset == 1 ) {		/* symmetric transform (i.e.AP's) */
            SIGN = 1;			/* sign does not matter */
            FORM = 0;			/* real ---> real */
            modi = 1;			/* input mode is cos and sin = 0 */
            modo = 1;			/* output mode is cos and sin = 0 */
         } else {			/* complex ---> complex */
            input_level = 0;		/* no default */
            FORM = 1;			/* real+imag ---> real+imag */
         }
      } else if ( inset == 1 ) {	/* from sky to visibility ? */
         SIGN = -1;			/* sign is -1 */
         FORM = 0;			/* real ---> real+imag */
         modi = 1;			/* input mode is cos and sin */
      } else {				/* from visibility to sky */
         SIGN = 1;			/* sign is 1 */
         FORM = -1;			/* real+imag ---> real */
         modo = 1;			/* output mode is cos and sin */
      }
      if ( input_level != 0 ) {
         sprintf( mes, "Sign of transform (1 or -1) [%d]", SIGN );
      } else {
         sprintf( mes, "Sign of transform (1 or -1)" );
      }
      items = 1;
      userint_c( &SIGN, &items, &input_level, KEY_SIGN, tofchar( mes ) );
      if ( SIGN == 0 ) {
         sprintf( mes, "Sign cannot be zero, you ...." );
         error_c( &error_level, tofchar( mes ) );
      }
      SIGN = SIGN / labs( SIGN );	/* make it 1 or -1 */
      if ( ( modi == 0 ) || ( modo == 0 ) ) {
         bool	phase = TRUE;

         if ( ( modi == 0 ) && ( modo == 0 ) ) {
            sprintf( mes, "input/output maps in amp. and phase [Y]" );
         } else if ( modi == 0 ) {
            sprintf( mes, "input maps in amp. and phase [Y]" );
         } else if ( modo == 0 ) {
            sprintf( mes, "output maps in amp. and phase [Y]" );
         }
         items = 1;
         input_level = 2;
         userlog_c( &phase, &items, &input_level, KEY_PHASE, tofchar( mes ) );
         if ( !tobool( phase) ) {
            if ( modi == 0 ) {
               modi = 1;		/* input is cos. and sin. */
            }
            if ( modo == 0 ) {
               modo = 1;		/* output is cos. and sin. */
            }
         } else {
            if ( modi == 0 ) {
               modi = -1;		/* input is amp. and pha. */
            }
            if ( modo == 0 ) {
               modo = -1;		/* output is amp. and pha. */
            }
         }
      }
      input_level = 2;
      items = 1;
      (void) userreal_c( &VALUE, &items, &input_level, KEY_VALUE, MES_VALUE );
      for ( itab = 0; itab < ( SIZE / 2 ); itab++ ) {
         COSTAB[itab] = cos( 2.0 * PI * itab / SIZE );
         SINTAB[itab] = sin( 2.0 * PI * itab / SIZE );
      }
      for ( itab = 0; itab < MAXP2; itab++ ) {
         POWTAB[itab]= 1 << itab;
      }
      NPTX = lpt;				/* X dimension of fft */
      NPTY = mpt / ( 2 - MAX( 0, FORM ) );	/* Y dimension of fft */
      for ( itab = 0; itab < MAXP2; itab++ ) {
         if ( NPTX ==  ( 2 * POWTAB[itab] ) ) {
            LOGP2X = itab + 1;
         }
         if ( NPTY == ( 2 * POWTAB[itab] ) ) {
            LOGP2Y = itab + 1;
         }
      }
      FACTAB = sqrt( ( 0.5 * labs( 1 - 3 * FORM ) ) / (double) ( NPTX * NPTY) );
      for ( itab = 0; itab < lpt; itab++ ) {
         XOFTAB[itab] = shiftx( -lmin, lpt, itab );
         REVTAB[itab] = bitrev( itab, LOGP2X );
      }
   }
   openf( lpt, mpt );			/* open scratch file */
   for ( k = 0; k < nsub; k++ ) {
      fint	isub[2], osub[2];

      isub[0] = isubr[k];
      isub[1] = isubi[k];
      osub[0] = osubr[k];
      osub[1] = osubi[k];
      message( iset, isub, inset, oset, osub, onset );
      inpmap( lpt, mpt, lmin, mmin, iset, isub, inset, modi );
      ffty( );
      outmap( lpt, mpt, lmin, mmin, oset, osub, onset, modo );
   }
   closef( );				/* close scratch file */
   finis_c( );
   return( EXIT_SUCCESS );		/* return with status */
}
