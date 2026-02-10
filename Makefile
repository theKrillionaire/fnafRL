build/prog: main.c
	mkdir -p build
	gcc main.c -o build/prog -lraylib -lm
	
with assets: build/prog
	mkdir -p build/sprites
	cp -r sounds build
	cp -r sprites/*.png build/sprites
	
run: build/prog
	./build/prog