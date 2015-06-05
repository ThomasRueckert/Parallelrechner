#include <mpi.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double eukl_distance(double* p1, double* p2, int dims) {

	int cnt = 0;
	double sum = 0;

	for (cnt=0; cnt<dims; cnt++) sum += (p1[cnt]-p2[cnt])*(p1[cnt]-p2[cnt]);

	return sqrt(sum);

}

void generate_data(double *vertices, int num_vertices, int num_dimensions) {

	int cnt;

	for (cnt=0; cnt<num_vertices*num_dimensions; cnt++)
		vertices[cnt] = rand() / (double) RAND_MAX;

}

int main(int argc, char** argv) {

    printf("starting");
  
	int NumRounds = 250;
	int NumClusters = 200;
	int NumDimensions = 10;
	int NumDatapoints = 100000;
	int cnt, cnt2, GroupSize, MyRank, Round;
	int *WorkSize, *Displacements, *ClusterSizes;
	double *NewCentroids, *OldCentroids, *AllVertices, *LocalVertices;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
	MPI_Comm_size(MPI_COMM_WORLD, &GroupSize);
	srand(time(NULL));

	/* generate/read test data */
	if (MyRank == 0) {
		AllVertices = (double*) malloc(NumDatapoints*NumDimensions*sizeof(double));
		generate_data(AllVertices, NumDatapoints, NumDimensions);
	}

	/* prepare distribution of test data */
	WorkSize = (int*) malloc(GroupSize*sizeof(int));
	Displacements = (int*) malloc(GroupSize*sizeof(int));
	for (cnt = 0; cnt < GroupSize; cnt++) {
		int PkgSize = floor(NumDatapoints / (double) GroupSize);
		if (cnt != GroupSize-1) WorkSize[cnt] = PkgSize;
		else WorkSize[cnt] = NumDatapoints - PkgSize * (GroupSize-1);
		WorkSize[cnt] *= NumDimensions;
		if (cnt==0) Displacements[cnt]=0;
		else Displacements[cnt] = Displacements[cnt-1] + PkgSize;
	}
	LocalVertices = malloc(WorkSize[MyRank]*sizeof(double));

	/* distribute test data among ranks */
	MPI_Scatterv(AllVertices, WorkSize, Displacements, MPI_DOUBLE, LocalVertices, WorkSize[MyRank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
	if (MyRank == 0) free(AllVertices);
	free(Displacements);

	/* initialize centroid data */
	NewCentroids = (double*) malloc(NumDimensions*NumClusters*sizeof(double));
	OldCentroids = (double*) malloc(NumDimensions*NumClusters*sizeof(double));
	ClusterSizes = (int*) malloc(NumClusters*sizeof(int));
	for (cnt=0; cnt<NumDimensions*NumClusters; cnt++) OldCentroids[cnt] = rand() / (double) RAND_MAX;

	double start = MPI_Wtime();
	/* main loop */
	for (Round=0; Round<NumRounds; Round++) {

		double MinimalDistance=-1;
		int ClusterWithMinimalDistance = -1;
		int d;

		/* rank 0 distributes centroids for next round */
		for (cnt=0; cnt<NumClusters; cnt++) ClusterSizes[cnt] = 0;
		//
		//TODO hier MPI_Bcast von OldCentroids
		//
		MPI_Bcast(OldCentroids, NumDimensions*NumClusters, MPI_DOUBLE, 0, MPI_COMM_WORLD);
		//void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm

		/* calculate new clustering for each vertex */
		for (cnt=0; cnt<NumDimensions*NumClusters; cnt++) NewCentroids[cnt] = 0;
		for (cnt=0; cnt<WorkSize[MyRank]/NumDimensions; cnt++) {
			for (cnt2=0; cnt2<NumClusters; cnt2++) {
				double dist = eukl_distance(&LocalVertices[cnt*NumDimensions], &OldCentroids[cnt2*NumDimensions], NumDimensions);
				if ((dist < MinimalDistance) || (MinimalDistance < 0)) {
					MinimalDistance = dist;
					ClusterWithMinimalDistance = cnt2;
				}

			}
			for (d=0; d<NumDimensions; d++) NewCentroids[ClusterWithMinimalDistance*NumDimensions+d] += LocalVertices[cnt*NumDimensions+d];
			ClusterSizes[ClusterWithMinimalDistance] += 1;
		}

		/* average local results on root */
		//
		//TODO hier MPI_Reduce von ClusterSizes und NewCentroids (mit MPI_SUM)
		//
		MPI_Reduce(ClusterSizes, ClusterSizes, NumClusters, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(NewCentroids, NewCentroids, NumDimensions*NumClusters, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		for (cnt=0; cnt<NumClusters; cnt++) {
			for (cnt2=0; cnt2<NumDimensions; cnt2++) {
				if (ClusterSizes[cnt] == 0) NewCentroids[cnt*NumDimensions+cnt2] = OldCentroids[cnt*NumDimensions+cnt2];
				else NewCentroids[cnt*NumDimensions+cnt2] /= (double) ClusterSizes[cnt];
			}
		}

		/* swap old and new centroids */
		memcpy(OldCentroids, NewCentroids, NumDimensions*NumClusters*sizeof(double));
	}
	double fin = MPI_Wtime();
	double wtime = fin - start;
	printf("Time needed: %d", wtime);

	MPI_Finalize();
	free(WorkSize); free(ClusterSizes); free(NewCentroids); free(OldCentroids); free(LocalVertices);
	exit(EXIT_SUCCESS);
}
