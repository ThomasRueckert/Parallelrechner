#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define SIZE 100000000

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	MPI_Status status;
	int p, myrank, i;
	double t_send, t_ssend;

	int* buffer = (int*) malloc(SIZE*sizeof(int));

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (myrank == 0)
	{
		printf("Daten initialisieren...\n");
	}

	for (i = 0; i < SIZE; i++)
	{
		buffer[i] = i;
	}

	if (myrank == 0)
	{
		printf("Daten verschicken...\n");
	}

	for (i = 1; i < SIZE; i*=2)
	{

		if (myrank == 1)
		{
			t_send = -MPI_Wtime();
			MPI_Send(buffer, i, MPI_INT, 0, 0, MPI_COMM_WORLD);
			t_send += MPI_Wtime();
		}

		if (myrank == 0)
		{
			MPI_Recv(buffer, i, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		}


		if (myrank == 1)
		{
			t_ssend = -MPI_Wtime();
			MPI_Ssend(buffer, i, MPI_INT, 0, 0, MPI_COMM_WORLD);
			t_ssend += MPI_Wtime();
		}

		if (myrank == 0)
		{
			MPI_Recv(buffer, i, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
		}


		if (myrank == 1)
		{
			 printf("Size = %13d,\t t_send = %g,\t t_ssend = %g\n", i, t_send, t_ssend);
		}
	}

	free(buffer);

	MPI_Finalize();
	return 0;
}
