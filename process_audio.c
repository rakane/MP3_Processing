#include "header.h"

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
unsigned int get_FrameLength(unsigned int, unsigned int, unsigned int, unsigned int);

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

struct frame process_audio(char* buffer, long int size) {
	struct frame frame1;

	long int index = find_Next_Frame(buffer, size, 0);
	printf("First frame index: %ld\n", index);
	
	frame1.version_id = get_VersionID(buffer, index);
	printf("version ID: ");
	print_binary(frame1.version_id, 2);

	frame1.layer = get_Layer(buffer, index);
	printf("\nlayer description: ");
	print_binary(frame1.layer, 2);

	frame1.protection = get_Protection(buffer, index);
	printf("\nProtection bit: ");
	print_binary(frame1.protection, 1);
	
	frame1.bit_rate = get_BitRate(buffer, index);
	printf("\nBit Rate: ");
	print_binary(frame1.bit_rate, 4);

	frame1.sampling_rate = get_SamplingRate(buffer, index);
	printf("\nSampling Rate: ");
	print_binary(frame1.sampling_rate, 2);

	frame1.padding = get_Padding(buffer, index);
	printf("\nPadding bit: ");
	print_binary(frame1.padding, 1);

	frame1.private_bit = get_Private(buffer, index);
	printf("\nPrivate bit: ");
	print_binary(frame1.private_bit, 1);

	frame1.channel_mode = get_ChannelMode(buffer, index);
	printf("\nChannel Mode: ");
	print_binary(frame1.channel_mode, 2);

	frame1.mode_extension = get_Mode(buffer, index);
	printf("\nMode Extension: ");
	print_binary(frame1.mode_extension, 2);

	frame1.copyright = get_Copyright(buffer, index);
	printf("\nCopywrite Bit: ");
	print_binary(frame1.copyright, 1);

	frame1.original = get_Original(buffer, index);
	printf("\nOriginal Bit: ");
	print_binary(frame1.original, 1);
	
	frame1.emphasis = get_Emphasis(buffer, index);
	printf("\nEmphasis: ");
	print_binary(frame1.emphasis, 2);


}


long int find_Next_Frame(char* buffer, long int size, long int start) {
	int found_byte = 0;
	int found_frame = 0;

	for(int byte = start; byte < size; byte++) {
		if(found_byte) {
			int b1 = buffer[byte] && 0x80;
			int b2 = buffer[byte] && 0x40;
			int b3 = buffer[byte] && 0x20;
			
			if(b1 && b2 && b3) {
				return byte - 1;
			} else {
				found_byte = 0;
			}
		}

		if((unsigned char) buffer[byte] == 0xFF) {
			found_byte = 1;
		}		
	}

	return -1;	
}

unsigned int get_VersionID(char* buffer, long int startIndex) {
	
	unsigned int b1 = buffer[startIndex + 1] & 0x10;
	unsigned int b2 = buffer[startIndex + 1] & 0x08;	
	unsigned int id;

	if(b1) {
		id = 0x02;		
	} else {
		id = 0x00;
	}
	
	if(b2) {
		id |= 0x01;
	} else {
		id |= 0x01;
	}

	return id;
}

unsigned int get_Layer(char* buffer, long int startIndex) {
	unsigned int b1 = buffer[startIndex + 1] & 0x04;
	unsigned int b2 = buffer[startIndex + 1] & 0x02;	
	unsigned int layer;

	if(b1) {
		layer = 0x02;		
	} else {
		layer = 0x00;
	}
	
	if(b2) {
		layer |= 0x01;
	} else {
		layer |= 0x00;
	}

	return layer;
}

unsigned int get_Protection(char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 1] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_BitRate(char* buffer, long int startIndex) {
	unsigned int bit_rate = (buffer[startIndex + 2] >> 4) & 0x0F;
	return bit_rate;
}

unsigned int get_SamplingRate(char* buffer, long int startIndex) {
	unsigned int b1 = buffer[startIndex + 2] & 0x08;
	unsigned int b2 = buffer[startIndex + 2] & 0x04;
	unsigned int sampling_rate;


	if(b1) {
		sampling_rate = 0x02;		
	} else {
		sampling_rate = 0x00;
	}
	
	if(b2) {
		sampling_rate |= 0x01;
	} else {
		sampling_rate |= 0x00;
	}
	
	return sampling_rate;
}

unsigned int get_Padding(char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x02;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Private(char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_ChannelMode(char* buffer, long int startIndex) {
	unsigned int b1 = buffer[startIndex + 3] & 0x80;
	unsigned int b2 = buffer[startIndex + 3] & 0x40;
	unsigned int channel;


	if(b1) {
		channel = 0x02;		
	} else {
		channel = 0x00;
	}
	
	if(b2) {
		channel |= 0x01;
	} else {
		channel |= 0x00;
	}
	
	return channel;
}

unsigned int get_Mode(char* buffer, long int startIndex) {
	unsigned int b1 = buffer[startIndex + 3] & 0x20;
	unsigned int b2 = buffer[startIndex + 3] & 0x10;
	unsigned int mode;


	if(b1) {
		mode = 0x02;		
	} else {
		mode  = 0x00;
	}
	
	if(b2) {
		mode |= 0x01;
	} else {
		mode |= 0x00;
	}
	
	return mode;
}

unsigned int get_Copyright(char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x08;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Original(char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x04;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Emphasis(char* buffer, long int startIndex) {
	unsigned int b1 = buffer[startIndex + 3] & 0x02;
	unsigned int b2 = buffer[startIndex + 3] & 0x01;
	unsigned int emphasis;


	if(b1) {
		emphasis = 0x02;		
	} else {
		emphasis = 0x00;
	}
	
	if(b2) {
		emphasis |= 0x01;
	} else {
		emphasis |= 0x00;
	}
	
	return emphasis;
}

unsigned int get_FrameLength(unsigned int layer, unsigned int bit_rate, 
				unsigned int sample_rate, unsigned int padding) {
	
	// if layer 1
	if(layer == 0x03) {
		return (((12 * bit_rate) / (sample_rate + padding)) * 4);
	} else {
		return ((144 * bit_rate) / (sample_rate + padding));
	}
}

