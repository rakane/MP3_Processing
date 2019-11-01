#include "header.h"

int main(int argc, char* argv[]) {
	
	if(argc != 2) {
		printf("Usage: ./main <FILE>\n");
		return 0;
	}

	long int size = audio_size(argv[1]);
	printf("File size: %ld\n", size);
	
	char* buffer = read_audio(argv[1], size);
	
	write_audio("output.mp3", buffer, size);
		
	return 1;
}
