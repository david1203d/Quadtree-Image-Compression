all: build

build: MAIN_runda2.c
	gcc -o quadtree MAIN_runda2.c -std=c11

clean:
	rm quadtree