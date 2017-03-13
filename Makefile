all: sudoku

sudoku: sudoku.c Makefile
	gcc -Wall -O3 sudoku.c -o sudoku

