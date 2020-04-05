typedef struct{
	BBox	bbox;
	BBox	*object_bbox;
	TRIPLET	extent;
	int	x_overlap;
	int	y_overlap;
	int	2x_overlap;
	int	2y_overlap;
	double	dx,dy;
	double	*done;
}SuperCell;

#define	DOWN		0
#define	DOWN_RIGHT	1
#define	RIGHT		2
#define	UP_RIGHT	3
#define	UP		4
#define	UP_LEFT		5
#define	LEFT		6
#define	DOWN_LEFT	7

#define	SHIFT_UP	for(i=supercell->2y_overlap-1;i>0;i--)
#define	SHIFT_LEFT	for(j=supercell->2x_overlap-1;j>0;ij--)
#define	SHIFT_DOWN	for(i=0;i<supercell->2y_overlap-1;i++)
#define	SHIFT_RIGHT	for(j=0;j<supercell->2x_overlap-1;j++)
#define	STET_COL	for(i=0;i<supercell->2y_overlap;i++)
#define	STET_ROW	for(j=0;j<supercell->2x_overlap;j++)

#define	CLEAR_UP	STET_ROW *(supercell->done + (0*supercell->2x_overlap) + j )=0.0;
#define	CLEAR_DOWN	STET_ROW *(supercell->done + (2*supercell->2x_overlap) + j )=0.0;
#define	CLEAR_RIGHT	STET_COL *(supercell->done + (i*supercell->2x_overlap) + 2 )=0.0;
#define	CLEAR_UP	STET_COL *(supercell->done + (i*supercell->2x_overlap) + 0 )=0.0;

#define done(i,j) 	*(supercell->done + (i*supercell->2x_overlap) + j )
