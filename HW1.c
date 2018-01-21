// Student name: Vincent Taylor
// Ilab machine used: pwd.cs.rutgers.edu

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


#define space 0x14
#define badICL 0x8
void segment_fault_handler(int signum)
{ 
	printf("I am slain!\n");
	void *p  = (&signum); //Use the signnum to construct a pointer to flag on stored stack
	void *q = p + space; //Increment pointer down to the stored PC
	*(int*)q = *(int*)q + badICL;//Increment value at pointer by length of bad instruction
}


int main()
{
	int r2 = 0;

	signal(SIGSEGV, segment_fault_handler);


	r2 = *( (int *) 0 );

	printf("I live again!\n");
	return 0;
}
//
//0x7ffffffddbec
//