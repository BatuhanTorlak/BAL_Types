compile:
	gcc -O3 -m64 -Iinclude -c source/*.c
	ar rcs build/bal_types.a *.o
	rm *.o