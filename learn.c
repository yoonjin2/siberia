#include <limits.h>
#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#define IGNORE 100000000
#define PREFIX 512

void strrem (char * orig, char *tok) {
	size_t len=strlen(tok);
	char *p = orig;
	while ((p = strstr(p,tok))!=NULL) {
		memmove(p,p+len,strlen(p+len)+1);
	}
}

char * book_orig;
float answer;
float THRESHOLD;
neuron_t ** initial_neuronset;
neuron_t ** hidden_neuronset;
neuron_t ** result_neuronset;
uint64_t input_size=1;
uint64_t hidden_size=1;
uint64_t result_size=1;
float learning_rate=0.1;

_Bool response (float net,float target,float threshold) {
	printf("net/target is %f\n" , net/target);
	return net/target < threshold;
}
_Bool schedule_learning_rate(float failrate, _Bool last_strategy) {
	if (last_strategy) {
		learning_rate-=(float)(rand()%SCHEDULE_PRECISION_RATE)/SCHEDULE_PRECISION_RATE*100;
		return 0;
	} else {
		learning_rate+=(float)(rand()%SCHEDULE_PRECISION_RATE)/SCHEDULE_PRECISION_RATE*100;
		return 1;
	}

}
void read(FILE *instruction) {
	uint64_t size=PREFIX;
	uint64_t input_size=1;
	book_orig=(char *)malloc(sizeof(char)*size);
	initial_neuronset = (neuron_t **)malloc(sizeof(neuron_t *)*input_size);
	while (!feof(instruction)) {
		size+=PREFIX;
		fgets( book_orig , 512, instruction );
		book_orig=(char *)realloc(book_orig,sizeof(char)*size);
	}

}


void init_neuralnet (int depth , float threshold) {
	THRESHOLD=threshold;
	
	char * book=malloc(sizeof(char*)*strlen(book_orig));
	sscanf(book_orig,"%[^\n]" , book);
	strrem(book_orig,book);
	input_size = strlen(book);
	if (book==NULL) {
		exit(0);
	}
	char * split_ptr = strtok ( book , ",");

	while (split_ptr!=NULL) {
		
		initial_neuronset[input_size-1]=(neuron_t *)malloc(sizeof(neuron_t));
		initial_neuronset[input_size-1]->data=strtof(split_ptr,NULL);
		initial_neuronset[input_size-1]->weight=-(depth/2)+rand()%(depth/2);
		initial_neuronset[input_size-1]->is_active=1;
		split_ptr = strtok( NULL , "," );
		input_size++;
		initial_neuronset = (neuron_t **)realloc(initial_neuronset , sizeof(neuron_t *)*input_size);
	}
	puts("Read task...");

}

void learn (int result_len , int layers) { 
	puts("Start learning...");

	neuron_t ** in_neuronset=(neuron_t **)malloc(sizeof(neuron_t *)*input_size);
	memcpy(in_neuronset,initial_neuronset, sizeof(neuron_t *)*input_size);
	hidden_size = input_size+1;
	auto int repeat_Main;
	auto _Bool last_strategy=0;
	auto int i=0;	

	for(repeat_Main=0;repeat_Main<layers;repeat_Main++) {	
		hidden_neuronset=(neuron_t **)malloc(sizeof(neuron_t *)*(hidden_size));
		for (i=0;i<hidden_size;i++) {
			hidden_neuronset[i]=(neuron_t *)malloc(sizeof(neuron_t));
			auto int current_repeat=0;
			auto int failrate=0;
			while(1) {
				current_repeat++;
				auto int net_count=0;
				hidden_neuronset[i]->net=0;
				for (net_count=0;net_count<hidden_size;net_count++) {
					if (in_neuronset[net_count]->data==(float)-IGNORE) {
						break;
					}
					hidden_neuronset[i]->net+=in_neuronset[net_count]->weight*in_neuronset[net_count]->data;
				}
				if (hidden_neuronset[i]->net>0) {
					printf("net is %f\n",hidden_neuronset[i]->net);
				} else {
					hidden_neuronset[i]->is_active=0;
					hidden_neuronset[i]->data=(float)-IGNORE;
					break;
				}
				hidden_neuronset[i]->is_active=response(hidden_neuronset[i]->net,in_neuronset[i]->data,THRESHOLD);
				if(!hidden_neuronset[i]->is_active) {
					failrate++;
					last_strategy=schedule_learning_rate(failrate,last_strategy);
					hidden_neuronset[i]->weight+=hidden_neuronset[i]->data*learning_rate*(in_neuronset[i]->data - hidden_neuronset[i]->is_active);
					hidden_neuronset[i]->data=hidden_neuronset[i]->net;
				} else {
					break;
				}
			}
		}
		in_neuronset=(neuron_t **)(neuron_t **)realloc(in_neuronset,sizeof(neuron_t *)*hidden_size);
		memcpy(in_neuronset,hidden_neuronset, sizeof(neuron_t *)*hidden_size);
		input_size=hidden_size;
		hidden_size++;
	}

	hidden_size--;
	result_size=result_len;
	result_neuronset=(neuron_t **)malloc(sizeof(neuron_t *)*result_len);

	auto int current_repeat=0;
	auto int failrate=0;
	for (i=0;i<result_size;i++) {
		result_neuronset[i]=(neuron_t *)malloc(sizeof(neuron_t));
		while(1) {
			current_repeat++;
			auto int net_count=0;
			result_neuronset[i]->net=0;
			for (net_count=0;net_count<input_size;net_count++) {
				if (in_neuronset[net_count]->data == (float)-IGNORE) {
					break;
				}
				result_neuronset[i]->net+=in_neuronset[net_count]->weight*in_neuronset[net_count]->data;
			}
			hidden_neuronset[i]->is_active=fabsf(hidden_neuronset[i]->data)>=0.5?1:0;
			if(!hidden_neuronset[i]->is_active) {
				failrate++;
					last_strategy=schedule_learning_rate(failrate,last_strategy);
				result_neuronset[i]->weight+=result_neuronset[i]->data*learning_rate*(in_neuronset[i]->data - result_neuronset[i]->is_active);
			} else {
				break;
			}
		}
		printf("Data Class %d: learning rate:%f\n" , i+1, (float)(failrate)/(float)(current_repeat));
	}
}
