#ifndef MATRIX_H
#define MATRIX_H
#define Tiny 1.0e-20
double vDot(double *a,double *b,int n);
void	LU_decomposition(double *processing_matrix, int n_length, int *processing_vector, double **interchanges );
void	LU_solve( double *processing_matrix, int n_length, int *processing_vector, double *vector_for_cols );
void	matrix_inversion( double *processing_matrix, double *matrix_inverted, double *interchanges, int n_length, double *V );
void	matrix_determinant( double *processing_matrix, double *determinant, int n_length );
void	vector_vector_addition_to_vector( double *vector_a, double *vector_b, double *return_vector, int vector_size );
void	vector_vector_subtraction_to_vector( double *vector_a, double *vector_b, double *return_vector, int vector_size);
void	vector_vector_multiplication_to_vector( double *vector_a, double *vector_b, double *return_vector, int vector_size);
void	vector_vector_division_to_vector( double *vector_a, double *vector_b, double *return_vector, int vector_size);
void	vector_vector_multiplication_to_Product( double *vector_a, double *vector_b, double *vector_product, int vector_size);
void	vector_vector_transpose_multiplication_to_matrix( double *vector_a, double *vector_b, double *return_matrix, int vector_size);
void	vector_matrix_multiplication_to_vector( double *vector, double *matrix, double *return_vector, int vector_size);
void	matrix_vector_multiplication_to_matrix( double *matrix, double *vector, double *return_matrix, int vector_size);
void	matrix_matrix_subtraction_to_matrix( double *matrix_sutract_from, double *matrix_subtractor, double *matrix_subtracted, int n_length);
void	matrix_matrix_addition_to_matrix( double *matrix_addition_to, double *matrix_adder, double *matrix_added, int n_length);
double	matrix_trace(double *matrix, int n_length);
void	matrix_copy( double *from_matrix, double *to_matrix, int n_length);
void	vector_reset( double *vector, int n_length, double value);
void	matrix_reset( double *matrix, int n_length, double value);
double	*vector_allocate( int vec_size);
void	vector_free( double *vector);
double	*matrix_allocate( int n_length );
void	matrix_free( double *matrix );
void	error_message_matrix( char *char_string );
void    matrix_matrix_multiplication_to_matrix( double *m_multiply, double *m_multiplier, double *m_multiplied, int n_length );



#endif
