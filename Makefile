CC = gcc -g -std=gnu99 -pthread -Wall -Wpedantic -Werror


make:
	cc *.c -o line_processor

clean:
	rm line_processor
