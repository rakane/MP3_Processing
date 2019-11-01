#include "../header.h"

char* read_audio(char* file, long int size) {
	FILE* input;
	char* buffer = malloc(size + 1);

	input = fopen(file, "rb");
	
	if(input == NULL) {
		printf("File not found!\n");
		exit(0);
	}

	fread(buffer, size, 1, input);
	
	fclose(input);

	printf("Buffer size: %ld\n", size);
	
	return buffer;
}


