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
unsigned int get_FrameLength(unsigned int, unsigned int, unsigned int, unsigned int, unsigned int);
unsigned int convert_SampleRate(unsigned int, unsigned int);
unsigned int convert_BitRate(unsigned int, unsigned int, unsigned int);
unsigned int convert_BitRate_V1(unsigned int, unsigned int);
unsigned int convert_BitRate_V2(unsigned int, unsigned int);
unsigned char* get_FrameData(char*, unsigned int, long int);

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

	frame1.frame_length = get_FrameLength(frame1.layer, frame1.version_id, frame1.bit_rate,
					frame1.sampling_rate, frame1.padding);
	printf("\nFrame Length (data + 4 byte header): %d", frame1.frame_length);
	
	frame1.data = get_FrameData(buffer, frame1.frame_length - 4, index + 4);
	
	unsigned char* ptr = frame1.data;

	printf("\n-------Frame Data-------\n");

	for(int i = 0; i < frame1.frame_length - 4; i++) {
		printf("%02x ", ptr[i]);
	}
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

unsigned int get_FrameLength(unsigned int layer, unsigned int version, 
				unsigned int bit_rate, unsigned int sample_rate, unsigned int padding) {
	
	unsigned int bit_rate_conv = convert_BitRate(bit_rate, version, layer) * 1000;
	unsigned int sample_rate_conv = convert_SampleRate(sample_rate, version);

	// if layer 1
	if(layer == 0x03) {
		return ((unsigned int)(((12.0 * bit_rate_conv) / (sample_rate_conv + padding)) * 4.0));
	} else {
		return ((unsigned int)((144.0 * bit_rate_conv) / (sample_rate_conv + padding)));
	}
}

unsigned char* get_FrameData(char* buffer, unsigned int size, long int startIndex) {
	
	unsigned char* data = malloc(size);
	
	for(int i = 0; i < size; i++) {
		data[i]	= buffer[i + startIndex];
	}

	return data;
}



unsigned int convert_SampleRate(unsigned int sample_rate, unsigned int version) {
	// Version 1
	if(version == 0x03) {
		switch(sample_rate) {
			case 0x00:
				return 44100;
			case 0x01:
				return 48000;
			case 0x02:
				return 32000;
			default: 
				return -1;
		}
	} else if(version == 0x02) {
		// Version 2
		switch(sample_rate) {
			case 0x00:
				return 22050;
			case 0x01:
				return 24000;
			case 0x02:
				return 16000;
			default: 
				return -1;
		}
	
	} else {
		//Version 2.5
		switch(sample_rate) {
			case 0x00:
				return 11025;
			case 0x01:
				return 12000;
			case 0x02:
				return 8000;
			default: 
				return -1;
		}
	}
}

unsigned int convert_BitRate(unsigned int bit_rate, unsigned int version, 
				unsigned int layer) {
	
	// Version 1
	if(version == 0x03) {
		return convert_BitRate_V1(bit_rate, layer);
	} else {
		// Version 2 and 2.5
		return convert_BitRate_V2(bit_rate, layer);
	}
}

unsigned int convert_BitRate_V1(unsigned int bit_rate, unsigned int layer) {
	
	// Layer 1
	if(layer == 0x03) {
		if(bit_rate == 0x00 || bit_rate == 0x0F) {
			return -1;
		} else {
			return bit_rate * 32;
		}
	} else if(layer == 0x02) {
		// Layer 2
		switch(bit_rate) {
			case 0x01:
				return 32;
			case 0x02:
				return 48;
			case 0x03:
				return 56;
			case 0x04:
				return 64;
			case 0x05:
				return 80;
			case 0x06:
				return 96;
			case 0x07:
				return 112;
			case 0x08:
				return 128;
			case 0x09:
				return 160;
			case 0x0A:
				return 192;
			case 0x0B:
				return 224;
			case 0x0C:
				return 256;
			case 0x0D:
				return 320;
			case 0x0E:
				return 384;
			default:
				return -1;
		}
	} else {
		// Layer 3
		switch(bit_rate) {
			case 0x01:
				return 32;
			case 0x02:
				return 40;
			case 0x03:
				return 48;
			case 0x04:
				return 56;
			case 0x05:
				return 64;
			case 0x06:
				return 80;
			case 0x07:
				return 96;
			case 0x08:
				return 112;
			case 0x09:
				return 128;
			case 0x0A:
				return 160;
			case 0x0B:
				return 192;
			case 0x0C:
				return 224;
			case 0x0D:
				return 256;
			case 0x0E:
				return 320;
			default:
				return -1;
		}
	}
}

unsigned int convert_BitRate_V2(unsigned int bit_rate, unsigned int layer) {
	// Layer 1
	if(layer == 0x03) {
		switch(bit_rate) {
			case 0x01:
				return 32;
			case 0x02:
				return 48;
			case 0x03:
				return 56;
			case 0x04:
				return 64;
			case 0x05:
				return 80;
			case 0x06:
				return 96;
			case 0x07:
				return 112;
			case 0x08:
				return 128;
			case 0x09:
				return 144;
			case 0x0A:
				return 160;
			case 0x0B:
				return 176;
			case 0x0C:
				return 192;
			case 0x0D:
				return 224;
			case 0x0E:
				return 256;
			default:
				return -1;
		}
	} else {
		// Layer 2 and 3
		switch(bit_rate) {
			case 0x01:
				return 8;
			case 0x02:
				return 16;
			case 0x03:
				return 24;
			case 0x04:
				return 32;
			case 0x05:
				return 40;
			case 0x06:
				return 48;
			case 0x07:
				return 56;
			case 0x08:
				return 64;
			case 0x09:
				return 80;
			case 0x0A:
				return 96;
			case 0x0B:
				return 112;
			case 0x0C:
				return 128;
			case 0x0D:
				return 144;
			case 0x0E:
				return 160;
			default:
				return -1;
		}
	} 
}


