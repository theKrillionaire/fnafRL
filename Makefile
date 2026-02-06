build/prog: main.c
	mkdir -p build
	gcc main.c -o build/prog -lraylib -lm