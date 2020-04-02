typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Plane		*plane[200];
}Plane_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	fFacet		*facet[200];
}Facet_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Cylinder	*cylinder[200];
}Cylinder_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Sphere		*sphere[200];
}Sphere_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Ellipse		*ellipse[200];
}Ellipse_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
        Ellipse         *spheroid[200];
}Spheroid_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Bezier4		*bezier[200];
}Bezier4_Block;

typedef struct {
        int             new;
        int             block;
        int             no_of_vertex_numbers;
	Bbox		*bbox[200];
}Bbox_Block;
