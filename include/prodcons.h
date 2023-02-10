/* Global variable for producer consumer */
extern int n; /* this is just declaration */
extern sid32 can_produce; 
extern sid32 can_consume; 
extern sid32 run_status; 
extern sid32 prod_status; 
extern sid32 cons_status; 

/* Function Prototype */
void consumer(int count);
void producer(int count);
