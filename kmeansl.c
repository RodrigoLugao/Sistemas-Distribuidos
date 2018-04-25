#include <stdio.h>
#include <mpi.h>
#include <math.h>
#include <limits.h>
main(int argc, int* argv) {
	int my_rank;
	int p; // número de processos
	int c; // número de centróides
	int n; // número de pontos cPontos
	double pontos[n][2]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[n]; //indica o id do centroide "dono" do ponto
	
	int ind;
	int pontoCent[n][2];
	double centroides[c][2];
	double incrCX[c];
	double incrCY[c];
	int totalC;
	preenche(pontos, cPontos);
	sorteia(centroides);
	
	for(ind = 0; ind < n; ind++){
		cPontos[ind] = ind;
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

	MPI_Init(&argc, &argv); //Aqui os processos iniciam.
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	meu_a = h * my_rank; //calcula a posição do primeiro ponto que eu preciso tomar conta;
	if(my_rank < p - 1){
		meu_b = meu_a + h -1; //calcula a posição do ultimo ponto que eu preciso tomar conta;
	}else{
		meu_b = n - 1;
	}
	
	double lastDist, newDist; 
	int i, j, newC, oldC;
	int changed = 0;
	
	while(1){ 	//loop principal. "condicao de parada" é nenhum centroide mudou de lugar

		if(!primeiraVez){ // se não é a primeira vez, temos que receber os valores dos centroides atualizados
			for( j = 0; j < c; j++){
				MPI_Recv(centroides[j][0], 1, MPI_DOUBLE, j, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				MPI_Recv(centroides[j][1], 1, MPI_DOUBLE, j, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
			//MPI_Recv(&centroides, c*2, MPI_INT, 0, tag,MPI_COMM_WORLD, &status);

		}else primeiraVez = 0;
		
		for( i = meu_a; i <= meu_b; i++){ //percorre os pontos de minha responsabilidade
			if(cPontos[i] == -1){
				lastDist = 100000 //->infinito
				oldC = -1;
			}else{
				lastDist = sqrt(pow((pontos[i][0] - centroides[cPontos[i]][0]), 2) + pow((pontos[i][1][1] - centroides[cPontos[i]][1]), 2));
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
		}
		
			if(changed && my_rank>0){
				MPI_Send(&cPontos, n, MPI_INT, 0, tag, MPI_COMM_WORLD);
			}

			if(my_rank ==0) {
				changed=0;
				for(source=1; source<p; source++) {
				
				MPI_Recv(&pontoCent, 2, MPI_INT, source, tag,MPI_COMM_WORLD, &status);
				meu_a = h * source; //calcula a posição do primeiro ponto que o processo estava cuidando;

				if(source < p - 1){
					meu_b = meu_a + h -1; //calcula a posição do ultimo ponto que o processo estava cuidando;
				}else{
					meu_b = n - 1;
				}
				//atualiza a lista de centroides de cada ponto
				for(k=0;k<meu_b; k++) {
					if(cPontos[k]!=pontoCent[k]){
						cPontos[k]=pontoCent[k];
						changed=1;
					}
				}
				
			}
				
				//se houve mudança de centroide, recalcula os centroides
				if(changed){
					if(atualiza(&centroides, &cPontos, &pontos)){
						for(source=1; source<p; source++) {
							MPI_Send(&centroides, 2*c, MPI_INT, source, tag, MPI_COMM_WORLD);
						}
			
					}else{
						for(source=1; source<p; source++) {
							MPI_Send(&centroides, 2*c, MPI_INT, source, tag, MPI_COMM_WORLD);
						}
					}
				}
		}
		if(my_rank == 0) printf("Resultado: %f\n"			, total);
			
		
	}
		MPI_Finalize();

}

void preenche(float **a, int *b){ //aqui a gente enche o vetor de pontos
}
void sorteia(float *cs){ //aqui a gente sorteia 
}


int atualiza(float *cs,int *cPts, double *pts){ //aqui a gente atualiza os centroides
	
}

