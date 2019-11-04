#include "../header.h"

/* Function: audio_size
 * ---------------------
 * Finds the length in bytes of the input file
 *
 * @param file
 *  Name of input file
 * 
 * @return 
 *  Length of file in bytes
 */
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

/* Function: read_audio
 * ---------------------
 * Reads bytes from file to buffer
 *
 * @param file
 *  Name of input file
 * @param size
 *  Length of file in bytes
 *
 * @return 
 *  Filled in buffer with file data
 */
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

/* Function: write_audio_buffer
 * ---------------------
 * Writes buffer of file data to the output file
 *
 * @param file
 *  Name of output file
 * @param buffer
 *  Buffer containing data to be written
 * @param size
 * 	Size of data buffer
 *
 * @return 
 *  -1 if failed, 1 if written
 */
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


/* Function: frames_to_buffer
 * ---------------------
 * Converts array of frames to a byte array 
 *
 * @param frames
 *  Array of audio frames
 * @param frame_count
 *  Number of frames
 * @param size
 *  Size of byte array
 * 
 * @return 
 *  Byte buffer from frame data
 */
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
	}	
	return buffer;
}

/* Function: write_audio_frames
 * ---------------------
 * Writes audio frames to output file
 *
 * @param file
 *  Name of output file
 * @param frames
 *  Array of audio frams
 * @param frame_count
 *  Number of frames 
 * @param size
 *  Size of buffer
 *
 * @return 
 *  -1 if failed, 1 if written
 */
int write_audio_frames(char* file, struct frame* frames, long int frame_count, 
				long int size) {

	unsigned char* buffer = frames_to_buffer(frames, frame_count, size);
	printf("Finished conversion!\n");
	return write_audio_buffer(file, buffer, size);
}

/* Function: print_binary
 * ---------------------
 * Prints num as a binary number
 *
 * @param num
 *  Number to be printed as binary
 * @param num_bits
 *  Number of bits to be printed
 */
void print_binary(unsigned int num, unsigned int num_bits) {
	unsigned int i;
	
	for(i = 1 << (num_bits - 1); i > 0; i = i / 2) {
		if(num & i) {
			printf("1");	   
		} else {
			printf("0");
		}
	}
}

/* Function: print_frame
 * ---------------------
 * Prints out frame header and data
 *
 * @param frame1
 *  Frame to be printed
 */
void print_frame(struct frame frame1) {
	printf("version ID: ");
	print_binary(frame1.version_id, 2);

	printf("\nlayer description: ");
	print_binary(frame1.layer, 2);

	printf("\nProtection bit: ");
	print_binary(frame1.protection, 1);
	
	printf("\nBit Rate: ");
	print_binary(frame1.bit_rate, 4);

	printf("\nSampling Rate: ");
	print_binary(frame1.sampling_rate, 2);

	printf("\nPadding bit: ");
	print_binary(frame1.padding, 1);

	printf("\nPrivate bit: ");
	print_binary(frame1.private_bit, 1);

	printf("\nChannel Mode: ");
	print_binary(frame1.channel_mode, 2);

	printf("\nMode Extension: ");
	print_binary(frame1.mode_extension, 2);

	printf("\nCopywrite Bit: ");
	print_binary(frame1.copyright, 1);

	printf("\nOriginal Bit: ");
	print_binary(frame1.original, 1);
	
	printf("\nEmphasis: ");
	print_binary(frame1.emphasis, 2);

	printf("\nFrame Length (data + 4 byte header): %d", frame1.frame_length);

	unsigned char* ptr = frame1.data;

	printf("\n-------Frame Data-------\n");

	for(int i = 0; i < frame1.frame_length - 4; i++) {
		printf("%02x ", ptr[i]);
	}

	printf("\n--------End Data--------\n");
}


