/* xsh_run.c - xsh_run */

#include <xinu.h>
#include <shprototypes.h>
#include <prodcons.h>

/*------------------------------------------------------------------------
 * xsh_run
 *------------------------------------------------------------------------
 */

int n;
sid32 can_produce;
sid32 can_consume;

shellcmd xsh_run(int nargs, char *args[]) {

	// Print list of available functions
	if ((nargs == 1) || (strncmp(args[1], "list", 4) == 0)) {	
		printf("list\n");
		printf("hello\n");
  		printf("prodcons\n");
  		return 0;
	}else if(strncmp(args[1], "hello", 5) == 0) {
  		/* create a process with the function as an entry point. */
  		resume (create((void *) xsh_hello, 1024, 20, "hello", 2, nargs - 1, &(args[1])));
		return 0;
	}else if(strncmp(args[1], "prodcons", 8) == 0) {
                /* create a process with the function as an entry point. */
                //can_produce = semcreate(123);
		//can_consume = semcreate(456);
		//resume (create((void *) xsh_prodcons, 1024, 20, "prodcons", 2, nargs - 1, &(args[1])));
		//return 0;
		
		int count = 2000;

        	/* Check argument count */
        	if (nargs-1 > 2) {
                	// fprintf(stderr, "%s: too many arguments\n", args[0]);
                	fprintf(stderr, "Syntax: run prodcons [counter]\n");
                	return 1;
        	}

        	if (nargs-1 == 2) {

                	count = atoi(args[2]);
                	if (count < 0){
                        	// fprintf(stderr, "%s: invalid argument, negative value.\n", args[0]);
                        	fprintf(stderr, "Syntax: run prodcons [counter]\n");
                        	return 1;
                	}
        	}

        	can_produce = semcreate(1);
        	can_consume = semcreate(0);

        	resume(create(producer, 1024, 20, "producer", nargs-1, count));
        	resume(create(consumer, 1024, 20, "consumer", nargs-1, count));

        	return 0;
        }else{
		fprintf(stderr, "%s: invalid argument, unknown process.\n", args[0]);
		return 0;
	}

	return 0;
}
