#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	MPI_Status status;
	int p, myrank;

	int v = 42;

	MPI_Comm_size(MPI_COMM_WORLD, &p);
	MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

	if (myrank % 2 == 0)
	{
		MPI_Send(&v, 1, MPI_INT, myrank+1, 0, MPI_COMM_WORLD);
	}

	if (myrank % 2 == 1)
	{
		MPI_Recv(&v, 1, MPI_INT, myrank-1, 0, MPI_COMM_WORLD, &status);
	}

	return 0;
}
