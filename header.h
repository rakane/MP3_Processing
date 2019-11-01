#include <stdio.h>
#include <stdlib.h>

struct frame {
	unsigned int frame_sync;
	unsigned int version_id;
	unsigned int layer;
	unsigned int protection;
	unsigned int bit_rate;
	unsigned int sampling_rate;
	unsigned int padding;
	unsigned int frame_length;
	unsigned int* data;
};

long int audio_size(char*);
char* read_audio(char*, long int);
int write_audio(char*, char*, long int);
