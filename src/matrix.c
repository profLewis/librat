/*	
**	Original Library Name	:
**				Matrix.c ( ~stuart/lib/libMatrix.a )
**	Library Name	:	matrix.c
**
**	Original Dependencies	:
**				Matrix.h ( ~stuart/Matrix.h )
**	Dependencies	:	matrix.h
**
**	Original Date	:	15/6/1993
**	Date		:	22 June 1994
**
**	Author		:	Stuart Barr, Research Officer, Remote Sensing Unit, University College London.
**
**	Copyright	:	The Remote Sensing Unit, Dept of Geography, Univ College London, 26 Bedford Way, London, WC1 0HP.
**	
**	Notes		:	The routines for LU_decomposition, LU_linear equation solving, Matrix_inversion and Matrix_determinant are
**				adapted from the code in Numerical Recipies in C with additions for pointer manipulations ( faster ).
**				additionally the matrix passed to the library functions Matrix_inversion and Matrix_determinant are not
**				over written as is the case in numerical recipies.
**
**	Bugs		:	Assumes all matrices are square and of the same dimensions ( i, j ) to each other.  Additionally assumes
**				that all vectors are of the same dimensionallity as each other.
**
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>
#include "matrix.h"
#ifdef TESTER
double input[1000],output[1000],t1[1000],vectorv[1000],parameters[1000];
#endif
/*#endif*/
void	LU_decomposition( a, n, indx, d )
int	n, *indx;
double	*a,**d;
	{
	int	i, imax=0, j, k;
	double	big, dum, sum, temp;
	double	*vector();
        static double vv[1000];

	**d = 1.0;

	for( i = 0; i < n; i++ )
	{	big = 0.0;
		for( j = 0; j < n; j++ )
			if( (temp = fabs( a[i * n + j] )) > big ) big = temp;

		if( big == 0.0 ) 
			error_message_matrix( "LU_decomposition : singular matrix encountered in LU decomposition" );
		
		vv[i] = 1.0 / big;
	}



	for( j = 0; j < n; j++ )
	{	
		for( i = 0; i < j; i++ )
		{	sum = a[i * n + j];
			for( k = 0; k < i; k++ ) sum -= a[i * n + k] * a[k * n + j];
			a[i * n + j] = sum;
		}

		big = 0.0;
		for( i = j; i < n; i++ )
		{	sum = a[i * n + j];
			for( k = 0; k < j; k++ ) sum -= a[i * n + k] * a[k * n + j];
			a[i * n + j] = sum;
			if( ( dum = vv[i] * fabs( sum ) ) >= big )
			{	big = dum;
				imax = i;
			}
		}

		if( j != imax )
		{	for( k = 0; k < n; k++ )
			{	dum = a[imax * n + k];
				a[imax * n + k] = a[j * n + k];
				a[j * n + k] = dum;
			}
			**d = -(**d);
			vv[imax] = vv[j];
		}

		indx[j] = imax;
		if( a[j * n + j] == 0.0 ) a[j * n + j] = Tiny;

		if( j != n )
		{	dum = 1.0 / a[j * n + j];
			for( i = j + 1 ; i < n; i++ ) a[i * n + j] *= dum;
		}
	}
/*
**	Lewis: 26 June 1994
**	free vector
*/
}








void	LU_solve( a, n, indx, b )
double	*a, *b;
int	n, *indx;
	{
	int	i, ii = -1, ip, j;
	double	sum;

	
	for( i = 0; i < n; i++ )
	{	ip = indx[i];
		sum = b[ip];
		b[ip] = b[i];
		
		if( ii != -1 ) for( j = ii; j < i; j++ ) sum -= a[i * n + j] * b[j];
		else if( sum ) ii = i;

		b[i] = sum;
	}

	for( i = n - 1; i >= 0; i-- )
	{	sum = b[i];
		for( j = i + 1; j < n; j++ ) sum -= a[i * n + j] * b[j];
		b[i] = sum / a[i * n + i];
	}
}



