#include <stdio.h>
#include <mpi.h>
main(int argc, int** argv) {
	int my_rank;
	int p; // número de processos
	int c; // número de centróides
	float pontos[][]; //matriz n x 2, onde n é o número de pontos no total
	int cPontos[]; //indica o id do centroide "dono" do ponto
	
	preenche(pontos, cPontos);
	
	float total; // integral total
	int source; // remetente da integral
	int dest=0; // destino das integrais (nó 0)
	int tag=200; // tipo de mensagem (único)
	
	
	
	MPI_Status status;
	
	int local_n, float h);
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	h = (b-a) / n;
	local_n = n / p;
	local_a = a + my_rank * local_n * h;
	local_b = local_a + local_n * h;
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

void preenche(float **a, int *b){ //aqui a gente enche o vetor de pontos
}

