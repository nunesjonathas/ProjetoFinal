//AREA DO ELEMENTO T3
double AREA(Node *v[3]){
	double area= (*v[0]).x*(*v[1]).y - (*v[1]).x*(*v[0]).y +
				  (*v[1]).x*(*v[2]).y - (*v[2]).x*(*v[1]).y +
				  (*v[2]).x*(*v[0]).y - (*v[0]).x*(*v[2]).y;

		
	return area/2;
}


//CALCULAR CENTROID EM X OU EM Y
double CENTROID(Node *v[3], char a){
	if (a=='x')
	{
		return ((*v[0]).x + (*v[1]).x + (*v[2]).x)/3;
	}else{
		return ((*v[0]).y + (*v[1]).y + (*v[2]).y)/3;
	}
}

//CALCULAR PERIMETRO
double PERIMETER(VectorEdges v_edges){
	int n = v_edges.num_edges;
	double p = 0;
	for (int i = 0; i < n; ++i){
		if (v_edges.e[i].outline==0){
			p+=v_edges.e[i].d;
			//printf("%lf\n", p);
		}else{
			//printf("NAO ENTROU\n");
		}
	}
		
	return p;
}

//CALCULA DISTANCIA ENTRE DOIS NOS
double DIST(Node n1, Node n2){
	double dist = sqrt(pow((n1.x-n2.x),2) + pow((n1.y-n2.y),2));
	return dist;
}


//LER NO E GUARDA NO VETOR DE NOS
void newNode(FILE *fp, char *word, Node *n){
	fscanf(fp,"%120s",word);
	int num_for = atoi(word), id;
	for (int i = 0; i < num_for; ++i)
	{
		fscanf(fp,"%120s",word);
		id = atoi(word);
		n[id-1].id = id;
		fscanf(fp,"%120s",word);
		n[id-1].x = atof(word);
		fscanf(fp,"%120s",word);
		n[id-1].y = atof(word);
		fscanf(fp,"%120s",word);
		n[id-1].z = atof(word);
	}
}

//LER MATERIAL E GUARDA NO VETOR DE MATERIAIS
void newMaterial(FILE *fp, char *word, Material *m){
	fscanf(fp,"%120s",word);
	int num_for = atoi(word), id;
	for (int i = 0; i < num_for; ++i)
	{
		fscanf(fp,"%120s",word);
		id = atoi(word);
		m[id-1].id = id;
		fscanf(fp,"%120s",word);
		m[id-1].mod_elast = atof(word);
		fscanf(fp,"%120s",word);
		m[id-1].coef_poisson = atof(word);
	}
}


//VERIFICA SE DUAS ARESTAS SAO IGUAIS
void verifyEdge(VectorEdges *v_edges, Node *n1, Node *n2, int *indice){
	int n = v_edges->num_edges;
	Edge **e=&v_edges->e;
	//printf("VERIFICANDO: N1_ID %d\tN2_ID %d\n",(*n1).id, (*n2).id);
	if (n==0)
		*indice = -1;
	else{
		//(*((*e)[i]).nodes[0]).id==(*n2).id
		for (int i = 0; i < n; ++i)
		{
			//printf(" COMPARA %d COM %d e %d com %d\n", (*((*e)[i]).nodes[0]).id, (*n2).id, (*((*e)[i]).nodes[1]).id, (*n1).id);
			if (((*((*e)[i]).nodes[0]).id==(*n2).id && (*((*e)[i]).nodes[1]).id==(*n1).id)){

				*indice= i;
				return;
			}
		}

		*indice=-1;
	}
	
}


//CRIACAO DE ARESTAS
void newEdge(VectorEdges *v_edges, Node *v[3]){
	Edge **e=&v_edges->e;
	int *num = &v_edges->num_edges, j, indice;
	
	for (int i = 0; i < 3; ++i)
	{
		//printf("ADD %d e %d\n", (*v[i%3]).id, (*v[(i+1)%3]).id);
		verifyEdge(v_edges,v[i%3],v[(i+1)%3], &indice);

		//printf("J=%d\n", j);
			//printf("%d\n", verifyEdge(v_edges,v[i%3], v[(i+1)%3]));

			if(indice==-1){
				
				((*e)[*num]).nodes[0] = v[i%3];
				((*e)[*num]).nodes[1] = v[(i+1)%3];
				((*e)[*num]).d = DIST(*v[i%3], *v[(i+1)%3]);
				((*e)[*num]).outline = 0;
				//printf("%lf\n", ((*e)[*num]).d);
				//printf("%d\n", *num);
				++*num;

			}else{
		//		printf("ENTROU????\n");
				((*e)[indice]).outline = 1;
			}
			
			
	}
}

void newElement(FILE *fp, char *word, Element *el, Node *n, VectorEdges *v_edges, Material *m){

	fscanf(fp,"%120s",word);
	int num_for = atoi(word), id;
	for (int i = 0; i < num_for; ++i)
	{
		fscanf(fp,"%120s",word);
		id = atoi(word);
		el[id-1].id = id;
		fscanf(fp,"%120s",word);
		el[id-1].mat = &m[atoi(word)-1];
		fscanf(fp,"%120s",word);
		el[id-1].id_integration = atoi(word);
		fscanf(fp,"%120s",word);
		el[id-1].id_thickness = atoi(word);
		
		for (int i = 0; i < 3; ++i)
		{
			fscanf(fp,"%120s",word);
			el[id-1].nodes[i] = &n[atoi(word)-1];
		}

		el[id-1].area_el = AREA(el[id-1].nodes);
		el[id-1].centroid_el_x = CENTROID(el[id-1].nodes, 'x');
		el[id-1].centroid_el_y = CENTROID(el[id-1].nodes, 'y');
		//printf("ADD NOS\n");
		newEdge(v_edges, el[id-1].nodes);

	}
}

void calcVAR(Element *el, int num_elements, double *area_total, double *mod_elast_total, double *coef_poisson_total, double *centroid_total_x, double *centroid_total_y){
	
	for (int i = 0; i < num_elements; ++i)
	{
		*area_total += (el[i].area_el);
		*mod_elast_total += (*el[i].mat).mod_elast*(el[i].area_el);
		*coef_poisson_total += (*el[i].mat).coef_poisson*(el[i].area_el);
		*centroid_total_x +=(el[i].centroid_el_x)*(el[i].area_el); 
		*centroid_total_y +=(el[i].centroid_el_y)*(el[i].area_el);
	}
	*mod_elast_total= *mod_elast_total / *area_total;
	*coef_poisson_total = *coef_poisson_total / *area_total;
	*centroid_total_x = *centroid_total_x / *area_total;
	*centroid_total_y = *centroid_total_y / *area_total;
}