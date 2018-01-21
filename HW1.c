// Student name: Vincent Taylor
// Ilab machine used: pwd.cs.rutgers.edu

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void segment_fault_handler(int signum)
{
	int *move; 
	printf("I am slain!\n");
	*move  = *((&signum)+0);
	//Use the signnum to construct a pointer to flag on stored stack
	//Increment pointer down to the stored PC
	//Increment value at pointer by length of bad instruction
	*move  = ((&move)+4);
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