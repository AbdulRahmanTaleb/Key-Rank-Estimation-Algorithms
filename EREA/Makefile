CC=gcc -std=c11
DEPS = Libraries.h Heap.h REA_algorithm.h Key_volume.h EREA.h

all: Test

Test: Test.o Heap.o REA_algorithm.o Key_volume.o EREA.o $(DEPS)
	$(CC) -o Test Test.c Heap.c REA_algorithm.c Key_volume.c EREA.c -lm

Test.o: Test.c $(DEPS)
	$(CC) -c Test.c -lm
	
Heap.o: Heap.c $(DEPS)
	$(CC) -c Heap.c -lm
	
REA_algorithm.o : REA_algorithm.c $(DEPS)
	$(CC) -c REA_algorithm.c -lm
	
Key_volume.o: Key_volume.c $(DEPS)
	$(CC) -c Key_volume.c -lm
	
	
EREA.o: EREA.c $(DEPS)
	$(CC) -c EREA.c -lm


clean:
	rm -f *.o Test
