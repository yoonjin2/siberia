#include <stdio.h>
#include <stdint.h>
#define HIDDEN_SIZE 1000
#define	LEARNING_ADJUST_RATE 0.01
#define ALLOWED_ACCURACY 0.99
#define FAIL_THRESHOLD 0.1
typedef struct neuron_t {
	float weight;
	float data;
	float net;
	_Bool is_active;
} neuron_t;
void init_neuralnet (int depth , float threshold);
float learn (int result_len,int layers);
void read (FILE * instruction);
void add_new(float nval);
