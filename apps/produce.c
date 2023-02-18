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

void producer_bb(int count, int id, sid32* status) {

        int i = 0;
        while (i < count){
		wait(other_producers);
                wait(can_produce);
                printf("name : producer_%d, write : %d\n", id, i);
                arr_q[head%Q_LEN] = i;
		head += 1;
		i += 1;
                signal(can_consume);
		signal(other_producers);
        }
        signal(status[id]);
}
