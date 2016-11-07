#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/*
 *	Launch with sudo !!!
 *
 *
 */

# define ARRAY_SIZE 536870912  // Allocated memory size
# define OFFSET 16777216	   // 16 Mb offset	   
//# define BLOCK_SIZE 32768 // L1d
//# define BLOCK_SIZE 262144 // L2
# define BLOCK_SIZE 3145728	 // > 3 MB to contain whole L3


static inline uint64_t rdtsc(void)				// Function's code from seminar 
{
	unsigned hi,lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | ( ((unsigned long long)hi) << 32);
}

void cache_clear()
{
	FILE *fp = fopen ("/proc/sys/vm/drop_caches", "w");
	fprintf (fp, "3");
	fclose (fp);
}

int main()
{

	volatile char* arr = (char*)malloc(sizeof(char) * ARRAY_SIZE);

	long long delta = 0;
	long long rdtsc_start = 0;
	long long rdtsc_finish = 0;
	
	cache_clear();
	
	int i, j, N;

	for (N = 1; N <= 32; N++)
	{
		rdtsc_start = rdtsc();

		for (j = 0; j < BLOCK_SIZE/N; j++)
			for (i = 0; i < N; i++)	
				arr[OFFSET * i + j];

		rdtsc_finish = rdtsc();			
		delta = (rdtsc_finish - rdtsc_start)/(N * (BLOCK_SIZE/N));

		fprintf(stdout, "%d %lld\n", N, delta);
		
		cache_clear();	
	}	

			
	return 0;
}

