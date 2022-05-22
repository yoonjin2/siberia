#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define CHARACTERS 10
#define LAYER_LEN 1
int main (int argc, char **argv) {
	if (argc<2) {
		return 1;
	}
	char * filename=argv[1];
	FILE * file = fopen ( filename, "rt");
	read(file);
	fclose(file);
	while (1) {
		init_neuralnet(LAYER_LEN,0.7);
		learn(CHARACTERS,LAYER_LEN);
	}
	return 0;
}
	
