compile:
	gcc -O3 -m64 -Iinclude -c source/*.c
	ar rcs build/baltypes.a *.o
	rm *.o

test:
	gcc -O3 -m64 -Iinclude -o test.exe test.c build/baltypes.a
	./test.exe