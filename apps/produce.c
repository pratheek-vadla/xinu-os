#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	
	int i = 0;
	while (i <= count){
		wait(can_produce);
		// CRITICAL SECTION
		printf("produced : %d\n", i);
		n = i;
		i += 1;
		signal(can_consume);
		// CRITICAL SECTION END
	}
}
