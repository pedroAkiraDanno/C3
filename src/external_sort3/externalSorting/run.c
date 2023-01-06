void print_run(run* r)
{
	int i = 0 ;
	for( ; i < r -> count ; i++)
		printf("%3d ", r -> data[i]);
	printf("\n");
}

void print_runs(run** runs , int num_runs) 
{
	printf("-------------------------------------------------------------------\n") ;
	int i ;
	printf("Number of runs : %3d\n", num_runs);
	for(i = 0 ; i < num_runs ; i++)
	{
		printf("Run %3d : %3d keys\n", i , runs[i] -> count);
		print_run(runs[i]) ;
	}
	printf("-------------------------------------------------------------------\n") ;
}

run** create_runs(int* ip , int num , int* n , int pages_per_run)// only used in pass_0 currently
{
	int k = 0 ;
	int i , j , ip_pages = num / keys_per_page + (num % keys_per_page == 0 ? 0 : 1) ;
	int num_runs = ip_pages / pages_per_run + (ip_pages % pages_per_run == 0 ? 0 : 1) ;
	run** res = (run**) malloc((sizeof(run*)) * num_runs) ;
	for(i = 0 ; i < num_runs ; i++)
	{		
		res[i] = (run*) malloc(sizeof(run)) ;
		res[i] -> data = (int*) malloc((sizeof (int)) * pages_per_run * keys_per_page) ;
		for(j = 0 ; j < keys_per_page*pages_per_run ; j++)
		{
			if(k < num) 
				res[i] -> data[j] = ip[k++] ;
			else
				break ;
		}
		res[i] -> count = j ;
	}
	*n = num_runs ;
	return res ;
}

void destroy_run(run* r) 
{
	free(r -> data) ; 
	free(r) ;
}

void destroy_runs(run** runs , int num_runs)
{
	if(runs == NULL)
		return ;
	int i = 0 ;
	for( ; i < num_runs ; i++)
		destroy_run(runs[i]) ;
	free(runs) ;
}