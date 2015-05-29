#include <stdio.h>
#include <mpi.h>
#define PPTAG 2

int main(int argc, char **argv)
{
	int rank, size, i;
	struct {
		int a;
		double b;
	} value;
	MPI_Datatype mystruct;
	int blocklens[2];
	MPI_Aint indices[2];
	MPI_Datatype old_types[2];
	MPI_Status status;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);


	/* Ein Element von jedem Typ */
	blocklens[0] = 1;
	blocklens[1] = 1;
	/* Die Basistypen */
	old_types[0] = MPI_INT;
	old_types[1] = MPI_DOUBLE;
	/* Die Adressen jedes Elementes */
	MPI_Address(&value.a, &indices[0]);
	MPI_Address(&value.b, &indices[1]);
	/* Ändern in relative Adressen */
	indices[1] = indices[1] - indices[0];
	indices[0] = 0;
	MPI_Type_struct(2, blocklens, indices, old_types, &mystruct);
	MPI_Type_commit(&mystruct);

	if (rank == 0) {
		value.a = 1;
		value.b = 2;

		MPI_Bcast(&value, 1, mystruct, 0, MPI_COMM_WORLD);
		if (size > 4) {
			MPI_Barrier(MPI_COMM_WORLD);
			if (size <= 4) {
				//Manueller Barrier
				for (i = 1; i < size; i++) {
					MPI_Send(&i, 1, MPI_INT, i, PPTAG,
						 MPI_COMM_WORLD);
				}
			}
			printf("Done.\n");
		}

	} else {
		MPI_Bcast(&value, 1, mystruct, 0, MPI_COMM_WORLD);
		if (size > 4)
			MPI_Barrier(MPI_COMM_WORLD);
		if (size <= 4) {
			//Manueller Barrier
			MPI_Recv(&i, 1, MPI_INT, 0, PPTAG, MPI_COMM_WORLD,
				 &status);
		}
		printf("Process %d got %d and %lf\n", rank, value.a,
		       value.b);
	}

	/* Aufräumen */
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Type_free(&mystruct);
	MPI_Finalize();
	return 0;
}
