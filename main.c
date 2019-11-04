#include "header.h"

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		printf("Usage: ./main <FILE>\n");
		return 0;
	}

	long int size = audio_size(argv[1]);
	printf("File size: %06lx (%ld)\n", size, size);
	
	char* buffer = read_audio(argv[1], size);
	
	struct frame* frames;

	frames = process_audio(buffer, size);	
	
	long int count = 0;
	
	while(frames[count].frame_length != 0) {
		count++;
	}

	printf("Num of frames: %ld", count);	

	write_audio_frames("output.mp3", frames, count); 
	return 1;
}
