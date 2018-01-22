// Student name: Vincent Taylor
// Ilab machine used: pwd.cs.rutgers.edu

//Preconditions: Appropriate C libraries,-m32, iLab machines
// Postconditions: Generates Segmentation Fault for
//                               signal handler self-hack

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>


#define space 0x3c
#define badICL 0x6
void segment_fault_handler(int signum)
{ 
	void *p
	printf("I am slain!\n");
	 p = (&signum); //Use the signnum to construct a pointer to flag on stored stack
	p = p + space; //Increment pointer down to the stored PC
	*(int*)p = *(int*)p + badICL;//Increment value at pointer by length of bad instruction
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