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
