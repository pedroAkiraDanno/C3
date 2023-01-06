int comp(const void* a , const void* b) 
{
	return *((int*)a) - *((int*)b) ;
}

run** pass_0(const char* ip , int* op_runs)
{
	// Initial pass . Each run is one page long .
	int i , num_runs , n ;
	int* input = get_input(ip , &n) ;
	run** runs = create_runs(input , n , &num_runs , mm_pages) ;
	for(i = 0 ; i < num_runs ; i++)
	{
		qsort(runs[i] -> data , runs[i] -> count , (sizeof (int)) , comp) ;
	}
	*op_runs = num_runs ;
	return runs ;
}

int min(buffer** buffers , int num_buffs)
{
	int min_ind = 0 , i = 0 , flag = 0 ;
	for(i = 0 ; i < num_buffs ; i++)
	{
		if(buffers[i] -> cur != -1)
		{
			flag = 1 ;
			break ;
		}
	}
	min_ind = i ;
	for( ; i < num_buffs ; i++)
	{
		if(buffers[i] -> cur != -1 && buffers[i] -> data[buffers[i] -> cur] < 
			buffers[min_ind] -> data[buffers[min_ind] -> cur])
		{
			flag = 1 ;
			min_ind = i ;
		}
	}
	if(flag)
		return min_ind ;
	return -1 ;
}

void sort(const char* ip)
{
	int num_runs , i , j , runs_processed ;
	run** runs = pass_0(ip , &num_runs) ;
	print_runs(runs , num_runs) ;
	run** new_runs = NULL ;
	int no_ip = (mm_pages / io_buffers) - 1 ;
	buffer** ip_buffers = (buffer**) malloc(no_ip * (sizeof(buffer*))) ;
	for( i = 0 ; i < no_ip ; i++)
		ip_buffers[i] = create_buffer() ;
	while(num_runs > 1)
	{
		printf("New pass : Runs remaining : %d \n" , num_runs);
		int num_new_runs = (num_runs / no_ip) + (num_runs % no_ip == 0 ? 0 : 1) ;
		run** new_runs = (run**) malloc(num_new_runs * (sizeof(run*))) ;
		int k = 0 ;
		runs_processed = 0 ;
		while(runs_processed < num_runs)
		{
			int count = 0 ;
			for(i = 0 ; i < no_ip ; i++)
			{			
				ip_buffers[i] -> run_no = runs_processed + i ;
				ip_buffers[i] -> run_count = 0 ;
				if(runs_processed + i < num_runs)
					count += runs[runs_processed + i] -> count ;				
				if(load_buffer(ip_buffers[i] , runs , num_runs) == 0)
				{
					ip_buffers[i] -> cur = -1 ;
					ip_buffers[i] -> count = 0 ;
				}
			}
			runs_processed += i ;
			int* new_data = (int*) malloc((sizeof(int)) * count) ;
			int a = 0 ;
			while(1)
			{
				int min_ind = min(ip_buffers , no_ip) ;
				if(min_ind == -1)
					break ;
				new_data[a++] = ip_buffers[min_ind] -> data[ip_buffers[min_ind] -> cur] ;
				ip_buffers[min_ind] -> cur++ ;
				if(ip_buffers[min_ind] -> cur == ip_buffers[min_ind] -> count)
				{
					ip_buffers[min_ind] -> cur = 0 ;
					if(load_buffer(ip_buffers[min_ind] , runs , num_runs) == 0)
						ip_buffers[min_ind] -> cur = -1 ;
				}
			}
			new_runs[k] = (run*) malloc((sizeof(run))) ;
			new_runs[k] -> data = new_data ;
			new_runs[k++] -> count = count ;
		}
		destroy_runs(runs , num_runs) ;
		runs = new_runs ;
		num_runs = num_new_runs ;
		print_runs(runs , num_runs) ;
	}
}