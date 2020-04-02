#ifndef MATRIX_H
#define MATRIX_H
#define Tiny 1.0e-20

extern	void	LU_decomposition( /* double *processing_matrix, int n_length, int *processing_vector, double *interchanges */ );
extern	void	LU_solve( /* double *processing_matrix, int n_length, int *processing_vector, double *vector_for_cols */ );
extern	void	Matrix_inversion( /* double *processing_matrix, double *matrix_inverted, double *interchanges, int n_length */ );
extern	void	Matrix_determinant( /* double *processing_matrix, double *determinant, int n_length */ );
extern	void	Vector_Vector_addition_to_Vector( /* double *vector_a, double *vector_b, double *return_vector, int vector_size */ );
extern	void	Vector_Vector_subtraction_to_Vector( /* double *vector_a, double *vector_b, double *return_vector, int vector_size */ );
extern	void	Vector_Vector_multiplication_to_Vector( /* double *vector_a, double *vector_b, double *return_vector, int vector_size */ );
extern	void	Vector_Vector_division_to_Vector( /* double *vector_a, double *vector_b, double *return_vector, int vector_size */ );
extern	void	Vector_Vector_multiplication_to_Product( /* double *vector_a, double *vector_b, double *vector_product, int vector_size */ );
extern	void	Vector_Vector_transpose_multiplication_to_Matrix( /* double *vector_a, double *vector_b, double *return_matrix, int vector_size */ );
extern	void	Vector_Matrix_multiplication_to_Vector( /* double *vector, double *matrix, double *return_vector, int vector_size */ );
extern	void	Matrix_Vector_multiplication_to_Matrix( /* double *matrix, double *vector, double *return_matrix, int vector_size */ );
extern	void	Matrix_Matrix_multiplication_to_Matrix( /* double *m_multiply, double *m_multiplier, double *m_multiplied, int n_length */ );
extern	void	Matrix_Matrix_subtraction_to_Matrix( /* double *matrix_sutract_from, double *matrix_subtractor, double *matrix_subtracted, int n_length */ );
extern	void	Matrix_Matrix_addition_to_Matrix( /* double *matrix_addition_to, double *matrix_adder, double *matrix_added, int n_length */ );
extern	double	Matrix_trace( /* double matrix, int n_length */ );
extern	void	Matrix_copy( /* double *from_matrix, double *to_matrix, int n_length */ );
extern	void	Vector_reset( /* double *vector, int n_length, double value */ );
extern	void	Matrix_reset( /* double *matrix, int n_length, double value */ );
extern	double	*Vector_allocate( /* int vec_size */ );
extern	void	Vector_free( /* double *vector */ );
extern	double	*Matrix_allocate( /* int n_length */ );
extern	void	Matrix_free( /* double *matrix */ );
extern	void	error_message_matrix( /* char char_string */ );

#endif
