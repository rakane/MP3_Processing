link: lib compile
	gcc main.o -L. libread_audio.a libaudio_size.a -o main
	gcc test/check.o -L. libread_audio.a libaudio_size.a -o check `pkg-config --cflags --libs check`
	rm main.o
	rm test/check.o
	rm libread_audio.a
	rm libaudio_size.a
lib:
	gcc -c io/read_audio.c -o read_audio.o
	gcc -c io/audio_size.c -o audio_size.o
	mv read_audio.o libread_audio.a
	mv audio_size.o libaudio_size.a
compile:
	gcc -c main.c -o main.o
	gcc -c test/check.c -o test/check.o
clean:
	rm main
	rm check
