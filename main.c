#include "header.h"

int main(int arc, char* argv[]) {
	
	long int size = audio_size("test/samples/sample.mp3");
	printf("File size: %ld\n", size);
	
	char* buf = read_audio("test/samples/sample.mp3", size);
	
	return 1;
}
