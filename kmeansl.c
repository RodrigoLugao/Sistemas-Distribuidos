#include <stdio.h>
#include <mpi.h>
main(int argc, int* argv) {
	int my_rank;
	int p; // número de processos
	int c; // número de centróides
	int n; // número de pontos 
	float pontos[][]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[]; //indica o id do centroide "dono" do ponto
	float centroides[c][2];
	
	preenche(pontos, cPontos);
	sorteia(centroides);
	
	int h = n/(p-1); //quantos pontos cada processo vai tomar conta
	int resto = n%p;
	int meu_a, meu_b; // meu_a = primeiro ponto da minha sublista; meu_b = ultimo ponto
	
	float total; // integral total
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
	meu_b = meu_a + h -1; //calcula a posição do ultimo ponto que eu preciso tomar conta;
	
	while(!condicao de parada){ 	//loop principal. "condicao de parada" é nenhum centroide mudou de lugar
		// primeiro passo = calcula os novos donos dos pontos que me pertencem
		
		me

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

