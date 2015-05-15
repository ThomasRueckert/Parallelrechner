#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

#define MASTER 	0
#define TAG 	1

int main(int argc, char *argv[])
{
	MPI_Status status;
	int myRank, nTasks, i, Buffer;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &nTasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank == MASTER)
	{
		printf("Hi from Master, my Rank is %d\n", myRank);
		printf("Iterations: \n");
		int count = 0;
		scanf("%d", &count);
		count++; //1-Basiert
		if(count > nTasks) count = nTasks;
		MPI_Barrier(MPI_COMM_WORLD);
		for (i = 1; i < count; ++i)
		{
			MPI_Recv(&Buffer, 1, MPI_INT, i, TAG, MPI_COMM_WORLD, &status);
			printf("Received Message from Node %d\n", Buffer);
		}
	}
	else
	{
		MPI_Barrier(MPI_COMM_WORLD);
		MPI_Send(&myRank, 1, MPI_INT, MASTER, TAG, MPI_COMM_WORLD);
	}

	printf("Bye from %d\n", myRank);
	MPI_Finalize();

	return 0;
}
