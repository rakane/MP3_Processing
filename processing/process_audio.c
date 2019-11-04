#include "../header.h"

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

struct frame* process_audio(unsigned char* buffer, long int size) {
	struct frame frame1;
	struct frame* frames = malloc(10000 * sizeof(struct frame));
	
	long int firstIndex = find_Next_Frame(buffer, size, 0);
	long int index = firstIndex;
	printf("First frame index: 0x%06lx\n", index);
	
	frames[0].version_id = get_VersionID(buffer, index);
	printf("version ID: ");
	print_binary(frames[0].version_id, 2);

	frames[0].layer = get_Layer(buffer, index);
	printf("\nlayer description: ");
	print_binary(frames[0].layer, 2);

	frames[0].protection = get_Protection(buffer, index);
	printf("\nProtection bit: ");
	print_binary(frames[0].protection, 1);
	
	frames[0].bit_rate = get_BitRate(buffer, index);
	printf("\nBit Rate: ");
	print_binary(frames[0].bit_rate, 4);

	frames[0].sampling_rate = get_SamplingRate(buffer, index);
	printf("\nSampling Rate: ");
	print_binary(frames[0].sampling_rate, 2);

	frames[0].padding = get_Padding(buffer, index);
	printf("\nPadding bit: ");
	print_binary(frames[0].padding, 1);

	frames[0].private_bit = get_Private(buffer, index);
	printf("\nPrivate bit: ");
	print_binary(frames[0].private_bit, 1);

	frames[0].channel_mode = get_ChannelMode(buffer, index);
	printf("\nChannel Mode: ");
	print_binary(frames[0].channel_mode, 2);

	frames[0].mode_extension = get_Mode(buffer, index);
	printf("\nMode Extension: ");
	print_binary(frames[0].mode_extension, 2);

	frames[0].copyright = get_Copyright(buffer, index);
	printf("\nCopywrite Bit: ");
	print_binary(frames[0].copyright, 1);

	frames[0].original = get_Original(buffer, index);
	printf("\nOriginal Bit: ");
	print_binary(frames[0].original, 1);
	
	frames[0].emphasis = get_Emphasis(buffer, index);
	printf("\nEmphasis: ");
	print_binary(frames[0].emphasis, 2);

	frames[0].frame_length = get_FrameLength(frames[0].layer, frames[0].version_id, frames[0].bit_rate,
					frames[0].sampling_rate, frames[0].padding);
	printf("\nFrame Length (data + 4 byte header): %d", frames[0].frame_length);
	
	frames[0].data = get_FrameData(buffer, frames[0].frame_length - 4, index + 4);
	
	unsigned char* ptr = frames[0].data;

	printf("\n-------Frame Data-------\n");

	for(int i = 0; i < frames[0].frame_length - 4; i++) {
		printf("%02x ", ptr[i]);
	}

	printf("\n--------End Data--------\n");

	index = index + frames[0].frame_length;
	long int frameIndex = 1;	
	
	printf("Processing remaining frames\n");

	while(index < size - firstIndex) {
			
		frames[frameIndex].version_id = get_VersionID(buffer, index);
		frames[frameIndex].layer = get_Layer(buffer, index);
		frames[frameIndex].protection = get_Protection(buffer, index);
		frames[frameIndex].bit_rate = get_BitRate(buffer, index);
		frames[frameIndex].sampling_rate = get_SamplingRate(buffer, index);
		frames[frameIndex].padding = get_Padding(buffer, index);
		frames[frameIndex].private_bit = get_Private(buffer, index);
		frames[frameIndex].channel_mode = get_ChannelMode(buffer, index);
		frames[frameIndex].mode_extension = get_Mode(buffer, index);
		frames[frameIndex].copyright = get_Copyright(buffer, index);
		frames[frameIndex].original = get_Original(buffer, index);
		frames[frameIndex].emphasis = get_Emphasis(buffer, index);
		frames[frameIndex].frame_length = get_FrameLength(frames[frameIndex].layer, frames[frameIndex].version_id, frames[frameIndex].bit_rate, frames[frameIndex].sampling_rate, 
						frames[frameIndex].padding);
		frames[frameIndex].data = get_FrameData(buffer, frames[0].frame_length - 3, index + 4);
		
		unsigned char* ptr1 = frames[frameIndex].data;

//		if(frameIndex > 6000) {
//			printf("Frame index %ld: %06lx\tLength: %d\tPadding: %d\tLast Data byte: %02x\n", frameIndex, index, frames[frameIndex].frame_length, frames[frameIndex].padding,
//							ptr1[frames[frameIndex].frame_length - 5]);
//		}

		index = index + frames[frameIndex].frame_length;
		frameIndex++;
	}
	
	printf("\nProcessing Completed!\n");

	return frames;
}


long int find_Next_Frame(unsigned char* buffer, long int size, long int start) {
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

		if(byte == size - 1) {
			return -1;
		}
	}

	return -1;	
}

unsigned int get_VersionID(unsigned char* buffer, long int startIndex) {
	
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

unsigned int get_Layer(unsigned char* buffer, long int startIndex) {
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

unsigned int get_Protection(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 1] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_BitRate(unsigned char* buffer, long int startIndex) {
	unsigned int bit_rate = (buffer[startIndex + 2] >> 4) & 0x0F;
	return bit_rate;
}

unsigned int get_SamplingRate(unsigned char* buffer, long int startIndex) {
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

unsigned int get_Padding(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x02;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Private(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_ChannelMode(unsigned char* buffer, long int startIndex) {
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

unsigned int get_Mode(unsigned char* buffer, long int startIndex) {
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

unsigned int get_Copyright(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x08;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Original(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x04;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

unsigned int get_Emphasis(unsigned char* buffer, long int startIndex) {
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
		return (unsigned int) (((12 * bit_rate_conv) / sample_rate_conv) + padding) * 4;
	} else {
		return (unsigned int) ((144 * bit_rate_conv) / sample_rate_conv) + padding;
	}
}

unsigned char* get_FrameData(unsigned char* buffer, unsigned int size, long int startIndex) {
	
	unsigned char* data = malloc(size);
	
	for(int i = 0; i < size; i++) {
		data[i]	= buffer[i + startIndex];
	}

	return data;
}

