compile:
	gcc -O3 -m64 -w -Iinclude -c source/*.c
	nasm -f win64 -o bal_asm.o assembly/*.asm
	ar rcs build/wbaltypes.a *.o
	rm *.o

compiledll:
	gcc -O3 -m64 -w -Iinclude -c source/*.c
	nasm -f win64 -o bal_asm.o assembly/*.asm
	gcc -shared -o build/wbaltypes.dll *.o
	rm *.o

test:
	gcc -O3 -m64 -Iinclude -eBalEntry -g -o test.exe test.c build/wbaltypes.a
	./test.exe asd asd 

all: compile test