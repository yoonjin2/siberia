#include <limits.h>
#include "machine.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#define IGNORE 100000000
#define PREFIX INT_MAX
#define	MAX_FAIL 100

void strrem (char * orig, char *tok) {
	size_t len=strlen(tok);
	char *p = orig;
	while ((p = strstr(p,tok))!=NULL) {
		memmove(p,p+len,strlen(p+len)+1);
	}
}

char * book_orig;
char * book;
float THRESHOLD=0.7;
float cache=0;
int learning_rate=1;
int fail=0;
int DEPTH=0;
uint32_t book_page=0;
neuron_t * initial_neuronset;
neuron_t ** hidden_neuronset;
neuron_t * result_neuronset;
uint64_t input_size=1;
uint64_t result_size=1;

float reLU (float net) {

	if (net<0) {
		return net*0.01;
	}
	return net;
}
void read(FILE *instruction) {
	srand(time(NULL));
	int size=PREFIX;
	uint64_t input_size=1;
	book_orig=(char *)malloc(sizeof(char)*size);
	initial_neuronset = (neuron_t *)realloc(initial_neuronset, sizeof(neuron_t)*input_size);
	fgets( book_orig , PREFIX, instruction );

}


void init_neuralnet (int depth , float threshold) {
	book_page++;
	fprintf(stderr,"Initial Dataset size:%lu" ,strlen(book_orig));
	if(book_page==1) {
		book=malloc(sizeof(char*)*strlen(book_orig));
	} else {
		book=realloc(book,sizeof(char*)*strlen(book_orig));
	}
	if (0.1<THRESHOLD && THRESHOLD < 1) {
		THRESHOLD=threshold;
	}
	DEPTH=depth;

	if(book_orig==NULL) {
		return;
	}
	sscanf(book_orig,"%[^\n]" , book);
	strrem(book_orig,book);
	input_size = 1;
	char * split_ptr = strtok ( book , ",");
	fprintf(stderr,"page: %d, split_ptr:%s\n", book_page, split_ptr);
	while(1) {
		if (split_ptr==NULL) {
			break;
		}
		fprintf(stderr,"page:%d, split_ptr:%s\n", book_page, split_ptr);
		while (initial_neuronset[input_size-1].weight==0.0F) {	
			initial_neuronset[input_size-1].weight=(float)(rand()%HIDDEN_SIZE)/(float)(HIDDEN_SIZE);
		}
		float sp=atof(split_ptr);
		fprintf(stderr,"adding : atof(\"%f\n\")" , sp);
		initial_neuronset[input_size-1].data=sp;
		initial_neuronset[input_size-1].net=0;
		initial_neuronset[input_size-1].is_active=1;
		input_size++;
		initial_neuronset = (neuron_t *)realloc(initial_neuronset,sizeof(neuron_t )*input_size);
		split_ptr = strtok( NULL , "," );
	}
	printf("\n");
	puts("Read task...");

}
void add_new(float nval) {
		input_size++;
		fprintf(stderr,"adding : %f\n" , nval);
		initial_neuronset = (neuron_t *)realloc(initial_neuronset,sizeof(neuron_t )*input_size);
		initial_neuronset[input_size-1].data=nval;
		initial_neuronset[input_size-1].net=0;
		initial_neuronset[input_size-1].is_active=1;
}

	

