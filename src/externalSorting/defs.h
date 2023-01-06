int mm_pages ;
int keys_per_page ;
int io_buffers ;

typedef struct run
{
	int* data ;
	int count ;
}run ;

typedef struct buffer
{
	int run_no ;
	int run_count ; // number of pages of current run read
	int cur ;
	int* data ;
	int count ;
}buffer ;

