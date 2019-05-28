CC=gcc -std=c11
DEPS = Libraries.h AVL_BinarySearchTree.h PRO_Algorithm.h

all: Test

Test: Test.o $(DEPS)
	$(CC) -o Test Test.c AVL_BinarySearchTree.c PRO_Algorithm.c -lm

Test.o: Test.c $(DEPS)
	$(CC) -c Test.c -lm
	
AVL_BinarySearchTree.o: AVL_BinarySearchTree.c $(DEPS)
	$(CC) -c AVL_BinarySearchTree.c -lm
	
PRO_Algorithm.o: PRO_Algorithm.c $(DEPS)
	$(CC) -c PRO_Algorithm.c -lm

clean:
	rm -f *.o Test
