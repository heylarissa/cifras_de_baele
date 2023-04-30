all: clean beale

beale:
	gcc main.c -o beale -std=c99 -Wall

clean:
	rm -f beale

