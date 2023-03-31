/* xsh_run.c - xsh_run */

#include <xinu.h>
#include <shprototypes.h>
#include <prodcons.h>
#include <future.h>
#include <heap.h>

/*------------------------------------------------------------------------
 * xsh_run
 *------------------------------------------------------------------------
 */

sid32 run_status;
sid32 futest_run_status;
sid32 sync_malloc;

shellcmd xsh_run(int nargs, char *args[]) {

	// Print list of available functions
	if ((nargs == 1) || (strcmp(args[1], "list") == 0)) {	
		printf("futest\n");
		printf("hello\n");
		printf("list\n");
		printf("memtest\n");
  		printf("prodcons\n");
  		printf("prodcons_bb\n");
  		return 0;
	}else if(strcmp(args[1], "hello") == 0) {
  		/* create a process with the function as an entry point. */
  		resume (create((void *) xsh_hello, 1024, 20, "hello", 2, nargs - 1, &(args[1])));
		return 0;
	}else if(strcmp(args[1], "prodcons_bb") == 0) {
                /* create a process with the function as an entry point. */
		run_status = semcreate(0);
                resume (create(xsh_prodcons_bb, 1024, 20, "prodcons_bb", 2 ,nargs - 1, &(args[1])));
                wait(run_status);
                return 0;
        }else if(strcmp(args[1], "prodcons") == 0) {
                /* create a process with the function as an entry point. */
		run_status = semcreate(0);
		resume (create(xsh_prodcons, 1024, 20, "prodcons", 2, nargs - 1, &(args[1])));
		wait(run_status);
		return 0;
        }else if(strcmp(args[1], "futest") == 0) {
                /* create a process with the function as an entry point. */
                futest_run_status = semcreate(0);
                resume (create(xsh_futest, 1024, 20, "futest", 2, nargs - 1, &(args[1])));
                wait(futest_run_status);
                // printf("Received Signal from futest. Exiting Run.\n");
		return 0;
	}else if(strcmp(args[1], "memtest") == 0) {
                /* create a process with the function as an entry point. */
                sync_malloc = semcreate(0);
                resume (create(xsh_memtest, 1024, 20, "memtest", 2, nargs - 1, &(args[1])));
                wait(sync_malloc);
                return 0;
        }
	else{
		fprintf(stderr, "%s: invalid argument, unknown process.\n", args[0]);
		return 0;
	}

	return 0;
}
