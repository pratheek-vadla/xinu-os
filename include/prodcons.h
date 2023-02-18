/* Global variable for producer consumer */
extern int n; /* this is just declaration */
extern int arr_q[5];
extern int tail;
extern int head;
extern int Q_LEN;

/* Semaphores for Producer Consumer & Run */
extern sid32 can_produce; 
extern sid32 can_consume; 
extern sid32 run_status; 
extern sid32 prod_status; 
extern sid32 cons_status;
extern sid32 other_producers;
extern sid32 other_consumers;

/* Function Prototype */
void consumer(int count);
void producer(int count);

void producer_bb(int count, int id, sid32* status);
void consumer_bb(int count, int id, sid32* status);
