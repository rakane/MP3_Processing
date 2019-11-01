link: lib compile
	gcc main.o -L. libaudio_io.a -o main
	gcc test/check.o -L. libaudio_io.a -o check `pkg-config --cflags --libs check`
	rm main.o
	rm test/check.o
	rm libaudio_io.a
lib:
	gcc -c io/audio_io.c -o audio_io.o
	mv audio_io.o libaudio_io.a
compile:
	gcc -c main.c -o main.o
	gcc -c test/check.c -o test/check.o
clean:
	rm main
	rm check
