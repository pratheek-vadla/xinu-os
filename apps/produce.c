#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	
	int i = 0;
	while (i <= count){
		printf("produced : %d\n", i);
		n = i;
		i += 1;
	}
}
