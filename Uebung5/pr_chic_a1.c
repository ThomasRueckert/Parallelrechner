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
	double tmpStart = 0,tmpFin = 0;
	double mathStartTime, mathEndTime;
	double mathTime[1]; mathTime[0]=0;
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

		
	      mathStartTime = MPI_Wtime();
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
		mathEndTime = MPI_Wtime();
		mathTime[0]+= mathEndTime - mathStartTime;

		/* average local results on root */
		//
		//TODO hier MPI_Reduce von ClusterSizes und NewCentroids (mit MPI_SUM)
		//
		MPI_Reduce(ClusterSizes, ClusterSizes, NumClusters, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
		MPI_Reduce(NewCentroids, NewCentroids, NumDimensions*NumClusters, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
		
		mathStartTime = MPI_Wtime();
		for (cnt=0; cnt<NumClusters; cnt++) {
			for (cnt2=0; cnt2<NumDimensions; cnt2++) {
				if (ClusterSizes[cnt] == 0) NewCentroids[cnt*NumDimensions+cnt2] = OldCentroids[cnt*NumDimensions+cnt2];
				else NewCentroids[cnt*NumDimensions+cnt2] /= (double) ClusterSizes[cnt];
			}
		}
		mathEndTime = MPI_Wtime();
		mathTime[0]+= mathEndTime - mathStartTime;

		/* swap old and new centroids */
		memcpy(OldCentroids, NewCentroids, NumDimensions*NumClusters*sizeof(double));
	}
	double endTime = MPI_Wtime();
	double wtime[1];wtime[0] = endTime - start;
	
	double sumWtime[1], sumMathTime[1];
	sumWtime[0] = 0; sumMathTime[0] = 0;
	double minWtime[1], minMathTime[1];
	minWtime[0] = 0; minMathTime[0] = 0;
	double maxWtime[1], maxMathTime[1];
	maxWtime[0] = 0; maxMathTime[0] = 0;
	
	double sumMpiTime, minMpiTime, maxMpiTime;
	
	MPI_Reduce(sumWtime, wtime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	MPI_Reduce(sumMathTime, mathTime, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	MPI_Reduce(minWtime, wtime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
	MPI_Reduce(minMathTime, mathTime, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);

	MPI_Reduce(maxWtime, wtime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	MPI_Reduce(maxMathTime, mathTime, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	sumMpiTime = sumWtime[0] - sumMathTime[0];
	minMpiTime = minWtime[0] - minMathTime[0];
	maxMpiTime = maxWtime[0] - maxMathTime[0];
	
	if (MyRank == 0) {
	  printf("#GroupSize, sum W-Time, min W-Time, max W-Time\n");
	  printf("%d, %d, %d, %d\n", GroupSize, sumWtime[0], minWtime[0], maxWtime[0]);
	  printf("#sumMathTime, minMathTime, maxMathTime\n");
	  printf("%d, %d, %d\n", sumMathTime[0], minMathTime[0], maxMathTime[0]);
	  printf("#sumMpiTime, minMpiTime, maxMpiTime\n");
	  printf("%d, %d, %d\n", sumMpiTime, minMpiTime, maxMpiTime);
	}

	MPI_Finalize();
	free(WorkSize); free(ClusterSizes); free(NewCentroids); free(OldCentroids); free(LocalVertices);
	exit(EXIT_SUCCESS);
}
