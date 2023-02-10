#include <xinu.h>
#include <prodcons.h>

void producer(int count) {
	
	int i = 0;
	while (i <= count){
		wait(can_produce);
		printf("produced : %d\n", i);
		n = i;
		i += 1;
		signal(can_consume);
	}
	signal(prod_status);
}
