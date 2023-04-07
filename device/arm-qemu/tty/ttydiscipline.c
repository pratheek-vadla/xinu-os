#include <xinu.h>

#define TY_ESC   0x1b
#define TY_BRACE 0x5b
#define TY_A     0x41

#define SKIP     0
#define DO_PRINT 1

local void echo(char ch, struct ttycblk* typtr, struct uart_csreg* csrptr);
//local void print_buff(struct ttycblk* typtr);
local void clearline(struct ttycblk* typtr, struct uart_csreg* csrptr);

/*
 * ttydiscipline - Handles special character interactions.
 * 
 * Returns - Whether to print character (SKIP or DO_PRINT)
 */
int ttydiscipline(char ch,
		  struct ttycblk *typtr,
		  struct uart_csreg* csrptr) {

  if (ch == TY_NEWLINE || ch == TY_RETURN) {
    /* 
     * Copy the contents of the 'tyibuff' buffer from the 'tyihead' through 'tyitail'
     *     into the 'typrev' buffer.
     */
	char *head = typtr->tyihead;
	int i = 0;
	while(i < typtr->tyicursor) {
		typtr->typrev[i] = *head;
		head++;
		if (head >= &typtr->tyibuff[TY_IBUFLEN]){
                	head = typtr->tyibuff;
		}
		i++;
	}
	typtr->typrev[i] = '\0';
  }

  /*
   * Check if the up key was pressed:
   * Use 'tycommand' to indicate if a command is in progress and the previous character
   *     If the characters appear in the sequence TY_ESC, then TY_BRACE, then TY_A
   *     the up key was sent
   */
  if (ch == TY_ESC){
  	typtr->tycommand = ch;
  }else if (typtr->tycommand == TY_ESC && ch == TY_BRACE){
  	typtr->tycommand = ch;
  }else if (typtr->tycommand == TY_BRACE && ch == TY_A){
  // If the up key is detected (as above)
      /*
       * Clear the current input with the 'clearline' function and copy the contents of 
       *     'typrev' buffer into the 'tyibuff' buffer
       *     remember to reset the 'tyicursor' as well
       *  Call 'echo' on each character to display it to the screen
       */
	clearline(typtr, csrptr);
	int i = 0;	
	while(typtr->typrev[i] != '\0'){
		
		*typtr->tyitail++ = typtr->typrev[i];
		typtr->tyicursor++;
		echo(typtr->typrev[i], typtr, csrptr);
		if (typtr->tyitail >= &typtr->tyibuff[TY_IBUFLEN]){
                        typtr->tyitail = typtr->tyibuff;
		}
		i++;
  	}
	return SKIP;
  }
  return DO_PRINT;
}
/*
local void print_buff(struct ttycblk* typtr){
	printf("\n== buffer ==\n");
	char *head = typtr->tyihead;
	int i = 0;
	while(i < typtr->tyicursor){
		printf("%c", *head);
		i++;
		if (head >= &typtr->tyibuff[TY_IBUFLEN]){
                        head = typtr->tyibuff;
		}else{
			head++;
		}
	}
}*/

local void echo(char ch, struct ttycblk* typtr, struct uart_csreg* csrptr) {
  *typtr->tyetail++ = ch;

  /* Wrap around buffer, if needed */
  if (typtr->tyetail >= &typtr->tyebuff[TY_EBUFLEN])
    typtr->tyetail = typtr->tyebuff;
  ttykickout(csrptr);
  return;
}

local void clearline(struct ttycblk* typtr, struct uart_csreg* csrptr) {
  int i, j;
  while (typtr->tyitail != typtr->tyihead)
    if ((--typtr->tyitail) < typtr->tyibuff)
      typtr->tyitail += TY_IBUFLEN;

  for (i=0; i <= typtr->tyicursor; i++) {
    echo(TY_BACKSP, typtr, csrptr);
    echo(TY_BLANK, typtr, csrptr);
    echo(TY_BACKSP, typtr, csrptr);
    for (j=0; j<3; j++)
      ttyhandle_out(typtr, csrptr);
  }
  typtr->tyicursor = 0;
}
