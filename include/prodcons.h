/* Global variable for producer consumer */
extern int n; /* this is just declaration */
extern sid32 can_produce; 
extern sid32 can_consume; 

/* Function Prototype */
void consumer(int count);
void producer(int count);
