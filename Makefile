build/prog: main.c
	mkdir -p build
	gcc main.c -o build/prog -lraylib -lm
	
with assets: build/prog
	cp -r sounds build
	cp -r sprites build
	
run: build/prog
	./build/prog