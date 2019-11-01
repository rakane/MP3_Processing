#include "../header.h"

long int audio_size(char* file) {
	
		FILE* f = fopen(file, "r");
		
		if(f == NULL) {
			printf("File not found!\n");
			return -1;
		}

		fseek(f, 0L, SEEK_END);

		long int result = ftell(f);

		fclose(f);

		return result;
}

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

	return buffer;
}

int write_audio(char* file, char* buffer, long int size) {
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
