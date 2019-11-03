#include <stdio.h>
#include <stdlib.h>
#include <math.h>

struct frame {
	unsigned int version_id;
	unsigned int layer;
	unsigned int protection;
	unsigned int bit_rate;
	unsigned int sampling_rate;
	unsigned int padding;
	unsigned int private_bit;
	unsigned int channel_mode;
	unsigned int mode_extension;
	unsigned int copyright;
	unsigned int original;
	unsigned int emphasis;
	unsigned char* data;
	unsigned int frame_length;
};

long int audio_size(char*);
char* read_audio(char*, long int);
int write_audio(char*, char*, long int);
struct frame process_audio(char*, long int);
long int find_Next_Frame(char*, long int, long int);
unsigned int get_VersionID(char*, long int);
