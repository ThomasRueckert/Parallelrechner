/* Dies ist ein stark vereinfachtes Beispiel, angelehnt  
* an eine Berechnung der Gesamtenergie eines atomaren Systems aus dem 
* Softwarepacket ABINIT.
*
* Dabei muss jedes einzelne Atom in eine von 3 Raumrichtungen verschoben
* und dann jeweils die Energie für das gesamte System berechnet werden.
* Die Energie wird gespeichert. Am Ende soll die größte, bei diesen 
* Berechnungen gefundene, Energie zurückgegeben werden.
* 
* Letzte Änderung: 20120629
* (indent -kr -i8)
*/
#include <stdio.h>
#include <malloc.h>
#include <unistd.h>

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

int main(int argc, char **argv)
{

	int i, j;
	int atoms;
	float **atom_positions;
	float *energies;
	float maxenergy;

	/* 
	 * Initialisierung der Atome 
	 */

	/* Vereinfachung: normalerweise von Benutzer z.B. durch Eingabedatei übergebbar */

	atoms = 10;

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


	/* 
	 * Berechnung 
	 */

	/* Bewege jedes Atom in eine der 3 Raumrichtungen und berechne die Energie */
	for (i = 0; i < atoms; i++) {
		/* Verschiebe Atom und bestimme jeweils die Energie 
		 *  Speicher die maximale Energie */

		/* init maxenergy */
		maxenergy = 0;

		for (j = 0; j < 3; j++) {
			/* verschieben */
			atom_positions[i][j] += 1 * i;
			/* Energie berechnen */
			maxenergy = calc_energy(atoms, atom_positions);
			if (energies[i] < maxenergy) {
				energies[i] = maxenergy;
			}
			/* zurücksetzen */
			atom_positions[i][j] -= 1 * i;
		}
		printf("Energie für Atom %d: %f \n", i + 1, energies[i]);
	}

	maxenergy = 0;
	for (i = 0; i < atoms; i++) {
		if (energies[i] > maxenergy) {
			maxenergy = energies[i];
		}
	}
	printf("Maximalenergie: %f \n", maxenergy);

	/* Allokationen freigeben */
	free(energies);
	for (i = 0; i < atoms; i++) {
		free(atom_positions[i]);
	}
	free(atom_positions);

	return 0;
}
