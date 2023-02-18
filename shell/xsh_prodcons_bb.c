/* xsh_prodcons_bb.c - xsh_prodcons_bb */

#include <xinu.h>
#include <prodcons.h>

/*------------------------------------------------------------------------
 * xsh_prodcons_bb
 *------------------------------------------------------------------------
 */

int arr_q[5];
int tail, head, Q_LEN;

sid32 can_produce;
sid32 can_consume;
sid32 other_producers;
sid32 other_consumers;

shellcmd xsh_prodcons_bb(int nargs, char *args[]) {

	Q_LEN = 5;
	int n, m, i, j;
	

	/* Check argument count */
	if (nargs > 5 || nargs < 5) {
		fprintf(stderr, "Syntax: run prodcons [counter]\n");
		signal(run_status);
		return 1;
	}

	if (nargs == 5) {
		
		n = atoi(args[1]);
		m = atoi(args[2]);
		i = atoi(args[3]);
		j = atoi(args[4]);

		if (n*i != m*j){
			fprintf(stderr, "Iteration Mismatch Error: the number of producer(s) iteration does not match the consumer(s) iteration\n");
                	signal(run_status);
                	return 1;
		}
	}
	
	tail = 0;
	head = 0;	
	can_produce = semcreate(1);
	can_consume = semcreate(0);
	other_producers = semcreate(1);
	other_consumers = semcreate(1);
	
	sid32 producer_status[n];
	sid32 consumer_status[m];

	for(int x=0; x < n; x++){
		producer_status[x] = semcreate(0);
		resume(create(producer_bb, 1024, 20, "producer", nargs, i, x, &producer_status));
	}
	for(int y=0; y < m; y++){
		consumer_status[y] = semcreate(0);
		resume(create(consumer_bb, 1024, 20, "consumer", nargs, j, y, &consumer_status));
	}
	
	for(int x=0; x < n; x++){
                wait(producer_status[x]);
        }
        for(int y=0; y < m; y++){
                wait(consumer_status[y]);
        }
	
	signal(run_status);
		
	return 0;
}
