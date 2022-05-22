#include <stdio.h>
#include <stdint.h>
#define ADJUST_THRESHOLD 0.35
#define SCHEDULE_PRECISION_RATE 1000
#define ALLOWED_ERROR_RATE 0.01
#define FAIL_THRESHOLD 0.2
typedef struct neuron_t {
	float weight;
	float data;
	float net;
	_Bool is_active;
} neuron_t;
void init_neuralnet (int depth , float threshold) ;
void learn (int result_len,int layers);
void read (FILE * instruction);
