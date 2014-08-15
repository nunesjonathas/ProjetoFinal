#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "structs.c"
#include "functions.c"



int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		printf("ERRO: ARGUMENTOS INSUFICIENTES\n");
		return 1;
	}
	clock_t ti=clock();
	FILE *fp, *fpout;
	Node *n;
	Material *m;
	Element *el;
	VectorEdges v_edges;
	v_edges.num_edges = 0;
	int num_nodes, num_materials, num_elements;
	double perimeter, area_total=0, mod_total=0, coef_poisson_total=0, centroid_total_x=0, centroid_total_y=0;
	char word[121], *output_name;
	fp = fopen( argv[1], "rt");
	output_name = argv[1];
	strtok(output_name, ".");
	strcat(output_name,".out");
	
	fpout = fopen(output_name, "wt"); 
	
	if (fp==NULL || fpout == NULL)
	{
		printf("ERRO\n");
		return 1;
	}
	while (fscanf(fp,"%120s",word) == 1)
	{
		if (strcmp(word,"%NODE")==0) {
			//LE O NUMERO DE NOS e Aloca
			fscanf(fp,"%120s",word);
			num_nodes = atoi(word);
			n = (Node*) malloc(num_nodes * sizeof(Node));

		}
		else if (strcmp(word,"%NODE.COORD")==0) {
			newNode(fp, word, n);
		}
		else if (strcmp(word,"%MATERIAL")==0) {
			fscanf(fp,"%120s",word);
			num_materials = atoi(word);
			m = (Material*) malloc(num_materials * sizeof(Material));			
		}
		else if (strcmp(word,"%MATERIAL.ISOTROPIC")==0) {
			newMaterial(fp, word, m);
		}
		else if (strcmp(word,"%ELEMENT")==0) {
			fscanf(fp,"%120s",word);
			num_elements = atoi(word);
			el = (Element*) malloc(num_elements * sizeof(Element));
			v_edges.e = (Edge*) malloc((num_elements*3) * sizeof(Edge));
		}
		else if (strcmp(word,"%ELEMENT.T3")==0) {
			newElement(fp, word, el, n, &v_edges, m);
		}
		else {
			printf("ERRO NA LEITURA DO ARQUIVO, FORMATO INCORRETO\n");
			return 1;
		}
	}

	fclose(fp);

	v_edges.e = (Edge *) realloc(v_edges.e, v_edges.num_edges * sizeof(Edge));
	perimeter = PERIMETER(v_edges);
	calcVAR(el,num_elements,&area_total, &mod_total, &coef_poisson_total, &centroid_total_x, &centroid_total_y);

	fprintf(fpout, "%%ELEMENTO\n'T3'\n\n");
	fprintf(fpout, "%%AREA\n%10.6f\n\n", area_total);
	fprintf(fpout, "%%PERIMETRO\n%10.6f\n\n", perimeter);
	fprintf(fpout, "%%VARIAVEL\n'Modulo_de_elasticidade'\n%10.6e\n\n", mod_total);
	fprintf(fpout, "%%VARIAVEL\n'Coeficiente_de_Poisson'\n%.6f\n\n", coef_poisson_total);
	fprintf(fpout, "%%VARIAVEL\n'Centroide'\n%+10.6f\t%+10.6f", centroid_total_x,centroid_total_y);
	fclose(fpout);
	
	
	
	
	//libera memória alocada
	free(v_edges.e);
	free(m);
	free(el);
	free(n);
	
	double texec = ((double)(clock()-ti))/CLOCKS_PER_SEC;
	printf("\n%.10lf\n", texec);
	return 0;
}