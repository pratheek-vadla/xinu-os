/* xsh_run.c - xsh_run */

#include <xinu.h>
#include <shprototypes.h>
#include <prodcons.h>

/*------------------------------------------------------------------------
 * xsh_run
 *------------------------------------------------------------------------
 */

sid32 run_status; 

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
		run_status = semcreate(0);
		resume (create(xsh_prodcons, 1024, 20, "prodcons", 2, nargs - 1, &(args[1])));
		wait(run_status);
		return 0;
        }else{
		fprintf(stderr, "%s: invalid argument, unknown process.\n", args[0]);
		return 0;
	}

	return 0;
}
