#include "malloc.c"
//#include "my_P_Thread"
 
int unProtectMem()
{
	int errn = mprotect(&memory,META_SIZE,PROT_READ | PROT_WRITE);
	if(errn == -1)
		{
			printf("Error on Protecting Memory\n");
			exit(0);
		}
}
int protectMem(int size)
{
	unProtectMem();
	if(size > 0)
	{
		buffer = memalign(sysconf(_SC_PAGE_SIZE),size*sysconf(_SC_PAGE_SIZE));
	
		int errn = mprotect(buffer,sysconf(_SC_PAGE_SIZE), PROT_NONE);
		
		if(errn == -1)
		{
			printf("Error on Protecting Memory\n");
			exit(0);
		}
	}
}
