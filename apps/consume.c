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
	signal(cons_status);
}

void consumer_bb(int count, int id, sid32* status) {

        int i = 0;
        while (i < count){
		wait(other_consumers);
                wait(can_consume);
                printf("name : consumer_%d, read : %d\n", id, arr_q[tail%Q_LEN]);
		i += 1;
		tail += 1;
                signal(can_produce);
		signal(other_consumers);
        }
        signal(status[id]);
}
