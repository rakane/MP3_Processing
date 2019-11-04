#include "../header.h"

long int audio_size(char* file) {
	
		FILE* f = fopen(file, "rb");
		
		if(f == NULL) {
			printf("File not found!\n");
			return -1;
		}

		fseek(f, 0L, SEEK_END);
		long int result = ftell(f);
		fseek(f, 0, SEEK_SET);

		fclose(f);

		return result;
}

unsigned char* read_audio(char* file, long int size) {
	FILE* input;
	unsigned char* buffer = malloc(size + 1);

	input = fopen(file, "rb");
	
	if(input == NULL) {
		printf("File not found!\n");
		exit(0);
	}

	fread(buffer, size, 1, input);	
	fclose(input);

	return buffer;
}

int write_audio_buffer(char* file, unsigned char* buffer, long int size) {
	FILE * f;

	f = fopen(file, "wb");

	if(f == NULL) {
		printf("File not found!\n");
		return -1;
		exit(0);
	}
	
	fwrite(buffer, size, 1, f);

	fclose(f);
	return 1;
}

char* frames_to_buffer(struct frame* frames, long int frame_count, long int size) {
	unsigned char* buffer = malloc(size);
	long int bufferIndex = 0;
	
	printf("Starting Conversion...\n");
	
	for(int i = 0; i < frame_count; i++) {
			
		int byte = 0;
		
		// Sync bits
		buffer[bufferIndex++] = 0xFF;
		byte = 0xE0;

		// Version ID bits
		byte |= (frames[i].version_id & 0x02) << 3;
		byte |= (frames[i].version_id & 0x01) << 3;
		
		// Layer bits
		byte |= (frames[i].layer & 0x02) << 1;
		byte |= (frames[i].layer & 0x01) << 1;
		
		//Protection bit
		byte |= (frames[i].protection & 0x01);
		
		//Write byte to buffer
		buffer[bufferIndex++] = (unsigned char) byte;
		
		// Bit Rate
		byte = (frames[i].bit_rate & 0x0F) << 4;
	   	
		// Sampling Rate
		byte |= (frames[i].sampling_rate & 0x02) << 2;
		byte |= (frames[i].sampling_rate & 0x01) << 2;

		// Padding bit
		byte |= (frames[i].padding & 0x01) << 1;

		// Private bit
		byte |= (frames[i].private_bit & 0x01);

		// Write byte to buffer
		buffer[bufferIndex++] = (unsigned char) byte;

		// Channel Mode
		byte = (frames[i].channel_mode) << 6;

		// Mode Extension
		byte |=  (frames[i].mode_extension & 0x02) << 4;
		byte |=  (frames[i].mode_extension & 0x01) << 4;

		// Copywrite
		byte |= (frames[i].copyright & 0x01) << 3;

		// Original 
		byte |= (frames[i].original & 0x01) << 2;

		// Emphasis
		byte |= (frames[i].emphasis & 0x02);
		byte |= (frames[i].emphasis & 0x01);

		// Write byte to buffer
		buffer[bufferIndex++] = (unsigned char) byte;
		
		// Data
		unsigned char* ptr = frames[i].data;

		for(int d = 0; d < frames[i].frame_length - 4; d++) {
			buffer[bufferIndex++] =  ptr[d];
		}
		
		// Add padding bit
//		if(frames[i].padding) {
//			buffer[bufferIndex++] = 0x62;
//		}
	}
	
//	printf("Last byte index %06lx\n", bufferIndex);	
	return buffer;
}

int write_audio_frames(char* file, struct frame* frames, long int frame_count, 
				long int size) {

	unsigned char* buffer = frames_to_buffer(frames, frame_count, size);
	printf("Finished conversion!\n");
	write_audio_buffer(file, buffer, size);
}


