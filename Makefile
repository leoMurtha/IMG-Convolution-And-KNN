all:
	gcc -o T4 src/T4.c src/filematrix.c src/util.c src/cv.c src/knn.c src/opmath.c -I./include -g -Wall -lm
run:
	./T4
frun:
	valgrind -v --track-origins=yes --leak-check=full --show-leak-kinds=all ./T4
