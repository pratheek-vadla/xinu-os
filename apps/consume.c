#include <xinu.h>
#include <prodcons.h>

void consumer(int count) {
	
	int i = 0;
	while (i <= count){
		wait(can_consume);
		printf("consumed : %d\n", n);
		i += 1;
		signal(can_produce);
	}
}
