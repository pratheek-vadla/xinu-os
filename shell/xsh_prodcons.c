/* xsh_prodcons.c - xsh_prodcons */

#include <xinu.h>
#include <prodcons.h>

/*------------------------------------------------------------------------
 * xsh_prodcons
 *------------------------------------------------------------------------
 */
int n;

shellcmd xsh_prodcons(int nargs, char *args[]) {

	int count = 2000;

	/* Check argument count */
	if (nargs > 2) {
		fprintf(stderr, "%s: too many arguments\n", args[0]);
		return 1;
	}

	if (nargs == 2) {

		count = atoi(args[1]);
		if (count < 0){
			fprintf(stderr, "%s: invalid argument, negative value.\n", args[0]);
			return 1;
		}
	}
	
	resume(create(producer, 1024, 20, "producer", nargs, count));
	resume(create(consumer, 1024, 20, "consumer", nargs, count));

	return 0;
}
