#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "utils.c"
#include "defs.h"
#include "run.c"
#include "buffer.c"
#include "external.c"

void gen_rand(int n)
{
	printf("%d\n", n) ;
	srand(time(NULL)) ;
	int i = 0 ;
	for( ; i < n ; i++)
		printf("%d\n", rand() % 1000) ;
}

int main(int argc, char const *argv[])
{
	int i , num_runs = 0 ;
	mm_pages = atoi(argv[1]) ;
	keys_per_page = atoi(argv[2]) ;
	io_buffers = atoi(argv[3]) ;
	sort(argv[4]) ;
	return 0;
}