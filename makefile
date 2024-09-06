compile:
	gcc -O3 -m64 -Iinclude -c source/*.c
	ar rcs build/libbal_types.a *.o
	rm *.o

test:
	gcc -O3 -m64 -Iinclude -o test.exe test.c source/*.c
	./test.exe