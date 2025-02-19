#include <stdio.h>
#include <stdlib.h>
#include "./dSFMT-src-2.1/dSFMT.c"
#include "./vector_matrix.c"
#define NUM_R 10000
#define NUM_C 52
#define replicate 100

void shuffle(int *array, int n);
int random_k_species(int k, double **mat);

dsfmt_t dsfmt; //variable for random number generator. Defined as global variable to be used in 'shuffle'.


int main(int argc, char *argv[])
{
	FILE *fp_mat, *fp_sort, *fp_commu;
	double **mat;
	int i, j ,k, h;
	int s, t, tmp; //for loops in bubble sorting
	double n_species, *n_Sp_gene;
	double **rand_commu;
	int num_gene2=0;
	double num_sp = 0.0;
	int col; // index for selecting speicies in community
	int	index[NUM_C+1] = {0};

	// Initialize random number generator
	int seed;
	seed= 2018;//time(NULL);
	//if(seed==0)seed= 1;
	dsfmt_init_gen_rand(&dsfmt,seed);


	mat = d_matrix(NUM_R, NUM_C);
	n_Sp_gene = d_vector(NUM_R); //number of species with a particular gene
	rand_commu = d_matrix(replicate, NUM_C); //value = # of genes, col= # of species in the community


// Read File
	if(argc != 2) message_error("Number of parameter is incompatible.");
	fp_mat = fopen(argv[1], "r"); //open file
	if (fp_mat == NULL) message_error("File cannot be opened.");

	for(i = 1; i <= NUM_R; i++)
	{
		for(j = 1; j <= NUM_C; j++) fscanf(fp_mat, "%lf,", &mat[i][j]);
	}
	fclose(fp_mat);  //close file


// count number of species possessing a gene
	for (i = 1; i <= NUM_R; i++){
		n_species = 0;
		for (j = 1; j <= NUM_C; j++) n_species += mat[i][j];
		n_Sp_gene[i] = n_species;
	}

	// bubble sort to rank "n_Sp_gene"
	for(s = 1; s <= NUM_R; s++) {
		for(t = s + 1; t <= NUM_R; t++) {
			if(n_Sp_gene[t] > n_Sp_gene[s]) {
				tmp = n_Sp_gene[t];
				n_Sp_gene[t] = n_Sp_gene[s];
				n_Sp_gene[s] = tmp;
				}//end if
		}//end for t
	}//end for s

	// write file
	fp_sort = fopen("ranked_gene.txt", "w");
	if(fp_sort == NULL) message_error("File cannot be opened\n");
	fprintf(fp_sort, "rank\tnum_of_species\n");
	for(i = 1; i <= NUM_R; i++) {
		fprintf(fp_sort, "%d\t%f\n", i, n_Sp_gene[i]);
	}
	fclose(fp_sort);
		
	free_d_vector(n_Sp_gene);



// random community
	for (k=2; k<=50; k++){
		for (i=1; i<=replicate; i++) rand_commu[i][k] = random_k_species(k, mat);
	}
	


	// community with 1 species
	for (j=1; j<=NUM_C; j++){
		for (i=1; i<=NUM_R; i++) if (mat[i][j] > 0.01) num_gene2 += 1;
		rand_commu[j][1] = num_gene2;
		num_gene2 = 0;
	}


	// community with 51 species
	for (k=1; k<=NUM_C; k++)
	{
		//prepare an array of index for 51 species by removing the kth species
		for (j=1; j<k; j++) index[j] = j; 
		for (h=k; h<=NUM_C-1; h++) index[h] = h+1;
		
		// count the number of genes in the community of the 51 species
		for (i=1; i<=NUM_R; i++) //loop over every row(gene) of 'mat'
		{ 
			num_sp = 0.0;
			for (j=1; j<=NUM_C-1; j++) // loop over 51 columns(species) of 'mat'
			{ 				
				col = index[j];
				num_sp = num_sp + mat[i][col]; // for every row(gene), count num of species having this gene 
			}
			if (num_sp > 0.01) num_gene2 += 1;	// add 1 if the number of species having this gene > 0.
		}
		rand_commu[k][51] = num_gene2; //num of genes in the community(without the kth species)
		num_gene2 = 0;
	}


	// community with 52 species(all)
	rand_commu[1][NUM_C] = random_k_species(NUM_C, mat);


// write file
	fp_commu = fopen("random_community.txt", "w");
	if (fp_commu == NULL) message_error("File cannot be opened\n");
	for (i = 1; i <= replicate; i++) 
	{
		for (j=1; j<=NUM_C; j++) fprintf(fp_commu, "%lf\t", rand_commu[i][j]);
		fprintf(fp_commu, "\n");
	}

	fclose(fp_commu);

	free_d_matrix(rand_commu);
	free_d_matrix(mat);
	return 0;
}



////////////// Function Definitions /////////////////
/* 
randomly select k species
return the number of functional genes 
in the community consist of these k species.
*/
int random_k_species(int k, double **mat)
{
	int col;
	double num_sp = 0.0;
	int num_gene = 0;
	int index[NUM_C+1] = {0};
	int i,j;
	
	for (i=1; i<=NUM_C; i++) 
	{	
		index[i] = i; //assign value 1-52 to array "index"
	}
	shuffle(index, NUM_C);
	for (i=1; i<=NUM_R; i++){ //loop over every row
		num_sp = 0.0;
		for (j=1; j<=k; j++){ // loop over k columns(species randomly picked)
			col = index[j];
			num_sp = num_sp + mat[i][col]; // for every row(gene), count num of species having this gene 
			//printf("%lf\t%lf\n", num_sp, mat[i][col]);
		}
		if (num_sp > 0.01) num_gene += 1;
	}
	return num_gene;
}


/*
Take an array and its (size-1)(skipping index '0') as input,
shuffle the array.
*/
void shuffle(int *array, int n)
{

    double rand; //random number from uniform [0,1)

    if (n > 1) 
    {
        int i,j,t;
        for (i = 1; i < n; i++) 
        {
            rand = dsfmt_genrand_close_open(&dsfmt);
            int j = i + (n-i+1)*rand;
            int t = array[j];
            array[j] = array[i];
            array[i] = t;
        }
    }
}