/*
**	Comments		: 	Lewis, 26 June 1994
**
**	Function Name		:	Matrix_inversion
**
**	Purpose			: 	inversion of a n_length*n_length matrix
**
**	Returns			:	nothing
**
**	Failures		:	
**	FATAL: LU_decomposition(): singular matrix encountered
**	FATAL: free() calls	: error freeing memory (could be made non-fatal)
**
**	Arguments		:
** 	double data_matrix[n_length*n_length]
				:	input matrix
** 	double *matrix_inverted[n_length*n_length]
				:	output matrix
**	double interchanges[1]	:	determinant
**	int n_length		: 	matrix dimensions
**
*/
#define TOL 1e-20 

void	Matrix_inversion( data_matrix, matrix_inverted, interchanges, n_length ,V)
double	*data_matrix, *matrix_inverted, *interchanges,*V;
int	n_length;
	{
	double	determinant,*pm;
	int	i, j;
	void	Matrix_inversion();
        static int processing_vector[1000];
        static double vector_for_cols[1000];

	pm = Matrix_allocate( n_length );
	Matrix_copy( data_matrix, pm, n_length );

	Matrix_determinant(pm , &determinant, n_length );
	if(fabs(determinant)<TOL){
	  /*fprintf(stderr,"det below tol: %f (%f)\n",determinant,TOL);*/
		if(V){*V=0.0;return;}
		n_length--;
		if(n_length==0){
			if(V)*V=determinant;	
			return;
		}
		/* if det == 0 reduce the number of params by 1 & redo */
		/* reset the input matrix to dimension of 1-less */
		for(i=0;i<n_length;i++){
			for(j=0;j<n_length;j++){
				pm[i*n_length+j]=data_matrix[i*(n_length+1)+j];
			}
		}
		Matrix_inversion( pm, matrix_inverted, interchanges, n_length ,V);
		/* copy the inverse matrix to pm */
		Matrix_copy( matrix_inverted, pm, n_length );
		for(i=0;i<n_length;i++){
			for(j=0;j<n_length;j++){
				matrix_inverted[i*(n_length+1)+j]=pm[i*(n_length)+j];
			}
			matrix_inverted[i*(n_length+1)+j]=0.0;
		}
		for(j=0;j<=n_length;j++)matrix_inverted[i*(n_length+1)+j]=0.0;
		return;

	}
	if(V)*V=determinant;
	LU_decomposition( pm, n_length, processing_vector, &interchanges );

#ifdef TEST
	/*	fprintf(stdout,"LU Matrix (%d x %d)\n",n_length,n_length); */
		for(i=0;i<n_length;i++){
			for(j=0;j<n_length;j++)
				fprintf(stdout,"%20.10lf ",*(pm + i*n_length + j));
			fprintf(stdout,"\n");
		}
		fprintf(stdout,"\n");
#endif

	for( j = 0; j < n_length; j++ )
	{	for( i = 0; i < n_length; i++ ) vector_for_cols[i] = 0.0;
		vector_for_cols[j] = 1.0;
		LU_solve( pm, n_length, processing_vector, vector_for_cols );
		for( i = 0; i < n_length; i++ ) matrix_inverted[i * n_length + j] = vector_for_cols[i];
	}
	free(pm);
}


/*
**	Comments		: 	Lewis, 26 June 1994
**
**	Function Name		:	Matrix_determinant
**
**	Purpose			: 	determinant of a n_length*n_length matrix
**
**	Returns			:	nothing
**
**	Failures		:	
**	FATAL: LU_decomposition(): singular matrix encountered
**	FATAL: free() calls	: error freeing memory (could be made non-fatal)
**
**	Arguments		:
** 	double data_matrix[n_length*n_length]
			 	:	input matrix
** 	double determinant[1]	:	matrix determinant
**	int n_length		: 	matrix dimensions
**
*/


