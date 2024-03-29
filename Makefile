link: lib compile
	gcc main.o -L. libaudio_io.a libconvert_audio.a libconversion.a -o main
	gcc test/check.o -L. libaudio_io.a libconvert_audio.a libconversion.a -o check `pkg-config --cflags --libs check`
	rm main.o
	rm test/check.o
	rm libaudio_io.a
	rm libconvert_audio.a
	rm libconversion.a
lib:
	gcc -c io/audio_io.c -o audio_io.o
	gcc -c convert/convert_audio.c -o convert_audio.o
	gcc -c convert/conversion.c -o conversion.o
	mv audio_io.o libaudio_io.a
	mv convert_audio.o libconvert_audio.a
	mv conversion.o libconversion.a
compile:
	gcc -c main.c -o main.o
	gcc -c test/check.c -o test/check.o
clean:
	rm main
	rm check
	rm output.mp3
