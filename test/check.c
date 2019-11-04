#include <check.h>
#include "../header.h"

int compare_buffer(char*, char* , unsigned int);
void print_buffer(char*, unsigned int);

START_TEST(size_test) {
	long int size = audio_size("test/samples/sample.mp3");
	ck_assert(size == 5);
} END_TEST

START_TEST(read_test) {
	long int size = audio_size("test/samples/sample.mp3");
	char* buffer1 = read_audio("test/samples/sample.mp3", size);

	char* buffer2 = malloc(size);
	buffer2[0] = 0x11;
	buffer2[1] = 0xAA;
	buffer2[2] = 0xBB;
	buffer2[3] = 0xFF;
	buffer2[4] = 0xDD;
	
	ck_assert(compare_buffer(buffer1, buffer2, size));
} END_TEST

START_TEST(write_buffer_test) {	

	char* buffer = malloc(5);
	buffer[0] = 0x11;
	buffer[1] = 0xAA;
	buffer[2] = 0xBB;
	buffer[3] = 0xFF;
	buffer[4] = 0xDD;
		
	write_audio_buffer("test/output.mp3", buffer, 5);
	
	char* buffer2 = read_audio("test/output.mp3", 5);

	ck_assert(compare_buffer(buffer, buffer2, 5));
} END_TEST


Suite * test_suite(void)
{
	Suite *s;
	TCase *tc_core;
	s = suite_create("Basic");
	tc_core = tcase_create("Core");
	
	tcase_add_test(tc_core, size_test);
	tcase_add_test(tc_core, read_test);
	tcase_add_test(tc_core, write_buffer_test);

	suite_add_tcase(s, tc_core);
	return s;
}

int main(void)
{	
	int num_failed;
	Suite *s;
	SRunner *sr;
	
	s = test_suite();
	sr = srunner_create(s);
	
	srunner_run_all(sr, CK_NORMAL);
	num_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}

void print_buffer(char* buffer, unsigned int size) {
	for(int i = 0; i < size; i++) {
		printf("%02x", (unsigned char) buffer[i]);
	}
	printf("\n");
}

int compare_buffer(char* buffer1, char* buffer2, unsigned int size) {
	for(int i = 0; i < size; i++) {
		if(((unsigned char)buffer1[i]) != (unsigned char)buffer2[i]) {
			return 0;
		}
	}
	return 1;
}
