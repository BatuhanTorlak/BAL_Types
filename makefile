compile:
	gcc -O3 -m64 -w -Iinclude -c source/*.c
	nasm -f win64 -o bal_asm.o assembly/*.asm
	ar rcs build/baltypes.a *.o
	rm *.o

compiledll:
	gcc -O3 -m64 -w -Iinclude -c source/*.c
	nasm -f win64 -o bal_asm.o assembly/*.asm
	gcc -shared -o build/baltypes.dll *.o
	rm *.o

test:
	gcc -O3 -m64 -Iinclude -g -ePenguEntry -o test.exe test.c build/baltypes.a
	./test.exe

all: compile test