void	Matrix_determinant( data_matrix, determinant, n_length )
double	*data_matrix, *determinant;
int	n_length;
	{
	int	j;
        static double processing_matrix2[1000];
        static int processing_vector[1000];
	
	Matrix_copy( data_matrix, processing_matrix2, n_length );

	LU_decomposition( processing_matrix2, n_length, processing_vector, &determinant );

	for( j = 0; j < n_length; j++ ) *determinant *= processing_matrix2[j * n_length + j];

}


double vDot(double *a,double *b,int n){
  double retval=0;
  int i;
  for(i=0;i<n;i++){
    retval += a[i]*b[i];
  }
  return(retval);
}

/*
**	Comments		: 	Lewis, 26 June 1994
**
**	Function Name		:	MatrixInvertTest
**
**	Purpose			: 	Test Inversion of a n_length*n_length matrix
**
**	Returns			:	matrices printed on stdout
**
**	Arguments		:
**
*/


void	Vector_Vector_addition_to_Vector( vector_a, vector_b, return_vector, vector_size )
double	*vector_a, *vector_b, *return_vector;
int	vector_size;
	{
	int	i;

	for( i = 0; i < vector_size; i++ ) *return_vector++ = *vector_a++ + *vector_b++;
}








void	Vector_Vector_subtraction_to_Vector( vector_a, vector_b, return_vector, vector_size )
double	*vector_a, *vector_b, *return_vector;
int	vector_size;
	{
	int	i;

	for( i = 0; i < vector_size; i++ ) *return_vector++ = *vector_a++ - *vector_b++;
}








void	Vector_Vector_multiplication_to_Vector( vector_a, vector_b, return_vector, vector_size )
double	*vector_a, *vector_b, *return_vector;
int	vector_size;
	{
	int	i;

	for( i = 0; i < vector_size; i++ ) *return_vector++ = *vector_a++ * *vector_b++;
}








void	Vector_Vector_division_to_Vector( vector_a, vector_b, return_vector, vector_size )
double	*vector_a, *vector_b, *return_vector;
int	vector_size;
	{
	int	i;

	for( i = 0; i < vector_size; i++ ) *return_vector++ = *vector_a++ / *vector_b++;
}








void	Vector_Vector_multiplication_to_Product( vector_a, vector_b, vector_product, vector_size )
double	*vector_a, *vector_b, *vector_product;
int	vector_size;
	{
	int	i;

	*vector_product = 0.0;
	for( i = 0; i < vector_size; i++ ) *vector_product += *vector_a++ * *vector_b++;
}








void	Vector_Vector_transpose_multiplication_to_Matrix( vector_a, vector_b, return_matrix, vector_size )
double	*vector_a, *vector_b, *return_matrix;
int	vector_size;
	{
	int	i, j;

	for( i = 0; i < vector_size; i++ )	
	{	for( j = 0; j < vector_size; j++ )
		{	*( return_matrix + ( i * vector_size + j ) ) = *( vector_a + i ) * *( vector_b + j );
		}
	}
}








void	Vector_Matrix_multiplication_to_Vector( vector, matrix, return_vector, n_length )
double	*vector, *matrix, *return_vector;
int	n_length;
	{
	int	i, j;

	*return_vector=0.0;
	for( i = 0; i < n_length; i++ )
	{	for( j = 0; j < n_length; j++ ) 
		{	*return_vector += *(vector + j) * *(matrix + (i * n_length + j));
		}
		if(i<n_length-1){
			return_vector++;
			*return_vector=0.0;
		}
	}
}








void	Matrix_Vector_multiplication_to_Matrix( matrix, vector, return_matrix, n_length )
double	*vector, *matrix, *return_matrix;
int	n_length;
	{
	int	i, j;

	for( i = 0; i < n_length; i++ )
	{	for( j = 0; j < n_length; j++ ) 
		{	*( return_matrix + ( j * n_length + i ) ) += *(matrix + (i * n_length + j ) ) * *( vector + i );
		}
	}
}








