#include "../header.h"

/* Function: convert_audio
 * ---------------------
 * Converts byte buffer form of audio file to audio frames
 *
 * @param buffer
 *  Byte array of audio data
 * @param size
 *  Size of buffer 
 *
 * @return
 *  Array of frames
 */

struct frame* convert_audio(unsigned char* buffer, long int size) {
	struct frame frame1;
	struct frame* frames = malloc(10000 * sizeof(struct frame));
	
	long int firstIndex = find_First_Frame(buffer, size, 0);
	long int index = firstIndex;
	printf("First frame index: 0x%06lx\n", index);
	
	// Read in first frame data	
	frames[0].version_id = get_VersionID(buffer, index);
	frames[0].layer = get_Layer(buffer, index);
	frames[0].protection = get_Protection(buffer, index);
	frames[0].bit_rate = get_BitRate(buffer, index);
	frames[0].sampling_rate = get_SamplingRate(buffer, index);
	frames[0].padding = get_Padding(buffer, index);
	frames[0].private_bit = get_Private(buffer, index);
	frames[0].channel_mode = get_ChannelMode(buffer, index);
	frames[0].mode_extension = get_Mode(buffer, index);
	frames[0].copyright = get_Copyright(buffer, index);
	frames[0].original = get_Original(buffer, index);
	frames[0].emphasis = get_Emphasis(buffer, index);
	frames[0].frame_length = get_FrameLength(frames[0].layer, frames[0].version_id, frames[0].bit_rate, frames[0].sampling_rate, frames[0].padding);
	frames[0].data = get_FrameData(buffer, frames[0].frame_length - 4, index + 4);
	
//	print_frame(frames[0]);
	
	index = index + frames[0].frame_length;
	long int frameIndex = 1;	
	
	printf("Processing remaining frames\n");

	while(index < size - firstIndex) {
		
		// Read in frame data
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
		
		// increment frameIndex and start frame index
		index = index + frames[frameIndex].frame_length;
		frameIndex++;
	}
	
	printf("\nProcessing Completed!\n");

	return frames;
}

/* Function: find_First_Frame
 * ---------------------
 * Finds the index of the first frame of audio data
 *
 * @param buffer
 *  Byte array of audio data
 * @param size
 *  Buffer size in bytes
 * @param start
 *  Start index for search
 *
 * @return
 *  Index in buffer of first frame
 */
long int find_First_Frame(unsigned char* buffer, long int size, long int start) {
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

/* Function: get_VersionID
 * ---------------------
 * Reads frame version ID from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame version ID
 */
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

/* Function: get_Layer
 * ---------------------
 * Reads frame layer description from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame layer description
 */
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

/* Function: get_Protection
 * ---------------------
 * Reads frame protection bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame protection bit
 */

unsigned int get_Protection(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 1] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

/* Function: get_BitRate
 * ---------------------
 * Reads frame bit rate from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame bit rate
 */
unsigned int get_BitRate(unsigned char* buffer, long int startIndex) {
	unsigned int bit_rate = (buffer[startIndex + 2] >> 4) & 0x0F;
	return bit_rate;
}

/* Function: get_SamplingRate
 * ---------------------
 * Reads frame sampling rate from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame sampling rate
 */
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

/* Function: get_Padding
 * ---------------------
 * Reads frame padding bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame padding bit
 */
unsigned int get_Padding(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x02;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

/* Function: get_Private
 * ---------------------
 * Reads frame private bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame private bit
 */
unsigned int get_Private(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 2] & 0x01;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

/* Function: get_ChannelMode
 * ---------------------
 * Reads frame channel mode from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame channel mode
 */
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

/* Function: get_Mode
 * ---------------------
 * Reads frame mode extension from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame mode extension
 */
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

/* Function: get_Copyright
 * ---------------------
 * Reads frame copyright bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame copyright bit
 */
unsigned int get_Copyright(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x08;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

/* Function: get_Original
 * ---------------------
 * Reads frame original bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame original bit
 */
unsigned int get_Original(unsigned char* buffer, long int startIndex) {
	unsigned int b = buffer[startIndex + 3] & 0x04;

	if(b) {
		return 0x01;
	} else {
		return 0x00;
	}
}

/* Function: get_Emphasis
 * ---------------------
 * Reads frame emphasis bit from buffer
 *
 * @param buffer
 *  Byte array of audio data
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  frame emphasis bit
 */
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

/* Function: get_FrameLength
 * ---------------------
 * Calculates frame length in bytes
 *
 * @param layer
 *  Frame layer description
 * @param version
 *  Frame version ID
 * @param bit_rate
 *  Frame bit rate
 * @param sample_rate
 *  Frame sampling rate
 * @param padding
 *  Frame padding bit
 *
 * @return
 *  frame length in bytes
 */
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

/* Function: get_FrameData
 * ---------------------
 * Reads frame data
 *
 * @param buffer
 *  Byte array of audio data
 * @param size
 *  Buffer size in bytes
 * @param startIndex
 *  Index of the start of frame
 *
 * @return
 *  Byte array of frame data
 */
unsigned char* get_FrameData(unsigned char* buffer, unsigned int size, long int startIndex) {
	
	unsigned char* data = malloc(size);
	
	for(int i = 0; i < size; i++) {
		data[i]	= buffer[i + startIndex];
	}

	return data;
}

