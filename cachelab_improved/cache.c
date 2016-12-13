#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define OFFSET 16777216		// 16 Mb offset
#define BLOCK_SIZE 512
#define L1_SET_SIZE	512		// 512 * 8 = 4KB
#define CHUNK_AMOUNT 32

void cache_clear()
{
	FILE *fp = fopen ("/proc/sys/vm/drop_caches", "w");
	fprintf (fp, "3");
	fclose (fp);
}

static inline uint64_t rdtsc(void)				// Function's code from seminar 
{
	unsigned hi,lo;
	__asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
	return ((unsigned long long)lo) | ( ((unsigned long long)hi) << 32);
}

void cache_trash(int set_num) 
{
	long* chain = (long *) malloc((set_num + 1) * OFFSET * sizeof(long));

	int last_ch_link = 0, item_displ = 0, chunk_displ = 0, ch_link = 0;

	for (item_displ = 0; item_displ < BLOCK_SIZE / (set_num + 1); item_displ++)
	{
		for (chunk_displ = 0; chunk_displ < set_num; chunk_displ++)
		{
			ch_link = chunk_displ * OFFSET + item_displ;
			chain[ch_link] = ch_link + OFFSET;
		}
			
		last_ch_link = set_num * OFFSET + item_displ;
		chain[last_ch_link] = item_displ + 1;
	}
		
	chain[last_ch_link] = 0;

	cache_clear();
	
	int i;
	int index;

	long long start_time = rdtsc();
	
	for (i = 0; i < BLOCK_SIZE; i++)
	{
		index = chain[0];
		
		while (index != 0) 
			index = chain[index];
	}
	
	long long end_time = rdtsc();
	
	float time = ((end_time - start_time)) / BLOCK_SIZE / L1_SET_SIZE;
	fprintf(stdout, "%d %lf\n", set_num + 1, time);
	
	free(chain);
}


/*
void cache_trash(FILE* fp, int set_num) 
{
	long* array = (long *) malloc((set_num+1) * OFFSET * sizeof(long));

	int last_array_index = 0;
	int item_displ = 0;
	int chunk_displ = 0;


	for (item_displ = 0; item_displ < L1_SET_SIZE / (set_num+1); item_displ++)
	{
		for (chunk_displ = 0; chunk_displ < set_num; chunk_displ++)
		{
			int array_index = chunk_displ * OFFSET + item_displ;
			array[array_index] = array_index + OFFSET;
		}
			
		last_array_index = set_num * OFFSET + item_displ;
		array[last_array_index] = item_displ + 1;
	}
		
	array[last_array_index] = 0;

	cache_clear();
	
	int i;
	int index;

	long long start_time = rdtsc();
	
	for (i = 0; i < ATTEMPTS; i++)
	{
		index = array[0];
		
		while (index != 0) 
		{
			index = array[index];
		}
	}
	
	long long end_time = rdtsc();
	
	float time = ((end_time - start_time)) / ATTEMPTS / L1_SET_SIZE;
	fprintf(stdout, "%d %lf\n", set_num + 1, time);
//	fprintf(stdout, "%d, %lld\n", set_num + 1, time);

	free(array);
}
*/

int main() 
{
	int set_num;
	
	for (set_num = 0; set_num < CHUNK_AMOUNT; set_num++)
		cache_trash(set_num);
	
	return 0;
}