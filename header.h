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
unsigned char* read_audio(char*, long int);
int write_audio_buffer(char*, unsigned char*, long int);
int write_audio_frames(char*, struct frame*, long int, long int); 
void print_binary(unsigned int num, unsigned int num_bits); 

// Audio processing functions
struct frame* process_audio(unsigned char*, long int);
long int find_Next_Frame(unsigned char*, long int, long int);
unsigned int get_VersionID(unsigned char*, long int);
unsigned int get_Layer(unsigned char*, long int);
unsigned int get_Protection(unsigned char*, long int);
unsigned int get_BitRate(unsigned char*, long int);
unsigned int get_SamplingRate(unsigned char*, long int);
unsigned int get_Padding(unsigned char*, long int);
unsigned int get_Private(unsigned char*, long int);
unsigned int get_ChannelMode(unsigned char*, long int);
unsigned int get_Mode(unsigned char*, long int);
unsigned int get_Copyright(unsigned char*, long int);
unsigned int get_Original(unsigned char*, long int);
unsigned int get_Emphasis(unsigned char*, long int);
unsigned int get_FrameLength(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned char* get_FrameData(unsigned char*, unsigned int, long int);

// Bit and Sample Rate conversion functions
unsigned int convert_SampleRate(unsigned int, unsigned int);
unsigned int convert_BitRate(unsigned int, unsigned int, unsigned int);
unsigned int convert_BitRate_V1(unsigned int, unsigned int);
unsigned int convert_BitRate_V2(unsigned int, unsigned int);


