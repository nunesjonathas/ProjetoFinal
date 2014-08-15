typedef struct nodes{
	int id;
	double x;
	double y;
	double z;
}Node;

typedef struct edges{
	Node *nodes[2];
	int outline;//0 se contorno, 1 se nao.
	double d;

}Edge;

typedef struct materials{
	int id;
	double mod_elast;
	double coef_poisson;
}Material;

typedef struct elements{
	int id;
	Material *mat;
	int id_thickness;
	int id_integration;
	Node *nodes[3];
	double area_el;
	double centroid_el_x;
	double centroid_el_y;
	
}Element;

typedef struct vectorEdges{
	Edge *e;
	int num_edges;
}VectorEdges;