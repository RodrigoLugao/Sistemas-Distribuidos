#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>



int main(int argc, char** argv) {
	clock_t inicio, fim;
	inicio = clock();
	int my_rank;
	int p; // número de processos
	int c = 4; // número de centróides
	int n = 100; // número de pontos cPontos
	//double pontos[9][2]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[100];
	
	int ind;
	//int *pontoCent = (int*) malloc(n * sizeof(int));
	double centroides[4][2] = {{0,0}, {10,10},{2,2},{4,4}};
	double incrCX[4];
	double incrCY[4];
	int totalC[4];
	double incrCXAux[4];
	double incrCYAux[4];
	int totalCAux[4];
	srand(time(NULL));
	
	int tam=100;

	//printf("Entre com o tamanho do vetor: ");
    	//scanf("%d", &tam);
	double **pontos;
	pontos= (double **) malloc( tam* sizeof(double*));
    
	
	for(ind=0;ind<c;ind++){
	
		centroides[ind][0]=(double)(rand()%1000)/10;
		centroides[ind][1]=(double)(rand()%1000)/10;
		
	}
	for(ind=0;ind<tam;ind++){
		pontos[ind]= (double *) malloc( 2* sizeof(double));
		pontos[ind][0]=(double)(rand()%1000)/10;
		pontos[ind][1]=(double)(rand()%1000)/10;
		//printf("for inicio cluster %d tem como centroide o ponto (%f,%f)\n", ind, centroides[ind][0], centroides[ind][1]);
	}
	
	for(ind = 0; ind < n; ind++){
		cPontos[ind] = -1;
	}
	
	for(ind = 0; ind < c; ind++){
		incrCX[ind] = 0;
		incrCY[ind] = 0;
		totalC[ind] = 0;
	}
	
	
	
	
	//Declarar todas as variaveis necessarias
	
	MPI_Status status;

	MPI_Init(&argc, &argv); //Aqui os processos iniciam.
	
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	
	for(ind=0;ind<c;ind++){
	
		if(my_rank==0){
		//printf("for inicio cluster %d tem como centroide o ponto (%f,%f)\n", ind, centroides[ind][0], centroides[ind][1]);
		}
	}
	
	int h = n/(p); //quantos pontos cada processo vai tomar conta
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
	int parada = 0;
	int primeiraVez = 1;
	termino = 0;
	while(!parada){ 	//loop principal. "condicao de parada" é nenhum centroide mudou de lugar
		parada = 1;
		if(!primeiraVez){ // se não é a primeira vez, temos que receber os valores dos centroides atualizados
			
			if(my_rank != 0){
				for( j = 0; j < c; j++){
					MPI_Recv(&centroides[j][0], 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					MPI_Recv(&centroides[j][1], 1, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
					
//	printf("for recebendoo cluster %d tem como centroide o ponto (%f,%f)\n", j, centroides[j][0], centroides[j][1]);
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
		
		
		
		for(ind = 0; ind < c; ind++){
			incrCX[ind] = 0;
			incrCY[ind] = 0;
			totalC[ind] = 0;
			
			incrCXAux[ind] = 0;
			incrCYAux[ind] = 0;
			totalCAux[ind] = 0;
		}
		
		for( i = meu_a; i <= meu_b; i++){ //percorre os pontos de minha responsabilidade
			changed = 0;
			if(cPontos[i] == -1){
				lastDist = 100000; //->infinito
				oldC = -1;
			}else{
				lastDist = sqrt(pow((pontos[i][0] - centroides[cPontos[i]][0]), 2) + pow((pontos[i][1] - centroides[cPontos[i]][1]), 2));
				oldC = cPontos[i];
			}
			newC=oldC;
			for( j = 0; j < c; j++){ //percorre o vetor de centroides para ver qual é o mais próximo
				newDist = sqrt(pow((pontos[i][0] - centroides[j][0]), 2) + pow((pontos[i][1] - centroides[j][1]), 2));
				if(lastDist > newDist){
					lastDist = newDist;
					newC = j;
					changed = 1;
					
				}else{
					if(lastDist == newDist){
						lastDist = newDist;
						//newC = j;
					}
				}
			}
			cPontos[i] = newC;
				
			if(changed){
				
				parada = 0;
				
			}
			incrCX[newC] = incrCX[newC] + pontos[i][0];
				incrCY[newC] = incrCY[newC] + pontos[i][1];
			
				totalC[newC] ++;
			
				//printf("ponto %d do cluster %d para o cluster %d\n", i, cPontos[i], newC);
		}
		
		//printf("oi eu sou o %d\n", my_rank);
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Reduce(&incrCY, &incrCYAux, c, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&incrCX, &incrCXAux, c, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&totalC, &totalCAux, c, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(&parada, &changed, 1, MPI_INT, MPI_PROD, 0, MPI_COMM_WORLD);
		parada = changed;
		if(my_rank == 0) {
			if(!parada){
				for(i = 0; i < c; i++){
					if(totalCAux[i] != 0){
						centroides[i][0] = incrCXAux[i]/totalCAux[i];
						centroides[i][1] = incrCYAux[i]/totalCAux[i];
					}
					
						//printf("centroide %d: %f, %f\n", i, centroides[i][0], centroides[i][1]);
						//printf("soma x: %d: %f, %f, %d\n", i, incrCXAux[i], incrCYAux[i],totalCAux[i]);
				} 
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Bcast(&parada, 1, MPI_INT, 0, MPI_COMM_WORLD);
		
		//termino++;
	}
	if(my_rank !=0){
		for( i = meu_a; i <= meu_b; i++){
			MPI_Send(&cPontos[i], 1, MPI_INT, 0, i, MPI_COMM_WORLD);
		}
	}else{
		for( i = h; i < n; i++){
			MPI_Recv(&cPontos[i], 1, MPI_INT, MPI_ANY_SOURCE, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
		for( i = 0; i < n; i++){
			//printf("Ponto %d = (%f,%f) pertence ao cluster %d\n", i, pontos[i][0], pontos[i][1], cPontos[i]);
		}
		for(i = 0; i < c; i++){
			//printf("O cluster %d tem como centroide o ponto (%f,%f)\n", i, centroides[i][0], centroides[i][1]);
		}
		
		for(ind=0;ind<n;ind++){
			free(pontos[ind]);
		
		//printf("for inicio cluster %d tem como centroide o ponto (%f,%f)\n", ind, centroides[ind][0], centroides[ind][1]);
		}
		free(pontos);
		fim= clock();

		printf("Duração: %lf \n",(double)((fim - inicio)/(CLOCKS_PER_SEC/1000)));
	}
	MPI_Finalize();
	
}
