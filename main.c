#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define CHARACTERS 1
#define LAYER_LEN 8
int main (int argc, char **argv) {
	if (argc<2) {
		return 1;
	}
	char * filename=argv[1];
	FILE * file = fopen ( filename, "rt");
	read(file);
	fclose(file);
	init_neuralnet(LAYER_LEN,0.5);

	float estimated;
	while(1) {
		estimated=learn(CHARACTERS,LAYER_LEN);
		printf("%f,",estimated);
		if(estimated>1) {
			add_new(estimated);
		}
	}
	return 0;
}
	
