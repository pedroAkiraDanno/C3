buffer* create_buffer()
{
	buffer* b = (buffer*) malloc((sizeof (buffer))) ;
	b -> run_no = -1 ;
	b -> run_count = 0 ;
	b -> cur = -1 ;
	b -> data = (int*) malloc((sizeof(int)) * keys_per_page * io_buffers) ;
	b -> count = 0 ;
}

int load_buffer(buffer* b , run** runs , int num_runs)
{
	if(b -> run_no >= num_runs)
		return 0 ;
	run* cur = runs[b -> run_no] ;
	int i , j ;
	for(i = 0 ; i < keys_per_page*io_buffers ; i++)
	{
		if(i + b -> run_count >= cur -> count)
			break ;
		b -> data[i] = cur -> data[i + b -> run_count] ;
	}
	b -> run_count += i ;
	b -> cur = 0 ;
	b -> count = i ;
	return i ;
}

int print_buffer(buffer* b)
{
	printf("Run number : %d\n", b -> run_no) ;
	printf("Run count : %d\n", b -> run_count) ;
	printf("Number of elements : %d\n", b -> count) ;
	printf("Data : ");
	int i = 0 ;
	for( ; i < b -> count ; i++)
		printf("%3d ", b -> data[i]);
	printf("\n");
}

void print_buffs(buffer** bs , int num_buffs)
{
	printf("-----------------------------------\n\n") ;
	printf("Printing input buffers : \n\n") ;
	int i ;
	for(i = 0 ; i < num_buffs ; i++)
	{
		printf("Buffer %d \n" , i) ;
		print_buffer(bs[i]) ;
		printf("\n") ;
	}
	printf("-----------------------------------\n") ;
}

void destroy_buffer(buffer* b)
{
	free(b -> data) ;
	free(b) ;
}