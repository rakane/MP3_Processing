#include "../header.h"

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

