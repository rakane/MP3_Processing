link: lib compile
	gcc main.o -L. libaudio_io.a libprocess_audio.a -o main
	gcc test/check.o -L. libaudio_io.a libprocess_audio.a -o check `pkg-config --cflags --libs check`
	rm main.o
	rm test/check.o
	rm libaudio_io.a
	rm libprocess_audio.a
lib:
	gcc -c io/audio_io.c -o audio_io.o
	gcc -c process_audio.c -o process_audio.o
	mv audio_io.o libaudio_io.a
	mv process_audio.o libprocess_audio.a
compile:
	gcc -c main.c -o main.o
	gcc -c test/check.c -o test/check.o
clean:
	rm main
	rm check
