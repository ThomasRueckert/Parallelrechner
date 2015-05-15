#include "mpi.h"
#include <stdlib.h>
#include <stdio.h>

#define MASTER 0
#define TAG    1


/* Funktion zur Simulation einer Berechnung */
int calc_energy(int atoms, float **atom_positions)
{
	int i, j;
	float energy;

	/* Simuliere eine lange Berechnung */
	sleep(1);

	/* "Berechne" Energie */
	energy = 0.0;
	for (i = 0; i < atoms; i++) {
		for (j = 0; j < 3; j++) {
			energy += atom_positions[i][j];
		}
	}

	return energy;
}

int main( int  argc ,  char**argv )  {

	MPI_Status status;
	int myRank, nTasks, Buffer;

    //group
    MPI_Group MPIGroupWorld, MyGroup, OthersGroup;
    MPI_Comm MyComm, OthersComm;
    int GroupSize = 3, GroupRank;
    int *RankList;


	//INIT
	MPI_Init(&argc , &argv );
	MPI_Comm_size(MPI_COMM_WORLD, &nTasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

    MPI_Comm_group(MPI_COMM_WORLD, &MPIGroupWorld);
    MPI_Group_size(MPIGroupWorld, &GroupSize);
    MPI_Group_rank(MPIGroupWorld, &GroupRank);
	//******************************************************************

	int i, j;
	int atoms;
	float **atom_positions;
	float *energies;
	float maxenergy;
    atoms = 10;
    
    int sizeOfGroup = 3;
    int groupRank = (int) (myRank / sizeOfGroup);
    int countGroups = nTasks / sizeOfGroup;

    //group
    RankList = (int *) malloc(sizeOfGroup * sizeof(int));
    int diff = myRank % sizeOfGroup;
    int groupStart = myRank - diff;
    int myGroupRank = myRank - groupStart;
    int groupMaster = 0;
   
    for (i = 0; i < sizeOfGroup; i++) {    
        RankList[i] = groupStart+i;
    }

    MPI_Group_incl(MPIGroupWorld, sizeOfGroup, RankList, &MyGroup);
    MPI_Comm_create(MPI_COMM_WORLD, MyGroup, &MyComm);

    MPI_Group_difference(MPIGroupWorld, MyGroup, &OthersGroup);
    MPI_Comm_create(MPI_COMM_WORLD, OthersGroup, &OthersComm);

	/* 
	 * Allokierung des nötigen Speichers 
	 */

	/* Allokierung des 2-Dimensionalen atom_positions Arrays */

	/* Allokierung einer Liste von float Pointern (erste Dimension) */

	atom_positions = malloc(atoms * sizeof(float *));
	if (atom_positions == NULL) {
		fprintf(stderr, "out of memory\n");
		return -1;
	}
	/* Allokierung der 3 Spalten für die Positionsangaben (zweite Dimension) */
	for (i = 0; i < atoms; i++) {
		atom_positions[i] = malloc(3 * sizeof(float));
		if (atom_positions[i] == NULL) {
			fprintf(stderr, "out of memory\n");
			return -1;
		}
	}

	/* Allokierung und Initialisierung der Energie-Liste */

	energies = malloc(atoms * sizeof(float));
	if (energies == NULL) {
		fprintf(stderr, "out of memory\n");
		return -1;
	}
	for (i = 0; i < atoms; i++) {
		energies[i] = 0.0f;
	}

	/* Einlesen der Atompositionen */
	/* Vereinfachung: normalerweise von Benutzer z.B. durch Eingabedatei übergebbar */
	for (i = 0; i < atoms; i++) {
		for (j = 0; j < 3; j++) {
			atom_positions[i][j] = (i + 1) * (j + 1);
		}
	}
    int recvbuf[1], sendbuf[1];
	/* Bewege jedes Atom in eine der 3 Raumrichtungen und berechne die Energie */
	for (i = 0; i < atoms; i++) {
		/* Verschiebe Atom und bestimme jeweils die Energie 
		 *  Speicher die maximale Energie */

        if (i % countGroups == groupRank) {

		    /* init maxenergy */
		    maxenergy = 0;

		    /*******************************************************************
		     * Parallel Part
		     *******************************************************************/
		     
		    /* verschieben */

		    atom_positions[i][myGroupRank] += 1 * i;
		    /* Energie berechnen */
		    maxenergy = calc_energy(atoms, atom_positions);
		    /* zurücksetzen */
		    atom_positions[i][myGroupRank] -= 1 * i;
	
		
		    
		    sendbuf[0] = maxenergy;
            if (atoms - i > 1) {
                MPI_Reduce(sendbuf, recvbuf, 1, MPI_INT, MPI_MAX, groupMaster, MyComm);
            }

            if (myGroupRank == groupMaster) {
                printf("Energie für Atom %d: %d \n", i + 1, recvbuf[0]);
                energies[i] = recvbuf[0];
            }
		    
		
        }

		if (groupRank == MASTER)  {
		
			//printf("Energie für Atom %d: %d \n", i + 1, recvbuf[0]);
            //energies[i] = recvbuf[0];

		} else {
			
		}
		
		/*******************************************************************
		 * Parallel Part - END
		 *******************************************************************/
		
		
	}

if (myRank == MASTER)  {
	maxenergy = 0;
	for (i = 0; i < atoms; i++) {
		if (energies[i] > maxenergy) {
			maxenergy = energies[i];
		}
	}
	printf("Maximalenergie: %f \n", maxenergy);	
}
	/* Allokationen freigeben */
	free(energies);
	for (i = 0; i < atoms; i++) {
		free(atom_positions[i]);
	}
	free(atom_positions);

	//FIN
	MPI_Finalize();
return 0;
}