void	Matrix_Matrix_multiplication_to_Matrix( m_multiply, m_multiplier, m_multiplied, n_length )
double	*m_multiply, *m_multiplier, *m_multiplied;
int	n_length;
	{
	int	i, j, x;

	for( x = 0; x < n_length; x++ )
	{	for( i = 0; i < n_length; i++ )
		{	for( j = 0; j < n_length; j++ )
			{	*( m_multiplied + ( i * n_length + x ) ) += *( m_multiplier + ( i * n_length + j ) ) * *( m_multiply + ( j * n_length + x ) );
			}
		}
	}
}








void	Matrix_Matrix_subtraction_to_Matrix( matrix_sutract_from, matrix_subtractor, matrix_subtracted, n_length )
double	*matrix_sutract_from, *matrix_subtractor, *matrix_subtracted;
int	n_length;
	{
	int	i;

	for( i = 0; i < n_length * n_length; i++ )  *( matrix_subtracted + i ) = *( matrix_sutract_from + i ) - *( matrix_subtractor + i );
}








void	Matrix_Matrix_addition_to_Matrix( matrix_addition_to, matrix_adder, matrix_added, n_length )
double	*matrix_addition_to, *matrix_adder, *matrix_added;
int	n_length;
	{
	int	i;

	for( i = 0; i < n_length * n_length; i++ )  *( matrix_added + i ) = *( matrix_addition_to + i ) + *( matrix_adder + i );
}








double	Matrix_trace( matrix, n_length )
double	*matrix;
int	n_length;
	{
	double	trace_sum = 0.0;
	int	n;

	for( n = 0; n < n_length; n++ ) trace_sum += *( matrix + ( n * n_length + n ) );

	return( trace_sum );
}








void	Vector_copyM( from_vector, to_vector, n_length )
double	*from_vector, *to_vector;
int	n_length;
	{
	int	i;

	for( i = 0; i < n_length; i++ ) *to_vector++ = *from_vector++;
}








void	Matrix_copy( from_matrix, to_matrix, n_length )
double	*from_matrix,*to_matrix;
int	n_length;
	{
	int	i;

	for( i = 0; i < n_length * n_length; i++ ) *to_matrix++ = *from_matrix++;
}








void	Vector_reset( vector, n_length, value )
double	*vector;
int	n_length;
double	value;
	{
	int	n;

	for( n = 0; n < n_length; n++ ) *vector++ = value;
}








void	Matrix_reset( matrix, n_length, value )
double	*matrix;
int	n_length;
double	value;
	{
	int	n;

	for( n = 0; n < n_length * n_length; n++ ) *matrix++ = value;
}








double	*Vector_allocate( vec_size )
int	vec_size;
	{
	double	*vec;void *calloc();

	if( (vec = (double *) calloc( vec_size, sizeof(double))) == 0 )
		error_message_matrix( "Vector_allocate : unable to allocate memmory for a vector" );
	return( vec );
}








void	Vector_free( vector )
double	*vector;
	{



	free( vector );
}








double	*Matrix_allocate( n_length )
int	n_length;
	{
	double	*matrix;void *calloc();

/*
**	The matrix is considered as an one dimensional array of matrix_dimension^
*/

	if( (matrix = (double *) calloc( n_length*n_length, sizeof(double))) == 0 )
		error_message_matrix( "Matrix_allocate : unable to allocate memmory for a matrix" );
	return( matrix );
}








void	Matrix_free( matrix )
double	*matrix;
	{



	free( matrix );
}







void	error_message_matrix( string )
char	*string;
	{
#ifdef EXIT
	fprintf(stderr,"\nMatrix Error: %s\n\n", string );
	exit( 1 );
#else
	return;
#endif
}
