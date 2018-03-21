#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#include <errno.h>
#include <malloc.h>

#define malloc(x) myallocate(x, __FILE__, __LINE__,int THREADREQ)
//#define free(x) mydeallocate(x, __FILE__, __LINE__)

#define TOTAL_MEM 8388608	//8mb = 8388608
#define META_SIZE sizeof(struct _block)
#define Page_Size sysconf(_SC_PAGE_SIZE)

char memory[TOTAL_MEM];
void* global_base = NULL;
static int start = 0;
static int curr = 0;
static int numThreads = 0;
static char* buffer;


/*
 * Malloc Struct
 * sizeof(struct _block) == 24
 */
struct _block {
	size_t size;
	struct _block* next;
	int free;
	int threadOwner;
} block;

void* myallocate(int size,char FILE[],int LINE,int THREADREQ);
struct _block* findSpace(struct _block** last, size_t size);
struct _block* reqSpace(struct _block* last, size_t size);

// Used for debugging purposes
void printBlock(struct _block* b) {
	printf("Size: %zu | Free: %d | tid: %d\n",
		b->size, b->free, b->threadOwner);
}

static void handler(int sig, siginfo_t *si, void *unused)
    {
            printf("Got SIGSEGV at address: 0x%lx\n",(long) si->si_addr);
          //  struct _block * current_Block = get_block_ptr(si->si_addr);
           // if(current_Block->threadOwner == getCurrentThread())
          // {
           	//	unProtectMem();
           //} 
            //else
           // {

            //}
      }

void printList(void* global_base) {

	struct _block* ptr = global_base;
	while (ptr != NULL) {
		printBlock(ptr);
		ptr = ptr->next;
	}
}
/*
 * Iterate through the linked list to see if there is a block that's large enough
 */
struct _block* findSpace(struct _block **last, size_t size) {

	struct _block* current = global_base;

	//Iterate through the list, and return pointer to front of free space
	while (current && !(current->free && current->size >= size)) {
		*last = current;
		current = current->next;
	}

	return current;
}

/*
 * If we don't find a free block, request space from the OS via sbrk
 * and add new block to end of linked list
 */
struct _block* reqSpace(struct _block* last, size_t size) {

	//Ask for size of our actual malloc and our block struct
	struct _block* metadata = sbrk(size + META_SIZE);

	//sbrk() returns (void*) -1 when request does not go through
	if (metadata == (void*) -1) {
		return NULL;
	}

	printf("Request successful\n");

	if (last) { //Null on first request
		last->next = metadata;
	}

	//Setup metadata inside block
	metadata->size = size;
	metadata->next = NULL;
	metadata->free = 0;

	return metadata;
}

void* myallocate(int size, char FILE[], int LINE,int THREADREQ) {

	struct _block* metadata;
	 struct sigaction sa;
        sa.sa_flags = SA_SIGINFO;
        sigemptyset(&sa.sa_mask);
        sa.sa_sigaction = handler;
mprotect(memory,META_SIZE,PROT_READ | PROT_WRITE);
if (sigaction(SIGSEGV, &sa, NULL) == -1)
    {
        printf("Fatal error setting up signal handler\n");
        exit(EXIT_FAILURE);    //explode!
    }
	if (size <= 0) {
		return NULL;
	}

	if (global_base == NULL) {	//First call
		printf("WE ENTERING THIS BITCH\n");
		metadata = reqSpace(NULL, size);		
		if (!metadata) {
			return NULL;
		}

		printBlock(metadata);
		global_base = metadata;
	}

	else {
		printf("non-first\n");
		struct _block* last = global_base;

		metadata = findSpace(&last, size);

		if (!metadata) {
			metadata = reqSpace(last, size);
			if (!metadata) {
				return NULL;
			}
		}

		else {	//found free block
			metadata->free = 0;
		}
	}

	//Return metadata+1, since metadata is pointer of type struct _block,
	//we increment the size of one sizeof(struct _block)
	return (metadata + 1);
}

struct _block *get_block_ptr(void *ptr) {
 	return (struct _block*)ptr - 1;
}

void free(void *ptr) {
	if (!ptr) {
	return;
	}

	// TODO: consider merging blocks once splitting blocks is implemented.
	struct _block* block_ptr = get_block_ptr(ptr);
	block_ptr->free = 1;
}

int main() {


	return 0;
}
