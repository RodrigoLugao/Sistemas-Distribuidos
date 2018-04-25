#include <stdio.h>
#include <mpi.h>
#include <math.h>
main(int argc, int* argv) {
	int my_rank;
	int p; // número de processos
	int c; // número de centróides
	int n; // número de pontos 
	double pontos[n][2]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[]; //indica o id do centroide "dono" do ponto
	
	int ind;
	
	double centroides[c][2];
	double incrCX[c];
	double incrCY[c];
	
	preenche(pontos, cPontos);
	sorteia(centroides);
	
	for(ind = 0; ind < n; ind++){
		cPontos[ind] = -1;
	}
	
	for(ind = 0; ind < c; ind++){
		incrCX[ind] = 0;
		incrCY[ind] = 0;
	}
	
	int h = n/(p-1); //quantos pontos cada processo vai tomar conta
	int resto = n%p;
	int meu_a, meu_b; // meu_a = primeiro ponto da minha sublista; meu_b = ultimo ponto
	
	double total; // integral total
	int source; // remetente da integral
	int dest=0; // destino das integrais (nó 0)
	int tag=200; // tipo de mensagem (único)
	
	//Declarar todas as variaveis necessarias
	
	MPI_Status status;
	
	int local_n, float h);
	MPI_Init(&argc, &argv); //Aqui os processos iniciam.
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	meu_a = h * my_rank; //calcula a posição do primeiro ponto que eu preciso tomar conta;
	if(my_rank < p - 1){
		meu_b = meu_a + h -1; //calcula a posição do ultimo ponto que eu preciso tomar conta;
	}else{
		meu_b = n - 1;
	}
	
	double lastDist; 
	int i, j;
	while(!condicao de parada){ 	//loop principal. "condicao de parada" é nenhum centroide mudou de lugar
		// primeiro passo = calcula os novos donos dos pontos que me pertencem
		for( i = meu_a; i <= meu_b; i++){ //percorre os pontos de minha responsabilidade
			if(cPontos[i] == -1){
				cPontos[i] = 0;
				lastDist = sqrt(pow((pontos[i][0] - centroides[0][0]), 2) + pow((pontos[i][1] - centroides[0][1]), 2));
			}else{
				lastDist = sqrt(pow((pontos[i][0] - centroides[cPontos[i]][0]), 2) + pow((pontos[i][1] - centroides[cPontos[i]][1]), 2));
			}
			for( j = 1; j < c; j++){ //percorre o vetor de centroides para ver qual é o mais próximo
				
			}
		}

		if(my_rank == 0) {
			total = integral;
			for(source=1; source<p; source++) {
				MPI_Recv(&integral, 1, MPI_FLOAT, source, tag,
				MPI_COMM_WORLD, &status);
				total +=integral;
			}
		} else
			MPI_Send(&integral, 1, MPI_FLOAT, dest,
	tag, MPI_COMM_WORLD);
		}
		if(my_rank == 0) printf("Resultado: %f\n"
			, total);
			MPI_Finalize();
		}
	}
}

void preenche(float **a, int *b){ //aqui a gente enche o vetor de pontos
}