float learn (int result_len , int layers) { 
	float smaller=0,bigger=0;
	hidden_neuronset=(neuron_t **)malloc(sizeof(neuron_t *)*layers);
	fprintf(stderr,"Input Size is :%ld\n" , input_size);
	if(input_size<2) {
		exit(0);
	}
		
	int bigger_size=HIDDEN_SIZE>input_size?HIDDEN_SIZE:input_size;
	neuron_t * in_neuronset=malloc(sizeof(neuron_t)*bigger_size);
	memcpy(in_neuronset,initial_neuronset,sizeof(neuron_t)*input_size);
	int rep_cp;
	for(rep_cp=0;rep_cp<layers;rep_cp++) {
		hidden_neuronset[rep_cp]=(neuron_t *)malloc(sizeof(neuron_t)*HIDDEN_SIZE);
	}
	fprintf(stderr,"init_neuronset: Initialize input layers\n");

	auto int repeat_Main;
	auto int i=0;	

	for(repeat_Main=0;repeat_Main<layers;repeat_Main++) {	
		fprintf(stderr,"hidden_neuronset: Initialize Hidden layers\n");
		for (i=0;i<HIDDEN_SIZE;i++) {
		while (hidden_neuronset[repeat_Main][i].weight==0.0F) {	
			hidden_neuronset[repeat_Main][i].weight=(float)(rand()%HIDDEN_SIZE)/(float)(HIDDEN_SIZE);
		}
		hidden_neuronset[repeat_Main][i].net=0;
				auto int weight_count=0;
		while(1) {
			for (weight_count=0;weight_count<input_size;weight_count++) {
				if (in_neuronset[weight_count].weight==(float)-IGNORE) {
					continue;
				}
				hidden_neuronset[repeat_Main][i].net+=(in_neuronset[weight_count].data*in_neuronset[weight_count].weight);
			}
			
			hidden_neuronset[repeat_Main][i].data=reLU(hidden_neuronset[repeat_Main][i].net);
			if (hidden_neuronset[repeat_Main][i].weight==0) {
				hidden_neuronset[repeat_Main][i].is_active=0;
				hidden_neuronset[repeat_Main][i].weight=(float)-IGNORE;
				break;
			}
			hidden_neuronset[repeat_Main][i].is_active=reLU(hidden_neuronset[repeat_Main][i].net)>0?1:0;
			smaller=in_neuronset[i].data;
			bigger=hidden_neuronset[repeat_Main][i].data;
			if(smaller==0.0) {
				break;
			}
			if(bigger==0.0) {
				break;
			}
			if (bigger<smaller) {
				float tmp=bigger;
				bigger=smaller;
				smaller=tmp;
			}
		if(smaller/bigger==1.0F) {
			break;
		}
		if(isnan(smaller/bigger)) {
			break;
		}
		fprintf(stderr,"Current Accuracy:%f\n",1.0-smaller/bigger);
		if(((1.0-smaller/bigger)<(ALLOWED_ACCURACY))) {
			hidden_neuronset[repeat_Main][input_size-1].weight+=(bigger-reLU(hidden_neuronset[repeat_Main][input_size-1].net))*learning_rate;
			fail++;
			if((fail%MAX_FAIL==0)&&(learning_rate>0.0099)) {
				learning_rate-=LEARNING_ADJUST_RATE;
				fail=0;
				continue;
			}
		} else {
				break;
			}
		}
	}

	input_size=HIDDEN_SIZE;
	in_neuronset=hidden_neuronset[repeat_Main];
	memcpy(in_neuronset,hidden_neuronset[repeat_Main],sizeof(neuron_t)*input_size);
	}

	repeat_Main=layers-1;
	result_neuronset=(neuron_t *)malloc(sizeof(neuron_t)*result_len);


	repeat_Main=layers-1;

	i=0;
	result_neuronset[i].weight=(float)(rand()%HIDDEN_SIZE)/(float)(HIDDEN_SIZE);


	while(1) {
		auto int weight_count=0;
		result_neuronset[i].net=0;
		for (weight_count=0;weight_count<input_size;weight_count++) {
			if (in_neuronset[weight_count].weight == (float)-IGNORE) {
				continue;
			}
			result_neuronset[i].net+=(hidden_neuronset[repeat_Main][weight_count].net*hidden_neuronset[repeat_Main][weight_count].weight);
		}
		result_neuronset[i].data=reLU(result_neuronset[i].net);
		result_neuronset[i].is_active=fabsf(result_neuronset[i].weight)>0?1:0;
		smaller=hidden_neuronset[repeat_Main][i].data;
		bigger=result_neuronset[i].data;
		if (smaller==0) {
			break;
		}
		if (bigger==0) {
			break;
		}
		if (bigger<smaller) {
				auto float tmp=bigger;
				bigger=smaller;
				smaller=tmp;
		}
		if(smaller/bigger==1.0F) {
			break;
		}
		if(isnan(smaller/bigger)) {
			break;
		}
		fprintf(stderr,"Current Accuracy:%f\n",1.0-smaller/bigger);
		if(((1.0-smaller/bigger)<(ALLOWED_ACCURACY))) {
			result_neuronset[i].weight+=(bigger-reLU(result_neuronset[i].net))*learning_rate;
				if((fail%MAX_FAIL==0)&&(learning_rate>0.1)) {
					learning_rate-=LEARNING_ADJUST_RATE;
					fail=0;
					continue;
				}
		} else {
			break;
		}
	}
	float result=result_neuronset[i].data/pow(HIDDEN_SIZE,layers)/pow(layers,2)/2;
	fprintf(stderr,"estimated:%f\n" , result);
	return result;
}
