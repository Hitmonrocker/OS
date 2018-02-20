#include "my_pthread_t.h"

unsigned int tid = 5;
MTH* Master;
ucontext_t ctx_main;
ucontext_t ctx_handler;

typedef struct _JUNK {
	int x;
	char y;
}t_struct;
