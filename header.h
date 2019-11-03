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

// io functions
long int audio_size(char*);
char* read_audio(char*, long int);
int write_audio(char*, char*, long int);
void print_binary(unsigned int num, unsigned int num_bits); 

// Audio processing functions
struct frame process_audio(char*, long int);
long int find_Next_Frame(char*, long int, long int);
unsigned int get_VersionID(char*, long int);
unsigned int get_Layer(char*, long int);
unsigned int get_Protection(char*, long int);
unsigned int get_BitRate(char*, long int);
unsigned int get_SamplingRate(char*, long int);
unsigned int get_Padding(char*, long int);
unsigned int get_Private(char*, long int);
unsigned int get_ChannelMode(char*, long int);
unsigned int get_Mode(char*, long int);
unsigned int get_Copyright(char*, long int);
unsigned int get_Original(char*, long int);
unsigned int get_Emphasis(char*, long int);
unsigned int get_FrameLength(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned char* get_FrameData(char*, unsigned int, long int);

// Bit and Sample Rate conversion functions
unsigned int convert_SampleRate(unsigned int, unsigned int);
unsigned int convert_BitRate(unsigned int, unsigned int, unsigned int);
unsigned int convert_BitRate_V1(unsigned int, unsigned int);
unsigned int convert_BitRate_V2(unsigned int, unsigned int);


