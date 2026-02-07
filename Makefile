build/prog: main.c
	mkdir -p build
	cp -r sounds build
	cp -r sprites build
	gcc main.c -o build/prog -lraylib -lm
run: build/prog
	./build/prog