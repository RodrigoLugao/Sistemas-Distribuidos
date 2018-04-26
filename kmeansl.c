#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>


void preenche(double **a, int *b){ //aqui a gente enche o vetor de pontos

}
void sorteia(double **cs){ //aqui a gente sorteia 
}


int atualiza(double **cs,int *cPts, double **pts){ //aqui a gente atualiza os centroides
	
}

int main(int argc, char** argv) {
	int my_rank;
	int p; // número de processos
	int c = 4; // número de centróides
	int n = 20; // número de pontos cPontos
	double pontos[20][2]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[20];
	
	int ind;
	//int *pontoCent = (int*) malloc(n * sizeof(int));
	double centroides[4][2];
	double incrCX[4];
	double incrCY[4];
	int totalC[4];
	double incrCXAux[4];
	double incrCYAux[4];
	int totalCAux[4];

	
	for(ind = 0; ind < n; ind++){
		cPontos[ind] = -1;
	}
	
	for(ind = 0; ind < c; ind++){
		incrCX[ind] = 0;
		incrCY[ind] = 0;
		totalC[ind] = 0;
	}
	
	for(ind = 0; ind < n; ind++){
		pontos[ind][0] = ind;
		pontos[ind][1] = ind;
	}
	
	for(ind = 0; ind < c; ind++){
		centroides[ind][0] = ind/(1+ind);
		centroides[ind][1] = ind/(1+ind);
	}
	
	//preenche(pontos, cPontos);
	//sorteia(centroides);
	
	
	
	
	double total; // integral total
	int source; // remetente da integral
	int dest=0; // destino das integrais (nó 0)
	int tag=200; // tipo de mensagem (único)
	
	//Declarar todas as variaveis necessarias
	
	MPI_Status status;

	MPI_Init(&argc, &argv); //Aqui os processos iniciam.
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	int h = n/(p-1); //quantos pontos cada processo vai tomar conta
	int resto = n%p;
	int meu_a, meu_b; // meu_a = primeiro ponto da minha sublista; meu_b = ultimo ponto
	
	meu_a = h * my_rank; //calcula a posição do primeiro ponto que eu preciso tomar conta;
	if(my_rank < p - 1){
		meu_b = meu_a + h -1; //calcula a posição do ultimo ponto que eu preciso tomar conta;
	}else{
		meu_b = n - 1;
	}
	
	double lastDist, newDist; 
	int i, j, newC, oldC, termino;
	int changed = 0;
	int primeiraVez = 1;
	printf("oi");
	termino = 0;
	while(termino < 10){ 	//loop principal. "condicao de parada" é nenhum centroide mudou de lugar
		printf("oi");
		if(!primeiraVez){ // se não é a primeira vez, temos que receber os valores dos centroides atualizados
			if(my_rank != 0){
				for( j = 0; j < c; j++){
					MPI_Recv(&centroides[j][0], 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(&centroides[j][1], 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				}
			}else{
				for(i = 1; i < p; i++){
					for( j = 0; j < c; j++){
						MPI_Send(&centroides[j][0], 1, MPI_DOUBLE, i, 1, MPI_COMM_WORLD);
						MPI_Send(&centroides[j][1], 1, MPI_DOUBLE, i, 2, MPI_COMM_WORLD);
					}
				}
			}
		}else primeiraVez = 0;
		
		MPI_Barrier(MPI_COMM_WORLD);
		
		for(ind = 0; ind < c; ind++){
			incrCX[ind] = 0;
			incrCY[ind] = 0;
			totalC[ind] = 0;
		}
		
		for( i = meu_a; i <= meu_b; i++){ //percorre os pontos de minha responsabilidade
			if(cPontos[i] == -1){
				lastDist = 100000; //->infinito
				oldC = -1;
			}else{
				lastDist = sqrt(pow((pontos[i][0] - centroides[cPontos[i]][0]), 2) + pow((pontos[i][1] - centroides[cPontos[i]][1]), 2));
				oldC = cPontos[i];
			}
			for( j = 0; j < c; j++){ //percorre o vetor de centroides para ver qual é o mais próximo
				newDist = sqrt(pow((pontos[i][0] - centroides[cPontos[i]][0]), 2) + pow((pontos[i][1] - centroides[cPontos[i]][1]), 2));
				if(lastDist > newDist){
					lastDist = newDist;
					newC = j;
					changed = 1;
					cPontos[i]=newC;
				}
			}
			if(changed){
				incrCX[newC] += pontos[i][0];
				incrCX[newC] += pontos[i][1];
				totalC[newC] ++;				
			}
		}

		MPI_Reduce(&incrCY, &incrCYAux, c, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&incrCX, &incrCXAux, c, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&totalC, &totalCAux, c, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

		changed = 0;
		
		if(my_rank == 0) {
			for(i = 0; i < c; i++){
				centroides[i][0] = incrCX[i]/totalC[i];
				centroides[i][1] = incrCY[i]/totalC[i];
				printf("centroide: %d\n", centroides[i][1]);
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		termino ++;
	}
		MPI_Finalize();

}

